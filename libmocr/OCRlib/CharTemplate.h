#ifndef CHAR_TEMPLATE_H
#define CHAR_TEMPLATE_H

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include "Filters.h"

class CharTemplate
{
public:
	bool hasNoSimilarNeighbours( cv::Mat& m, int i, int j )
	{
		int h = m.rows;
		int w = m.cols;
		unsigned char curr = m.at<unsigned char>(i,j);

		for(int k = i - 1; k <= i + 1; k++)
			for(int l = j - 1; l <= j + 1; l++)
			{
				if( k < 0 || k >= h || l < 0 || l >= w )
					continue;

				if( k == i && l == j )
					continue;

				if( m.at<unsigned char>( k, l ) == curr )
					return false;
			}

		return true;
	}	

	void showImage(const char* name, cv::Mat& img)
	{
		cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
		cv::imshow( name, img );
	}

	float match( cv::Mat& templ, cv::Mat& img )
	{
		int h = templ.rows;
		int w = templ.cols;
		float matchedPx = 0;
		
		float scaleCoeff = (float)h / img.rows;
		int newH;
		int newW;

		if( scaleCoeff == 1 )
		{
			newH = img.rows;
			newW = templ.cols;
		} else {
			newH = img.rows * scaleCoeff;
			newW = img.cols * scaleCoeff;
		}

		cv::resize( img, img, cv::Size( newW, newH ) );
		
		for(int i = 0; i < h && i < img.rows; i++)
			for(int j = 0; j < w && j < img.cols; j++)
			{
				unsigned char templPx = templ.at<unsigned char>(i,j);
				unsigned char imgPx = img.at<unsigned char>(i,j);

				matchedPx += abs(templPx - imgPx) < 50 ? 2 : -2;
			}
		return matchedPx / abs(h*w); 
	}

};

#endif