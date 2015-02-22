#include "stdafx.h"
#include "FullTest.h"
#include "Timer.h"
#include "ocr.h"

#include <fstream>
using namespace std;

void FullTest::InitTestData(std::string inTestDataFilePath)
{
	ifstream configFile(inTestDataFilePath);
	std::string line;

	while (std::getline(configFile, line))
	{
		int first;
		int second;
		int third;
		int fourth;
		first = line.find('\"');
		second = line.find('\"', first + 1);
		third = line.find('\"', second + 1);
		fourth = line.find('\"', third + 1);

		mTestData.emplace_back(line.substr(first + 1, second - first - 1), line.substr(third + 1, fourth - third - 1));
	}

	mMetrics.imagesCount = mTestData.size();
}

void FullTest::RunTest()
{
	Timer timer;
	printf("Full test started with %d items.\n", mTestData.size());
	int count = 0;

	for (auto data : mTestData)
	{
		count++;
		printf("Recognition started. %d of %d.\n", count, mMetrics.imagesCount);
		
		timer.Start();
		std::string result = Recognize(data.imagePath.c_str());
		mMetrics.overallTime += timer.Stop();
	}

	mMetrics.averageTime = mMetrics.overallTime / mMetrics.imagesCount;
	printf("Full test finished.\n\n");
}

FullTestMetrics FullTest::GetMetrics()
{
	return mMetrics;
}