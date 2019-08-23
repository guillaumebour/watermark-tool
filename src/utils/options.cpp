#include "options.h"

void myoptions::parseArgs(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options(argv[0], "\nWatermarkTool - A basic utility to add a watermark to an image.\n");
        options
            .positional_help("FILE")
            .show_positional_help();

        options
        .add_options()
        ("v,verbosity", "Increase verbosity level")
        ("q,quiet", "Don't output anything")
        ("c,corner", "Set the corner. 0: Upper left / 1:Upper right / 2: Lower left / 3: Lower right", cxxopts::value<int>()->default_value("3"), "N")
        ("i,image", "Image on which to add the watermark", cxxopts::value<std::string>(), "FILE")
        ("o,output", "Output image", cxxopts::value<std::string>(), "FILE")
        ("b,black-watermark", "The black watermark", cxxopts::value<std::string>(), "FILE")
        ("w,white-watermark", "The white watermark", cxxopts::value<std::string>(), "FILE")
        ("h,help", "Print this help");

        options.parse_positional({"image"});
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        if (result.count("verbosity"))
        {
            logger::set_verbosity_level(result.count("verbosity"));
        }

        if (result.count("quiet"))
        {
            logger::set_verbosity_level(logger::VERBOSITY_QUIET);
        }

        if (result.count("corner"))
        {   
            switch(result["corner"].as<int>())
            {
                case 0:
                    watermark::setCorner(CORNER_UL);
                    break;
                case 1:
                    watermark::setCorner(CORNER_UR);
                    break;
                case 2:
                    watermark::setCorner(CORNER_LL);
                    break;
                default:
                    watermark::setCorner(CORNER_LR);
                    break;
            }
        }

        if (result.count("image"))
        {
            watermark::setInputImage(result["image"].as<std::string>());
            logger::vlog(logger::LOG_DEBUG, "Input image set to %s", watermark::getInputImage().c_str());
        } else {
            logger::vlog(logger::LOG_ERROR, "Please provide an image file.");
            exit(-1);
        }

        if (result.count("output"))
        {
            watermark::setOutputImage(result["output"].as<std::string>());
            logger::vlog(logger::LOG_DEBUG, "Output image set to %s", watermark::getOutputImage().c_str());
        }
        else
        {   
            std::string image = watermark::getInputImage();
            cv::String extension = image.substr(image.find_last_of("."), image.length());
            watermark::setOutputImage(image.substr(0, image.find_last_of(".")) + "_WM" + extension);
            logger::vlog(logger::LOG_INFO, "No output provided, output image set to %s", watermark::getOutputImage().c_str());
        }

        if(result.count("black-watermark")){
            watermark::setWatermarkBlack(result["black-watermark"].as<std::string>());
            logger::vlog(logger::LOG_DEBUG, "Black watermark set to %s", watermark::getWatermarkBlack().c_str());
        }

        if(result.count("white-watermark")){
            watermark::setWatermarkWhite(result["white-watermark"].as<std::string>());
            logger::vlog(logger::LOG_DEBUG, "White watermark set to %s", watermark::getWatermarkWhite().c_str());
        }

    } catch (const cxxopts::OptionException& e)
    {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}