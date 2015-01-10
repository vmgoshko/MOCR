#include "NeuralNetworkTools.h"
#include "ImagePreProccessor.h"
#include "SkeletonBuilder.h"
#include "Utils.h"
#include <numeric>
#include <functional>
#include <iostream>

using namespace std;
using namespace cv;

NeuralNetworkTools::NeuralNetworkTools(void) 
	: mcNetworkFile("nn")
{
}


NeuralNetworkTools::~NeuralNetworkTools(void)
{
	for (int i = 0; i < mObjects.size(); i++)
		delete mObjects[i];
	
	for (int i = 0; i < mObjectOutputs.size(); i++)
		delete mObjectOutputs[i];
}

void NeuralNetworkTools::fillCriteriaAndParamsEps(CvTermCriteria& outCriteria, CvANN_MLP_TrainParams& outTrainParams)
{
	outCriteria.epsilon = 0.00001f;
	outCriteria.type = CV_TERMCRIT_EPS;

	outTrainParams.train_method = CvANN_MLP_TrainParams::BACKPROP;
	outTrainParams.bp_dw_scale = 0.1f;
	outTrainParams.bp_moment_scale = 0.1f;
	outTrainParams.term_crit = outCriteria;
}

void NeuralNetworkTools::fillCriteriaAndParamsIter(CvTermCriteria& outCriteria, CvANN_MLP_TrainParams& outTrainParams)
{
	outCriteria.max_iter = 1;
	outCriteria.type = CV_TERMCRIT_ITER /*| CV_TERMCRIT_EPS*/;

	outTrainParams.train_method = CvANN_MLP_TrainParams::BACKPROP;
	outTrainParams.bp_dw_scale = 0.1f;
	outTrainParams.bp_moment_scale = 0.1f;
	outTrainParams.term_crit = outCriteria;
}
void NeuralNetworkTools::performeTraining()
{
	Mat inputs(mObjects.size(), mObjects[0]->size(), CV_32F);
	Mat outputs( mObjectOutputs.size(), mOutputStrings->size(), CV_32F );
	
	CvTermCriteria criteria;
	CvANN_MLP_TrainParams params;
	
	fillCriteriaAndParamsEps(criteria, params);

	for (size_t i = 0; i < mObjects.size(); i++)
		std::copy(mObjects[i]->begin(), mObjects[i]->end(), inputs.ptr<float>(i));

	for (size_t i = 0; i < mObjectOutputs.size(); i++)
		std::copy(mObjectOutputs[i]->begin(), mObjectOutputs[i]->end(), outputs.ptr<float>(i));

	createNetwork();
	int theIterations = mNetwork.train(inputs, outputs, cv::Mat(), cv::Mat(), params);
	cout << "Training complete with " << theIterations << " iterations" << endl;
}

void showImage( const char* name, cv::Mat& img )
{
	cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
	cv::imshow( name, img );
}

void NeuralNetworkTools::addObject( BlackObject& obj, int outIndex )
{
	SkeletonBuilder theSkeletonBuilder;
	obj = bound(&obj.object, 0);
	scaleToHeight(obj.object, Config::cNeuralNetworkImageHeight);
	copyMakeBorder(obj.object, obj.object, 1, 1, 1, 1, BORDER_CONSTANT, 255);
	
	//create input
	invert( obj.object );
	theSkeletonBuilder.thinningGuoHall(obj.object);
	obj = bound(&obj.object, 255);
	theSkeletonBuilder.calculateCharacteristic(obj.object, 255);
	std::vector< float >* theCharacteristics = new std::vector< float >(theSkeletonBuilder.calculateCharacteristicVectorize(obj.object, 255));
	mObjects.push_back( theCharacteristics );

	//create output
	vector<float>* out = new vector<float>( mOutputStrings->size( ), -1 );
	out->at(outIndex) = 1;
	mObjectOutputs.push_back(out);
}

void NeuralNetworkTools::setOutput( vector<char*>* outs )
{
	mOutputStrings = outs;
}

const char* NeuralNetworkTools::predict(BlackObject& obj)
{
	std::vector< float > theResult = getPossibleChars(obj);
	int theMaxIndex = distance(theResult.begin(), max_element(theResult.begin(), theResult.end()));
	return mOutputStrings->at(theMaxIndex);
}

std::vector< float > NeuralNetworkTools::getPossibleChars(BlackObject& obj)
{
	SkeletonBuilder theSkeletonBuilder;

	obj = bound(&obj.object, 0);
	copyMakeBorder(obj.object, obj.object, 1, 1, 1, 1, BORDER_CONSTANT, 255);
	scaleToHeight(obj.object, Config::cNeuralNetworkImageHeight);

	invert(obj.object);
	theSkeletonBuilder.thinningGuoHall(obj.object);
	obj = bound(&obj.object, 255);
	std::vector< float > theCharacteristics = theSkeletonBuilder.calculateCharacteristicVectorize(obj.object, 255);

	Mat input(1, theCharacteristics.size(), CV_32F);
	Mat predictOutput = Mat::zeros(1, mOutputStrings->size(), CV_32F);

	copy(theCharacteristics.begin(), theCharacteristics.end(), input.ptr<float>());

	mNetwork.predict(input, predictOutput);

	float theOffset = 1.5f;
	float* thePredictOutPtr = predictOutput.ptr<float>(0);
	int thePredictOutSize = predictOutput.cols;

	std::vector< float > theResult(thePredictOutPtr, thePredictOutPtr + thePredictOutSize);
	transform(theResult.begin(), theResult.end(), theResult.begin(), std::bind2nd(std::plus<float>(), theOffset));
	float average = std::accumulate(theResult.begin(), theResult.end(), 0.f) / theResult.size();
	for_each(theResult.begin(), theResult.end(), LessAverageToNullFunctor(average));

	float theMaxValue = *max_element(theResult.begin(), theResult.end());
	transform(theResult.begin(), theResult.end(), theResult.begin(), std::bind2nd(std::divides<float>(), theMaxValue));

	return theResult;
}

void NeuralNetworkTools::createNetwork()
{
	Mat layers(1, 3, CV_32S);
	layers.at<int>(0, 0) = mObjects[0]->size();
	layers.at<int>(0, 1) = mObjects.size();
	layers.at<int>(0, 2) = mOutputStrings->size();

	mNetwork.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);
}

void NeuralNetworkTools::createInputOutput(cv::Mat& inInput, cv::Mat& inOutput)
{
	for (size_t i = 0; i < mObjects.size(); i++)
		std::copy(mObjects[i]->begin(), mObjects[i]->end(), inInput.ptr<float>(i));

	for (size_t i = 0; i < mObjectOutputs.size(); i++)
		std::copy(mObjectOutputs[i]->begin(), mObjectOutputs[i]->end(), inOutput.ptr<float>(i));
}

bool NeuralNetworkTools::load()
{
	mNetwork.load(mcNetworkFile);
	return true;
}

bool NeuralNetworkTools::save(const char* name)
{
	mNetwork.save( name );
	return true;
}