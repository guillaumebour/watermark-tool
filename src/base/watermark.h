#ifndef WATERMARK_H
#define WATERMARK_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp> 
#include "../utils/log.h"

#define CORNER_UL cv::Point(0, 0)
#define CORNER_UR cv::Point(1, 0)
#define CORNER_LL cv::Point(0, 1)
#define CORNER_LR cv::Point(1, 1)

#define BRIGHTNESS_THRESHOLD 127

//void addWatermark(const cv::Mat &image, const cv::Mat &watermark, cv::Mat &output, cv::Point &wpt);
//bool isAreaBright(cv::Mat &image, cv::Mat &watermark, cv::Point &pos);
//setCorner(cv::Point corner);

namespace watermark {

void setInputImage(std::string i_img);
void setOutputImage(std::string o_img);
void setWatermarkBlack(std::string img);
void setWatermarkWhite(std::string img);
void setCorner(cv::Point corner);
std::string getInputImage();
std::string getOutputImage();
std::string getWatermarkBlack();
std::string getWatermarkWhite();
void processSingleImage(std::string image);

}

#endif
