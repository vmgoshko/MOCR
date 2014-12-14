#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <fstream>

#include "ImagePreProccessor.h"
#include "NeuralNetTools.h"
#include "ComponentsTree.h"
#include "Utils.h"
#include "PatternLeafRecognizer.h"
#include "Structurer.h"
#include "SkeletonBuilder.h"


#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

namespace {
	const char* imagePath = "img/10.bmp";
	const char* outPath = "result.txt";
};

namespace {
void showImage(const char* name, cv::Mat& img)
	{
		cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
		cv::imshow( name, img );
	}
};

class NativeRecognizer
{
private:
	const char* mImgPath;
public:
	NativeRecognizer( const char* inImagePath )
		: mImgPath( inImagePath ) {};

	void invert( cv::Mat& inImage )
	{
		for( int i = 0; i < inImage.rows; i++ )
			for( int j = 0; j < inImage.cols; j++ )
			{
				unsigned char newColor = 255 - inImage.at< unsigned char >( i, j );
				inImage.at< unsigned char >( i, j ) = newColor;
			}
	}

	std::string recognize()
	{
		ImagePreProccessor thePreProcessor;
		PatternLeafRecognizer theRecognizer;
		Structurer theStructurer;
		ComponentsTree theTree;
		
		cv::Mat theProcessedImage = thePreProcessor.process( "img/10.bmp" );
		invert( theProcessedImage );
		SkeletonBuilder::skeleton( theProcessedImage, theProcessedImage );
		std::vector< float > theCharacteristics = SkeletonBuilder::calculateCharacteristic( theProcessedImage, 1 );
		showImage( "1", theProcessedImage );
		saveTempFile( &theProcessedImage );
		theTree.setSourceImage( cv::imread( imagePath ) );
		theTree.build( theProcessedImage );
		theTree.getLeafs();


		return "";
	}
};

void main()
{
	ImagePreProccessor thePreProcessor;
	NeuralNetTools theTools;
	theTools.load();
	theTools.setOutput( getOutsVector() );

	thePreProcessor.process( "img/12.bmp" );
	std::vector< BlackObject > theBlackObjects = thePreProcessor.getBlackObjects();
	
	for( int i = 0; i < theBlackObjects.size(); i++ )
	{
		std::cout << theTools.predict( theBlackObjects[ i ] ) << std::endl;
	}
	system("pause");
	cvWaitKey( 0 );
}