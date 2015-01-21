#ifndef UTILS_H
#define UTILS_H

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "Defs.h"

// Should be optimized somehow
cv::Mat submat( cv::Mat& m, Bound b );

std::string saveTempFile( cv::Mat* image );

BlackObject bound( cv::Mat* m, float inObjectColor );

std::vector<char*>* getOutsVector();

void trim( std::string& str );

void scaleToHeight(cv::Mat& inImage, int inHeight);

void invert(cv::Mat& inImage);

class LessAverageToNullFunctor
{
private:
	float average;
public:
	LessAverageToNullFunctor(float inAverage);
	void operator()(float& element);
};

#endif //UTILS_H