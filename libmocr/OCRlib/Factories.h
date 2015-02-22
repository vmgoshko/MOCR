#ifndef FACTORIES_H
#define FACTORIES_H

#include "Utils.h"
#include "NeuralNetworkTools.h"
#include "TwoStepsLeafRecognizer.h"
#include "PatternNeuralNetworkTools.h"
#include "SkeletonNeuralNetworkTools.h"
#include "NeuralNetworkLeafRecognizer.h"
#include "SkeletonBuilder.h"

extern Configuration gConfiguration;

namespace OCRFactories
{
	static NeuralNetworkTools* CreateTools()
	{
		NeuralNetworkTools* neuralNetTools = nullptr;

		switch (gConfiguration.nnToolsType)
		{
			case NNToolsType::Pattern:
				neuralNetTools = new PatternNeuralNetworkTools();
				break;
			case NNToolsType::Skeleton:
				neuralNetTools = new SkeletonNeuralNetworkTools();
				break;
		}

		if (neuralNetTools)
		{
			neuralNetTools->setOutput(getOutsVector());
			neuralNetTools->load();
		}
		return neuralNetTools;
	}

	static LeafRecognizer* CreateRecognizer()
	{
		switch (gConfiguration.leafRecognizerType)
		{
			case LeafRecognizerType::NeuralNetworkRecognizer:
				return new NeuralNetworkLeafRecognizer();
			case LeafRecognizerType::TwoStepsRecognizer:
				return new TwoStepsLeafRecognizer();
			default:
				return nullptr;
		}
	}

	static SkeletonBuilder* CreateSkeletonBuilder()
	{
		switch (gConfiguration.skeletonType)
		{
		case SkeletonType::Raster:
			return new RasterSkeletonBuilder();
		case SkeletonType::Vector:
			return new VectorSkeletonBuilder();
		case SkeletonType::Raster_Vector:
			return new CombinedSkeletonBuilder();
		default:
			return nullptr;
		}
	}
}

#endif //FACTORIES_H