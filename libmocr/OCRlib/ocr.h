#ifndef OCR_H
#define OCR_H 

#include "Utils.h"
#include "Factories.h"
#include "Structurer.h"
#include "ComponentsTree.h"
#include "SkeletonBuilder.h"
#include "ImagePreProccessor.h"
#include "NeuralNetworkTools.h"
#include "TwoStepsLeafRecognizer.h"
#include "PatternNeuralNetworkTools.h"
#include "SkeletonNeuralNetworkTools.h"
#include "NeuralNetworkLeafRecognizer.h"

std::string Recognize(const char* inImagePath)
{
	ImagePreProccessor thePreProcessor;
	LeafRecognizer* theRecognizer;
	Structurer theStructurer;
	ComponentsTree theTree;

	theRecognizer = OCRFactories::CreateRecognizer();

	cv::Mat theProcessedImage = thePreProcessor.process(inImagePath);
	theTree.build(theProcessedImage);
	std::vector< std::string > theLeafsStrs = theRecognizer->recognizeLeafs(&theTree);
	std::string theResult = theStructurer.structure(theProcessedImage, theTree.getLeafs(), theLeafsStrs);

	delete theRecognizer;
	return theResult;
	//return "";
}

#endif