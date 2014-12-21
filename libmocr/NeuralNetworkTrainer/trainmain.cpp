#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "Utils.h"
#include "Defs.h"

using namespace cv;
using namespace std;

void main()
{
	NeuralNetworkTools neuralNetTools;
	ImagePreProccessor preProcessor;

	cv::Mat image = preProcessor.process("img/arev_traindata_fixed.png");
	for (int j = 0; j < 3; j++)
	{
		vector< BlackObject > mObjects = preProcessor.getBlackObjects();
		neuralNetTools.setOutput(getOutsVector());
		for (size_t i = 0; i < mObjects.size(); i++)
		{
			neuralNetTools.addObject(mObjects[i], i, 50 + j * 50);
		}

		neuralNetTools.performeTraining( j != 0 );
		neuralNetTools.clear();
	}

	cvWaitKey();
	system( "pause" );
}