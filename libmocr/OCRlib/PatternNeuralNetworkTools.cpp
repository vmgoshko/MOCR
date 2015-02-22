#include "PatternNeuralNetworkTools.h"
#include "Utils.h"

void PatternNeuralNetworkTools::addObject(BlackObject& obj, int inIndex)
{
	// Create input
	std::vector< float >* theInputVector = createInputVector(obj);
	mObjects.push_back(theInputVector);

	// Create output
	std::vector<float>* out = new std::vector<float>(mOutputStrings->size(), -1);
	out->at(inIndex) = 1;
	mObjectOutputs.push_back(out);
}

cv::Mat PatternNeuralNetworkTools::createInput(BlackObject& obj)
{
	std::vector< float >* theInputVector = createInputVector(obj);
	cv::Mat input(1, theInputVector->size(), CV_32F);
	copy(theInputVector->begin(), theInputVector->end(), input.ptr<float>());
	delete theInputVector;

	return input;
}

std::vector< float >* PatternNeuralNetworkTools::createInputVector(BlackObject& obj)
{
	cv::Mat theObj = obj.object;
	theObj = bound(&theObj, 0).object;
	cv::resize(theObj, theObj, cv::Size(40, 40));
	std::vector< float >* theInputVector = new std::vector< float >(theObj.rows * theObj.cols);
	
	for (int i = 0; i < theObj.rows; i++)
	for (int j = 0; j < theObj.cols; j++)
	{
		theInputVector->at(i*theObj.cols + j) = theObj.at< uchar >(i, j);
	}
	return theInputVector;
}