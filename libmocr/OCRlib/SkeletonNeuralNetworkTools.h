#ifndef SKELETON_NEURAL_NETWORK_TOOLS_H
#define SKELETON_NEURAL_NETWORK_TOOLS_H

#include "NeuralNetworkTools.h"

extern Configuration gConfiguration;

class SkeletonNeuralNetworkTools : public NeuralNetworkTools
{
public:
	SkeletonNeuralNetworkTools()
	{
		type = gConfiguration.skeletonType;
	}

	void addObject(BlackObject& obj, int inIndex) override;
protected:
	cv::Mat createInput(BlackObject& obj) override;
private:
	std::vector< float >* createInputVector(BlackObject& obj);

private:
	SkeletonType type;
};

#endif //SKELETON_NEURAL_NETWORK_TOOLS_H