#ifndef FILTER_H
#define FILTER_H

#include <string>

class Mat;

class Filter
{
public:
	void init( cv::Mat* image );
	virtual void doFilter() = 0;
	std::string saveAsTempFile();
	cv::Mat* getImage() { return mImage; };
protected:
	cv::Mat* mImage; 
};

#endif //FILTER_H