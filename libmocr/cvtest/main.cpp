#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <fstream>

#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "ComponentsTree.h"
#include "Utils.h"
#include "NeuralNetworkLeafRecognizer.h"
#include "TwoStepsLeafRecognizer.h"
#include "Structurer.h"
#include "SkeletonBuilder.h"
#include "Utils.h"


#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

namespace {
	const char* imagePath = "img/arev_2.png";
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



	std::string recognize()
	{
		ImagePreProccessor thePreProcessor;
		NeuralNetworkLeafRecognizer theRecognizer;
		Structurer theStructurer;
		ComponentsTree theTree;
		
		cv::Mat theProcessedImage = thePreProcessor.process( mImgPath );
		theTree.build(theProcessedImage);

		std::vector< std::string > theLeafsStrs = theRecognizer.recognizeLeafs(&theTree);
		//std::string theResult = theStructurer.structure(theProcessedImage, theTree.getLeafs(), theLeafsStrs);

		return "";
	}
};


void main()
{
	NativeRecognizer theNativeRecognizer(imagePath);
	std::cout << theNativeRecognizer.recognize() << std::endl;
	/*
	ImagePreProccessor thePreProcessor;
	NeuralNetTools theTools;
	theTools.load();
	theTools.setOutput( getOutsVector() );

	thePreProcessor.process( "img/4.bmp" );
	std::vector< BlackObject > theBlackObjects = thePreProcessor.getBlackObjects();
	
	for( int i = 0; i < theBlackObjects.size(); i++ )
	{
		std::cout << theTools.predict( theBlackObjects[ i ] ) << std::endl;
	}
	/*
	ImagePreProccessor thePreProcessor;
	cv::Mat image  = thePreProcessor.process("templ/10.jpg");
	scaleToHeight(image, 150);

	ThresholdFilter theFilter;
	theFilter.init(&image);
	theFilter.doFilter();
	SkeletonBuilder theSkeletonBuilder;
	invert(image);
	theSkeletonBuilder.skeleton(image, image);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++ )
		if (image.at<float>(i, j) != 0)
		{
			cycleSearch(image, i, j);
		}
	}
	int c = count;*/
	system("pause");
	cvWaitKey( 0 );
}