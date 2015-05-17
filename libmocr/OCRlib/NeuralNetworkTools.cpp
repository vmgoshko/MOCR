#include "NeuralNetworkTools.h"
#include "ImagePreProccessor.h"
#include "SkeletonBuilder.h"
#include "Utils.h"
#include <numeric>
#include <functional>
#include <iostream>

using namespace std;
using namespace cv;

extern Configuration gConfiguration;

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
	outCriteria.max_iter = 1000;
	outCriteria.type = CV_TERMCRIT_ITER;

	outTrainParams.train_method = CvANN_MLP_TrainParams::BACKPROP;
	outTrainParams.bp_dw_scale = 0.1f;
	outTrainParams.bp_moment_scale = 0.1f;
	outTrainParams.term_crit = outCriteria;
}

void NeuralNetworkTools::performTraining()
{
	Mat inputs(mObjects.size(), mObjects[0]->size(), CV_32F);
	Mat outputs( mObjectOutputs.size(), mOutputStrings->size(), CV_32F );
	
	CvTermCriteria criteria;
	CvANN_MLP_TrainParams params;
	
	fillCriteriaAndParamsEps(criteria, params);

	std::vector<size_t> index;

	for (size_t i = 0; i < mObjects.size(); i++)
		index.push_back(i);

	std::random_shuffle(index.begin(), index.end());
	std::random_shuffle(index.begin(), index.end());

	for (size_t i = 0; i < mObjects.size(); i++)
		std::copy(mObjects[index[i]]->begin(), mObjects[index[i]]->end(), inputs.ptr<float>(i));

	for (size_t i = 0; i < mObjectOutputs.size(); i++)
		std::copy(mObjectOutputs[index[i]]->begin(), mObjectOutputs[index[i]]->end(), outputs.ptr<float>(i));

	createNetwork();
	int theIterations = mNetwork.train(inputs, outputs, cv::Mat(), cv::Mat(), params);
	cout << "Training complete with " << theIterations << " iterations" << endl;
}

void showImage( const char* name, cv::Mat& img )
{
	cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
	cv::imshow( name, img );
}

int count(cv::Mat& mat, int i)
{
	int res = 0;

	for (int j = 0; j < mat.rows; j++)
	{
		if (mat.at<uchar>(j, i) > 0)
			res++;
	}

	return res;
}

cv::Mat* drawH(cv::Mat& obj)
{
	cv::Mat* res = new cv::Mat(obj.rows, obj.cols, CV_8UC1, 255);

	for (int i = 0; i < obj.cols; i++)
	{
		int countB = count(obj, i);

		for (int j = res->rows; j >= res->rows - countB; j--)
		{
			res->at<uchar>(j, i) = 0;
		}
	}
	return res;
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
	Mat input = createInput(obj);
	Mat predictOutput = Mat::zeros(1, mOutputStrings->size(), CV_32F);

	mNetwork.predict(input, predictOutput);

	float theOffset = 1.5f;
	float* thePredictOutPtr = predictOutput.ptr<float>(0);
	int thePredictOutSize = predictOutput.cols;

	std::vector< float > theResult(thePredictOutPtr, thePredictOutPtr + thePredictOutSize);
	transform(theResult.begin(), theResult.end(), theResult.begin(), std::bind2nd(std::plus<float>(), theOffset));
	float theAverage = std::accumulate(theResult.begin(), theResult.end(), 0.f) / theResult.size();
	for_each(theResult.begin(), theResult.end(), LessAverageToNullFunctor(theAverage));

	float theMaxValue = *max_element(theResult.begin(), theResult.end());
	
	if ( theMaxValue )
		transform(theResult.begin(), theResult.end(), theResult.begin(), std::bind2nd(std::divides<float>(), theMaxValue));

	return theResult;
}

void NeuralNetworkTools::createNetwork()
{
	Mat layers(1, 3, CV_32S);
	layers.at<int>(0, 0) = mObjects[0]->size();
	layers.at<int>(0, 1) = 2 * mObjects[0]->size();
	//layers.at<int>(0, 2) = mOutputStrings->size();
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
	mNetwork.load(gConfiguration.neuralNetworkFile);
	return true;
}

bool NeuralNetworkTools::save(const char* name)
{
	mNetwork.save( name );
	return true;
}