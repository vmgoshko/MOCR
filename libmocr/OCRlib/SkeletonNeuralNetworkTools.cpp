#include "SkeletonNeuralNetworkTools.h"
#include "SkeletonBuilder.h"
#include "Utils.h"
#include "Factories.h"
extern Configuration gConfiguration;

void SkeletonNeuralNetworkTools::addObject(BlackObject& obj, int inIndex)
{
	// Create input
	std::vector< float >* theCharacteristics = createInputVector(obj);
	mObjects.push_back(theCharacteristics);

	// Create output
	std::vector<float>* out = new std::vector<float>(mOutputStrings->size(), -1);
	out->at(inIndex) = 1;
	mObjectOutputs.push_back(out);
}

cv::Mat SkeletonNeuralNetworkTools::createInput(BlackObject& obj)
{
	std::vector< float >* theCharacteristics = createInputVector(obj);
	cv::Mat input(1, theCharacteristics->size(), CV_32F);
	copy(theCharacteristics->begin(), theCharacteristics->end(), input.ptr<float>());
	delete theCharacteristics;
	
	return input;
}

std::vector< float >* SkeletonNeuralNetworkTools::createInputVector(BlackObject& obj)
{
	SkeletonBuilder* theSkeletonBuilder = OCRFactories::CreateSkeletonBuilder();
	obj = bound(&obj.object, 0);
	scaleToHeight(obj.object, Config::cNeuralNetworkImageHeight);
	copyMakeBorder(obj.object, obj.object, 1, 1, 1, 1, cv::BORDER_CONSTANT, 255);

	//create input
	invert(obj.object);
	theSkeletonBuilder->thinningGuoHall(obj.object);
	obj = bound(&obj.object, 255);
	std::vector< float >* theCharacteristics;

	theCharacteristics = new std::vector< float >(theSkeletonBuilder->calculateCharacteristic(obj.object, 255));

	delete theSkeletonBuilder;
	return theCharacteristics;
}