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
#include "Filter.h"
#include "Utils.h"

void Filter::init( cv::Mat* image )
{
	this->mImage = image;
}

std::string Filter::saveAsTempFile()
{
	return saveTempFile( mImage );
}