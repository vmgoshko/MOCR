#include "ImagePreProccessor.h"
#include "Filters.h"
#include "Defs.h"
#include <queue>

using namespace cv;
using namespace std;



static void showImage(const char* name, cv::Mat& img)
{
	namedWindow( name, CV_WINDOW_AUTOSIZE );
	imshow( name, img );
}

BlackObject ImagePreProccessor::extractObject( int i, int j )
{
	queue<Point2i> blackPixels;
	int w = img.cols;
	int h = img.rows;
	
	queue<Point2i> resultPixels;

	BlackObject res;
	res.object.setTo( 255 );
	res.minX = h;
	res.maxX = 0;
	res.minY = w;
	res.maxY = 0;
	
	blackPixels.push(Point2i(i,j));
	checkArray[i][j] = true;

	while( !blackPixels.empty() )
	{
		Point2i pt = blackPixels.front();
		resultPixels.push(pt);
		blackPixels.pop();
		
		checkArray[pt.x][pt.y] = true;

		for(int k = pt.x - 1; k <= pt.x + 1; k++)
			for(int l = pt.y - 1; l <= pt.y + 1; l++)
			{
				if( k < 0 || k >= h || l < 0 || l >= w )
					continue;

				if( k == pt.x && l == pt.y )
					continue;

				if( !img.at<char>( k, l ) && !checkArray[k][l] ) 
				{
					blackPixels.push( Point2i( k, l ) );
					checkArray[k][l] = true;
				}
			}

		MAX_ASSIGN( res.maxX , pt.x );
		MAX_ASSIGN( res.maxY , pt.y );
		MIN_ASSIGN( res.minX , pt.x );
		MIN_ASSIGN( res.minY , pt.y );
	}

	res.object = cv::Mat( res.maxX - res.minX + 1, res.maxY - res.minY + 1, /*img.type()*/ CV_8UC1 );
	res.object.setTo(cv::Scalar(255,255,255));
	
	while( !resultPixels.empty() )
	{
		Point2i px = resultPixels.front();
		resultPixels.pop();
		res.object.at<char>(px.x - res.minX, px.y - res.minY) = 0;
	}
		
	return res;
}

void ImagePreProccessor::findBlackObjects()
{
	int h = img.rows;
	int w = img.cols;

	for(int j = 1; j < w - 1; j++)
		for(int i = 1; i < h - 1; i++)
		{
			if (img.at< char >(i, j) == 0 && !checkArray[i][j])
			{
				BlackObject obj = extractObject( i, j );
				
				blackObjects.push_back( obj );	
			}
		}

}

cv::Mat ImagePreProccessor::process( const char* path, bool doFiltering )
{
	GaussianBlurFilter theGaussianBlur; 
	ThresholdFilter theThresholdFilter;
	
	if ( path )
		img = imread(path , IMREAD_GRAYSCALE);

	if( doFiltering )
	{
		theGaussianBlur.init( &img );
		theGaussianBlur.doFilter();
		theThresholdFilter.init( theGaussianBlur.getImage() );
		theThresholdFilter.doFilter();
		img = *theThresholdFilter.getImage();
	}
	
	for(int i = 0; i < img.rows; i++ )
	{
		checkArray.push_back( vector<bool>(img.cols, false) );
	}

	return img;
}

void ImagePreProccessor::reset()
{
	checkArray.clear();
	blackObjects.clear();
	img.setTo(0);
	sums.setTo(0);
}