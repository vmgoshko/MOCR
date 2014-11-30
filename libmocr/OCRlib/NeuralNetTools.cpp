#include "NeuralNetTools.h"
#include "ImagePreProccessor.h"
#include "SkeletonBuilder.h"
#include <iostream>

using namespace std;
using namespace cv;

NeuralNetTools::NeuralNetTools(void) : 
	TRAIN_SIZE( 12 ), NETWORK_FILE("nn")
{
}


NeuralNetTools::~NeuralNetTools(void)
{
}


void NeuralNetTools::performeTraining( )
{
	Mat inputs( objects.size(), TRAIN_SIZE, CV_32F );
	Mat outputs( objectOuts.size(), outChars->size(), CV_32F );
	
	CvTermCriteria criteria;
//	criteria.max_iter = 1000;
	criteria.epsilon = 0.00001f;
	criteria.type = /*CV_TERMCRIT_ITER |*/ CV_TERMCRIT_EPS;

	CvANN_MLP_TrainParams params;
	params.train_method = CvANN_MLP_TrainParams::BACKPROP;
	params.bp_dw_scale = 0.1f;
	params.bp_moment_scale = 0.1f;
	params.term_crit = criteria;

	for (size_t i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < TRAIN_SIZE; j++)
		{
			float theCharacteristic = objects[i]->at(j);
			inputs.at<float>(i, j) = theCharacteristic;
		}
	}

	for (size_t i = 0; i < objectOuts.size(); i++)
	{
		for (size_t j = 0; j < outChars->size(); j++)
		{
			outputs.at<float>(i, j) = objectOuts[i]->at(j);
		}
	}

	Mat layers( 1, 3, CV_32S );
	layers.at<int>( 0, 0 ) = TRAIN_SIZE;
	layers.at<int>( 0, 1 ) = objects.size();
	layers.at<int>( 0, 2 ) = outChars->size();

	net.create(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1 );

	int theIterations = net.train( inputs, outputs, cv::Mat( ), cv::Mat( ), params );
	cout << "Training complete with " << theIterations << " iterations" << endl;
	net.save( NETWORK_FILE );
}

void invert( cv::Mat& inImage )
{
	for( int i = 0; i < inImage.rows; i++ )
		for( int j = 0; j < inImage.cols; j++ )
		{
			unsigned char newColor = 255 - inImage.at< unsigned char >( i, j );
			inImage.at< unsigned char >( i, j ) = newColor;
		}
}

void NeuralNetTools::addObject( BlackObject& obj, int outIndex )
{
	//create input
	invert( obj.object );
	SkeletonBuilder::skeleton( obj.object, obj.object );
	std::vector< float >* theCharacteristics = new std::vector< float >( SkeletonBuilder::calculateCharacteristic( obj.object, 1 ) );
	objects.push_back( theCharacteristics );

	//create output
	vector<float>* out = new vector<float>( outChars->size( ), -1 );
	out->at(outIndex) = 1;
	objectOuts.push_back(out);
}

void NeuralNetTools::setOutput( vector<char*>* outs )
{
	outChars = outs;
}

const char* NeuralNetTools::predict(BlackObject& obj)
{
	invert( obj.object );
	SkeletonBuilder::skeleton( obj.object, obj.object );
	std::vector< float >* theCharacteristics = new std::vector< float >( SkeletonBuilder::calculateCharacteristic( obj.object, 1 ) );

	Mat two( 1, TRAIN_SIZE, CV_32F );
	Mat predictOutput( 1, outChars->size(), CV_32F );
	predictOutput.setTo( 0 );
	
	for(int j = 0; j < TRAIN_SIZE; j++)
	{
		float theCharacteristic = (float)theCharacteristics->at(j);
		two.at<float>( 0, j ) = theCharacteristic;
	}

	net.predict(two, predictOutput);

	std::vector< float > theResult;
	for( int i = 0; i < predictOutput.cols; i++ )
	{	
		float theResultValue = predictOutput.at<float>( 0, i );
		theResult.push_back( theResultValue > 0 ? theResultValue : 0 );
	}

	int maxI = 0;
	for( int i = 0; i < predictOutput.cols; i++ )
		maxI = theResult[ i ] > theResult[ maxI ] ? i : maxI;

	if (maxI == 11)
		int a = 0;

	//cout << predictOutput << endl;
	return outChars->at(maxI);
}

bool NeuralNetTools::load()
{
	net.load( "../cvtest/nn" );
	return true;
}

bool NeuralNetTools::save(const char* name)
{
	net.save( name );
	return true;
}