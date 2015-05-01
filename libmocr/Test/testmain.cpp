#include "stdafx.h"
//#include "FullTest.h"
#include "LeafRecognitionTest.h"
#include <vector>

Configuration gConfiguration;

void main()
{
	
	Configuration configurations[] = 
	{
	//	{ "../nn_pattern", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Pattern, SkeletonType::None },
		{ "../nn_skeleton_raster", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Skeleton, SkeletonType::Raster },
		{ "../nn_skeleton_vector", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Skeleton, SkeletonType::Vector },
		{ "../nn_skeleton_raster_vector", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Skeleton, SkeletonType::Raster_Vector },
		
		{ "../nn_pattern", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Pattern, SkeletonType::None },
		{ "../nn_skeleton_raster", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Raster },
		{ "../nn_skeleton_vector", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Vector },
		{ "../nn_skeleton_raster_vector", LeafRecognizerType::NeuralNetworkRecognizer, NNToolsType::Skeleton, SkeletonType::Raster_Vector }
	};

	std::vector< LeafRecognitionTestMetrics > metrics;
	for (auto config : configurations)
	{
		gConfiguration = config;
		LeafRecognitionTest test;
		test.InitTestData("resources/LeafRecognitionConfig.cfg");
		test.RunTest();
		metrics.push_back(test.GetMetrics());
		test.PrintResult();
	}

	system("pause");
}