#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include <vector>

#include "ImagePreProccessor.h"
#include "SkeletonNeuralNetworkTools.h"
#include "PatternNeuralNetworkTools.h"
#include "Utils.h"
#include "Defs.h"
#include "Factories.h"

Configuration gConfiguration;

namespace {
	const char* cTrainDataFilePath = "img/iwona_traindata_fixed.png";
	const char* cNeuralNetworkSavePath = "../cvtest/nn";
	const char* cDataSetBasePath = "../traindata";
	const int cFontsCount = 1;
}

std::string makePath(std::string inChar, int inID)
{
	std::string thePath = cDataSetBasePath;
	thePath += "/";

	if (strcmp(inChar.c_str(), "/") == 0)
		thePath += "slash";
	else if (strcmp(inChar.c_str(), "*") == 0)
		thePath += "asterisk";
	else
		thePath += inChar;
	thePath += "/";

	char buff[10];
	char filename[] = "%i.png";
	sprintf(buff, filename, inID);

	thePath += buff;
	return thePath;
}

void train(NeuralNetworkTools* neuralNetTools)
{
	ImagePreProccessor preProcessor;
	auto chars = getOutsVector();
	int theIterationsCount = cFontsCount * chars->size();
	int theIterationsComplete = 0;
	 
	std::cout << "Adding training examples started..." << std::endl;
	for (int i = 0; i < cFontsCount; i++)
	{
		for (int j = 0; j < chars->size(); j++)
		{
			std::string path = makePath(chars->at(j), i);

			cv::Mat image = preProcessor.process(path.c_str());

			std::vector< BlackObject > theBlackObjects = preProcessor.getBlackObjects();

			for (auto theObject : theBlackObjects)
				neuralNetTools->addObject(theObject, j);

			//if ( theIterationsComplete % 5 == 0)
			std::cout << int(theIterationsComplete / (float)theIterationsCount * 100) << "% complete." << std::endl;
			preProcessor.reset();

			theIterationsComplete++;
		}
	}

	std::cout << "Adding training examples finished..." << std::endl;
	std::cout << "Training started..." << std::endl;
	neuralNetTools->performTraining();
	std::cout << "Training finished..." << std::endl;
	std::cout << "Saving neural network file started..." << std::endl;
	neuralNetTools->save(gConfiguration.neuralNetworkFile);
	std::cout << "Saving neural network file finished..." << std::endl;
	std::cout << "Training fininshed." << std::endl;

}


void main()
{
	Configuration configurations[] =
	{
		//{ "../nn_pattern", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Pattern, SkeletonType::None },
		//{ "../nn_skeleton_raster", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Raster },
		{ "../nn_skeleton_vector", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Vector },
		//{ "../nn_skeleton_raster_vector", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Raster_Vector },
	};

	for (auto config : configurations)
	{
		gConfiguration = config;

		NeuralNetworkTools* neuralNetTools = OCRFactories::CreateTools();
		if (neuralNetTools == nullptr)
			std::cout << "error nullptr" << std::endl;
		train(neuralNetTools);
		delete neuralNetTools;
	}
	system("pause");
}