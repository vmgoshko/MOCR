//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "ThresholdFilter.h"

void ThresholdFilter::doFilter()
{
	cv::adaptiveThreshold( *mImage, *mImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 71, 15);
}