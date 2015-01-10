#include <string>

#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "ComponentsTree.h"
#include "NeuralNetworkLeafRecognizer.h"
#include "TwoStepsLeafRecognizer.h"
#include "Structurer.h"
#include "SkeletonBuilder.h"
#include "Utils.h"

using namespace std;
const char* NativeRecognize(const char* inImagePath)
{
	ImagePreProccessor thePreProcessor;
	TwoStepsLeafRecognizer theRecognizer;
	Structurer theStructurer;
	ComponentsTree theTree;

	cv::Mat theProcessedImage = thePreProcessor.process(inImagePath);
	theTree.build(theProcessedImage);
	std::vector< std::string > theLeafsStrs = theRecognizer.recognizeLeafs(&theTree);
	//std::string theResult = theStructurer.structure(theProcessedImage, theTree.getLeafs(), theLeafsStrs);

	//return theResult.c_str();
	return "";
}