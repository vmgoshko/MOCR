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
	const char* cDataSetBasePath = "../traindata";
	const int cFontsCount = 3;
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

void main()
{
	NeuralNetworkTools neuralNetTools;
	ImagePreProccessor preProcessor;

	neuralNetTools.setOutput(getOutsVector());
	auto chars = getOutsVector();
	for (int i = 0; i < cFontsCount; i++)
	{
		for (int j = 0; j < chars->size(); j++)
		{
			std::string path = makePath(chars->at(j), i);
			
			cv::Mat image = preProcessor.process(path.c_str());

			std::vector< BlackObject > theBlackObjects = preProcessor.getBlackObjects();

			for (auto theObject : theBlackObjects)
				neuralNetTools.addObject(theObject, j);

			if (theBlackObjects.size() == 0)
				int a = 0;

			preProcessor.reset();
		}
	}

	neuralNetTools.performTraining();
	neuralNetTools.save(cNeuralNetworkSavePath);

	system("pause");
}