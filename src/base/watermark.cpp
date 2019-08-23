#include "watermark.h"

cv::Point wm_corner = CORNER_LR;
std::string input_image = "";
std::string output_image = "";
std::string watermark_white = "";
std::string watermark_black = "";

void addWatermark(const cv::Mat &image, const cv::Mat &watermark, cv::Mat &output, cv::Point &wpt)
{
    CV_Assert(image.depth() == CV_8U);
    const int channels = image.channels();

    image.copyTo(output);
    
    switch(channels)
    {
        case 1:
        {
            logger::vlog(logger::LOG_INFO, "Not implemented yet");
            break;
        }
        case 3:
        {
            cv::Mat_<cv::Vec3b> _image = image;
            cv::Mat_<cv::Vec3b> _output = output;
            cv::Mat_<cv::Vec4b> _watermark = watermark;

            for(int x = std::max(0, wpt.x); x < std::min(wpt.x + watermark.cols, image.cols); x++){
                for(int y = std::max(0, wpt.y); y < std::min(wpt.y + watermark.rows, image.rows); y++) {
                    
                    double opacity = (double) _watermark(y - wpt.y, x - wpt.x)[3] / 255.;
                    
                    for(int c = 0; c < channels; c++){
                        unsigned char image_px = _image(y, x)[c];
                        unsigned char watermark_px = _watermark(y - wpt.y, x - wpt.x)[c];    
                        _output(y, x)[c] = image_px * (1. - opacity) + (opacity * watermark_px);
                    }
                }
            }
            output = _output;
        }
    }
}


bool isAreaBright(cv::Mat &image, cv::Mat &watermark, cv::Point &pos) {

    if (pos.x >= image.cols || pos.y >= image.rows) 
    {
        logger::vlog(logger::LOG_WARNING, "Watermark is out of the image's boundaries.");
        return false;
    }

    int pos_x = pos.x;
    int pos_y = pos.y;
    int width = watermark.cols;
    int height = watermark.rows;

    if (pos.x < 0) {
        pos_x = 0;
        width = watermark.cols - abs(pos.x);
    }

    if (pos.y < 0) {
        pos_y = 0;
        height = watermark.rows - abs(pos.y);
    }

    cv::Rect roi = cv::Rect(pos_x, pos_y, width, height); 

    cv::Mat intersect = image(roi);
    cv::Mat gray_scale;
    
    cv::cvtColor(intersect, gray_scale, cv::COLOR_RGB2GRAY);

    return cv::mean(gray_scale)[0] > BRIGHTNESS_THRESHOLD;
}


cv::Mat getWatermark(cv::Mat &background) { 
    cv::Mat watermark;
    if(!watermark::getWatermarkBlack().empty() && !watermark::getWatermarkWhite().empty()) {
        logger::vlog(logger::LOG_DEBUG, "Both watermarks found, mode: adaptive.");
        
        watermark = cv::imread(watermark_white, cv::IMREAD_UNCHANGED);

        cv::Point pt = cv::Point(
        wm_corner.x * (background.cols - watermark.cols), 
        wm_corner.y * (background.rows - watermark.rows));

        if (isAreaBright(background, watermark, pt)) {
            watermark = imread(watermark_black, cv::IMREAD_UNCHANGED);
            logger::vlog(logger::LOG_INFO, "Watermark changed to dark version");
        }
    }
    else if(!watermark::getWatermarkBlack().empty()) {
        logger::vlog(logger::LOG_DEBUG, "Black watermark found, mode: black.");
        watermark = imread(watermark_black, cv::IMREAD_UNCHANGED);
    }
    else if(!watermark::getWatermarkWhite().empty()) {
        logger::vlog(logger::LOG_DEBUG, "White watermark found, mode: white.");
        watermark = imread(watermark_white, cv::IMREAD_UNCHANGED);
    }
    else {
        logger::vlog(logger::LOG_ERROR, "No watermark found, error.");
        exit(-1);
    }
    return watermark;
}

// ***********************************************

void watermark::processSingleImage(std::string image) {
    cv::Mat background = cv::imread(image);
    cv::Mat result;

    if(background.empty()){
        logger::vlog(logger::LOG_ERROR, "The image file \"%s\" could not be read", input_image.c_str());
        exit(-1);
    } 
    
    logger::vlog(logger::LOG_INFO, "%s loaded", input_image.c_str());
    logger::vlog(logger::LOG_DEBUG, "Image dimensions: %d x %d", background.cols, background.rows);
    
    cv::Mat watermark = getWatermark(background);

    cv::Point pt = cv::Point(
        wm_corner.x * (background.cols - watermark.cols), 
        wm_corner.y * (background.rows - watermark.rows));

    addWatermark(background, watermark, result, pt);

    std::vector<int> quality_params;
    quality_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    quality_params.push_back(100);
    
    try {
        cv::imwrite(output_image, result, quality_params);
        logger::vlog(logger::LOG_INFO, "Image saved to: %s", output_image.c_str());
    } catch (std::runtime_error& ex) {
        logger::vlog(logger::LOG_ERROR, "Exception writing image %s. Exception: %s\n", output_image.c_str(), ex.what());
        exit(-1);
    }
}

void watermark::setCorner(cv::Point corner) {
    wm_corner = corner;
}

void watermark::setInputImage(std::string i_img) {
    input_image = i_img;
}

void watermark::setOutputImage(std::string o_img) {
    output_image = o_img;
}

void watermark::setWatermarkBlack(std::string img) {
    watermark_black = img;
}

void watermark::setWatermarkWhite(std::string img) {
    watermark_white = img;
}

std::string watermark::getInputImage() {
    return input_image;
}

std::string watermark::getOutputImage() {
    return output_image;
}

std::string watermark::getWatermarkBlack() {
    return watermark_black;
}

std::string watermark::getWatermarkWhite() {
    return watermark_white;
}
