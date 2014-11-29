//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "GaussianBlurFilter.h"

void GaussianBlurFilter::doFilter()
{
	cv::GaussianBlur( *mImage, *mImage, cv::Size(3, 3), 5, 5);
}