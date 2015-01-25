#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include <vector>

#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "Utils.h"
#include "Defs.h"

namespace {
	const char* cTrainDataFilePath = "img/iwona_traindata_fixed.png";
	const char* cNeuralNetworkSavePath = "../cvtest/nn";
}

void main()
{
	NeuralNetworkTools neuralNetTools;
	ImagePreProccessor preProcessor;

	cv::Mat image = preProcessor.process(cTrainDataFilePath);
	
	std::vector< BlackObject > theBlackObjects = preProcessor.getBlackObjects();
	neuralNetTools.setOutput(getOutsVector());
	
	int theIndex = 0;
	for (auto theObject : theBlackObjects)
		neuralNetTools.addObject(theObject, theIndex++);

	neuralNetTools.performTraining();
	neuralNetTools.save(cNeuralNetworkSavePath);
	
	system( "pause" );
}