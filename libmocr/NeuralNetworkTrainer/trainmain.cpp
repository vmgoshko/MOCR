#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include "ImagePreProccessor.h"
#include "NeuralNetTools.h"
#include "Utils.h"

using namespace cv;
using namespace std;

void main()
{
	NeuralNetTools neuralNetTools;
	ImagePreProccessor preProcessor;
	vector<char*>* outChars = getOutsVector();

	cv::Mat image = preProcessor.process( "img/traindata.bmp" );
	vector< BlackObject > objects = preProcessor.getBlackObjects();
	
	neuralNetTools.setOutput( outChars );
	for( size_t i = 0 ; i < objects.size(); i++ )
	{
		neuralNetTools.addObject( objects[i], i /*/ (objects.size() / 1)*/ );
	}

	neuralNetTools.performeTraining();
	neuralNetTools.save("../cvtest/nn");

	cvWaitKey();
	delete outChars;
}