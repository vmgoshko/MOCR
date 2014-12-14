//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//std includes
#include <fstream>
#include <iostream>
#include <time.h>
#include <cmath>

//OCRlib includes
#include "Filters.h"
#include "Utils.h"

void Filter::init(cv::Mat* image)
{
	this->mImage = image;
}

std::string Filter::saveAsTempFile()
{
	return saveTempFile(mImage);
}

void GaussianBlurFilter::doFilter()
{
	cv::GaussianBlur(*mImage, *mImage, cv::Size(3, 3), 5, 5);
}

void ThresholdFilter::doFilter()
{
	cv::adaptiveThreshold(*mImage, *mImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 71, 15);
}