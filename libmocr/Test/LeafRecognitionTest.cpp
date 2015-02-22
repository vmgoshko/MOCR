#include "stdafx.h"
#include "Timer.h"
#include "Utils.h"
#include "Structurer.h"
#include "ComponentsTree.h"
#include "SkeletonBuilder.h"
#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "LeafRecognitionTest.h"
#include "TwoStepsLeafRecognizer.h"
#include "NeuralNetworkLeafRecognizer.h"

#include <fstream>
#include <sstream>

using namespace std;

extern Configuration gConfiguration;

void LeafRecognitionTest::InitTestData(std::string inTestDataFilePath)
{
	ifstream configFile(inTestDataFilePath);
	std::string line;

	bool isHasBegin = false;
	bool isHasEnd = false;

	while (std::getline(configFile, line))
	{
		if (line.empty())
			continue;

		if (line == "@begin")
		{
			isHasBegin = true;
			continue;
		}

		if (line == "@end")
		{
			isHasEnd = true;
			continue;
		}

		if (!isHasBegin || isHasEnd)
			continue;

		int first;
		int second;
		int third;
		int fourth;
		first = line.find('\"');
		second = line.find('\"', first + 1);
		third = line.find('\"', second + 1);
		fourth = line.find('\"', third + 1);

		LeafRecognitionTestData data("");
		data.imagePath = line.substr(first + 1, second - first - 1);

		std::string str = line.substr(third + 1, fourth - third - 1);
		std::stringstream expected(str);
		std::istream_iterator<std::string> begin(expected);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vstrings(begin, end);
		data.expectedChars = vstrings;

		mTestData.push_back(data);
	}

	mMetrics.charactersCount = mTestData.size();
}

void LeafRecognitionTest::RunTest()
{
	ImagePreProccessor thePreProcessor;
	LeafRecognizer* theRecognizer;
	ComponentsTree theTree;

	mType = gConfiguration.leafRecognizerType;
	if (mType == LeafRecognizerType::TwoStepsRecognizer)
		theRecognizer = new TwoStepsLeafRecognizer();
	else if (mType == LeafRecognizerType::NeuralNetworkRecognizer)
		theRecognizer = new NeuralNetworkLeafRecognizer();
	else
		return;

	int count = 0;
	Timer timer;
	printf("Recognition started (%d formulas): ", mTestData.size() );

	for (auto data : mTestData)
	{
		count++;
		printf("%d ", count);

		cv::Mat theProcessedImage = thePreProcessor.process(data.imagePath.c_str());
		theTree.build(theProcessedImage);

		timer.Start();
		std::vector< std::string > theLeafsStrs = theRecognizer->recognizeLeafs(&theTree);
		mMetrics.charactersCount += theLeafsStrs.size();

		for (int i = 0; i < theLeafsStrs.size(); i++)
		{
			if (theLeafsStrs[i] == data.expectedChars[i])
				mMetrics.rightOCRCount++;
		}

		mMetrics.fullRecognitionTime += timer.Stop();
		thePreProcessor.reset();
	}

	printf("\n");
	mMetrics.rightOCRPercent = mMetrics.rightOCRCount / (float)mMetrics.charactersCount * 100;
	mMetrics.averageRecognitionTime = mMetrics.fullRecognitionTime / (float)mMetrics.charactersCount;

	delete theRecognizer;
}

LeafRecognitionTestMetrics LeafRecognitionTest::GetMetrics()
{
	return LeafRecognitionTestMetrics();
}

void LeafRecognitionTest::PrintResult()
{
	char* recognizerName;
	if (mType == LeafRecognizerType::NeuralNetworkRecognizer)
		recognizerName = "Neural network leaf recognizer";
	else if (mType == LeafRecognizerType::TwoStepsRecognizer)
		recognizerName = "Two steps leaf recognizer";
	else
		recognizerName = "Unknown recognizer";

	char* nntype;
	if (gConfiguration.nnToolsType == NNToolsType::Pattern)
		nntype = "Pattern neural network";
	else if (gConfiguration.nnToolsType == NNToolsType::Skeleton)
		nntype = "Skeleton neural network";
	else
		nntype = "Unknown neural network";

	char* skeletonType = "";
	if (gConfiguration.nnToolsType == NNToolsType::Skeleton)
	{
		if (gConfiguration.skeletonType == SkeletonType::Raster)
			skeletonType = "Raster skeleton";
		else if (gConfiguration.skeletonType == SkeletonType::Vector)
			skeletonType = "Vector skeleton";
		else if (gConfiguration.skeletonType == SkeletonType::Raster_Vector)
			skeletonType = "Combined skeleton";
	}

	printf("------------------------------------------------------\n");
	printf("Recognitions results using %s, %s, %s\n", recognizerName, nntype, skeletonType);
	printf("\tCharacters count: %d\n", mMetrics.charactersCount);
	printf("\tRightly recognized characters count: %d\n", mMetrics.rightOCRCount);
	printf("\tRightly recognized percent: %3.3f\n", mMetrics.rightOCRPercent);
	printf("\tFull test time: %5.5f secs\n", mMetrics.fullRecognitionTime);
	printf("\tAverage time for each character: %5.5f secs\n", mMetrics.averageRecognitionTime);
	printf("------------------------------------------------------\n");
}