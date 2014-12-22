#include "Utils.h"

//std includes
#include <fstream>
#include <iostream>

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

// Should be optimized somehow
cv::Mat submat( cv::Mat m, Bound b )
{
	int w = b.r-b.l;
	int h = b.b-b.t;
	cv::Mat res( h, w, m.type() );

	for( int i = 0; i < h; i++ )
		for( int j = 0; j < w; j++ )
		if (res.type() == CV_8UC1)
			res.at<unsigned char>(i, j) = m.at<unsigned char>(i + b.t, j + b.l);
		else
			res.at<float>( i, j ) = m.at<float>( i + b.t, j + b.l );

	return res;
}

std::string saveTempFile( cv::Mat* image )
{
	static time_t theCurrTime = 1;
	size_t theCurrTimeLen = (long)floor( log10l( (long)theCurrTime++ ) ) + 5;
	char* theTempFileName = new char[ theCurrTimeLen ]; 
	char* jpgExt = ".bmp";
	_ltoa( (long)theCurrTime, theTempFileName, 10 );
	strcat( theTempFileName, jpgExt );
	cv::imwrite( theTempFileName, *image );
	std::string theResultFilePath( theTempFileName );
	//delete theTempFileName;
	return theResultFilePath;
}

BlackObject bound( cv::Mat* m, float inObjectColor )
{
	int h = m->rows;
	int w = m->cols;

	Bound b(-1,-1,-1,-1);
	
	// TOP
	int i = 0;
	int j = 0;
	bool isFound = false;

	while( i < h && !isFound )
	{
		j = 0;
		while( j < w && !isFound )
		{
			float px = -1;
			if (m->type() == CV_8UC1)
				px = m->at<unsigned char>(i, j);
			else if (m->type() == CV_32FC1)
				px = m->at<float>( i, j );

			if( px == inObjectColor && b.t == -1 )
			{
				b.t = i;
				isFound = true;
				break;
			}

			j++;
		}
		i++;
	}

	// BOTTOM
	i = h - 1;
	j = 0;
	isFound = false;

	while( i >= 0 && !isFound )
	{
		j = 0;
		while( j < w && !isFound )
		{
			float px = -1;
			if (m->type() == CV_8UC1)
				px = m->at<unsigned char>(i, j);
			else if (m->type() == CV_32FC1)
				px = m->at<float>(i, j);

			if( px == inObjectColor )
			{
				b.b = i;
				isFound = true;
				break;
			}

			j++;
		}
		i--;
	}
		
	// LEFT
	i = 0;
	j = 0;
	isFound = false;
	
	while( j < w && !isFound )
	{
		i = 0;
		while( i < h && !isFound )
		{
			float px = -1;
			if (m->type() == CV_8UC1)
				px = m->at<unsigned char>(i, j);
			else if (m->type() == CV_32FC1)
				px = m->at<float>(i, j);

			if( px == inObjectColor )
			{
				b.l = j;
				isFound = true;
				break;
			}

			i++;
		}
		j++;
	}

	// RIGHT
	i = 0;
	j = w - 1;
	isFound = false;

	while( j >= 0 && !isFound )
	{
		i = 0;
		while( i < h && !isFound )
		{
			float px = -1;
			if (m->type() == CV_8UC1)
				px = m->at<unsigned char>(i, j);
			else if (m->type() == CV_32FC1)
				px = m->at<float>(i, j);

			if( px == inObjectColor )
			{
				b.r = j;
				isFound = true;
				break;
			}

			i++;
		}
		j--;
	}

	if (b.b != h)
		b.b += 1;

	if (b.r != w)
		b.r += 1;

	BlackObject obj;
	obj.object = submat( *m, b );
	obj.maxX = b.b;
	obj.minX = b.t;
	obj.maxY = b.r;
	obj.minY = b.l;
	return obj;
}

std::vector<char*>* getOutsVector()
{
	std::vector< char* >* chars = new std::vector< char* >();
	
	chars->push_back( "1" );
	chars->push_back( "2" );
	chars->push_back( "3" );
	chars->push_back( "4" );
	chars->push_back( "5" );
	chars->push_back( "6" );
	chars->push_back( "7" );
	chars->push_back( "8" );
	chars->push_back( "9" );
	chars->push_back( "0" );
	chars->push_back( "(" );
	chars->push_back( ")" );

	chars->push_back( "+" );
	//chars->push_back( "-" );
	//chars->push_back( "*" );
	chars->push_back( "/" );
	
	chars->push_back( "a" );
	chars->push_back( "b" );
	chars->push_back( "c" );
	chars->push_back( "d" );
	chars->push_back( "e" );
	chars->push_back( "f" );
	chars->push_back( "g" );
	chars->push_back( "h" );
	chars->push_back( "i" );
	chars->push_back( "j" );
	chars->push_back( "k" );
	chars->push_back( "l" );
	chars->push_back( "m" );
	chars->push_back( "n" );
	chars->push_back( "o" );
	chars->push_back( "p" );
	chars->push_back( "q" );
	chars->push_back( "r" );
	chars->push_back( "s" );
	chars->push_back( "t" );
	chars->push_back( "u" );
	chars->push_back( "v" );
	chars->push_back( "w" );
	chars->push_back( "x" );
	chars->push_back( "y" );
	chars->push_back( "z" );
	
	chars->push_back( "sigma_big" );

	return chars;
}

void trim( std::string& str )
{
	std::string::size_type j = 0;
	while ( j < str.length() ) {
		j = str.find( '\n', j );
		if ( j == std::string::npos ) {
			break;
		}
		str.erase( j );
	}
}

void scaleToHeight(cv::Mat& inImage, int inHeight)
{
	float theScaleFactor = (float)inHeight / inImage.rows;

	cv::resize(inImage, inImage, cv::Size(ceil(inImage.cols * theScaleFactor), ceil(inImage.rows * theScaleFactor)));
}

void invert(cv::Mat& inImage)
{
	for (int i = 0; i < inImage.rows; i++)
	for (int j = 0; j < inImage.cols; j++)
	{
		unsigned char newColor = ~inImage.at< unsigned char >(i, j);
		inImage.at< unsigned char >(i, j) = newColor;
	}
}

LessAverageToNullFunctor::LessAverageToNullFunctor(float inAverage) 
	: average(inAverage)
{
};

void LessAverageToNullFunctor::operator()(float& element)
{
	if (element < average)
		element = 0;
}

AddForEach::AddForEach(float inOffset)
	: offset(inOffset)
{

}
void AddForEach::operator()(float& element)
{
	element += offset;
}
