#ifndef IMAGEPREPROCESSOR_H
#define IMAGEPREPROCESSOR_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <vector>
#include "Defs.h"

class ImagePreProccessor {
	private:
		BlackObject extractObject( int i, int j );
		void findBlackObjects();
		cv::Mat formula( cv::Mat& img );
		void createSums( cv::Mat &img );

	public:
		cv::Mat process( const char* path, bool doFiltering = true );

		inline std::vector<BlackObject> getBlackObjects()
		{
			findBlackObjects();
			return blackObjects;
		}

		void reset();

		void setImage( cv::Mat* m ) { this->img = *m; }
	private:
		std::vector<std::vector<bool>> checkArray;
		std::vector<BlackObject> blackObjects;
		cv::Mat img;
		cv::Mat sums;
};


#endif //IMAGEPREPROCESSOR_H