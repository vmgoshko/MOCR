#ifndef FILTERS_H
#define FILTERS_H

#include <string>

class Mat;

class Filter
{
public:
	void init(cv::Mat* image);
	virtual void doFilter() = 0;
	std::string saveAsTempFile();
	cv::Mat* getImage() { return mImage; };
protected:
	cv::Mat* mImage;
};

class GaussianBlurFilter : public Filter
{
public:
	virtual void doFilter();
};

class ThresholdFilter : public Filter
{
public:
	virtual void doFilter();
};

#endif //FILTERS_H