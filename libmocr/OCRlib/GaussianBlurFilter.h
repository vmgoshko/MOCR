#ifndef GAUSSIANBLURFILTER_H
#define GAUSSIANBLURFILTER_H

#include "Filter.h"

class GaussianBlurFilter : public Filter
{
public:
	virtual void doFilter();
};

#endif //GAUSSIANBLURFILTER_H