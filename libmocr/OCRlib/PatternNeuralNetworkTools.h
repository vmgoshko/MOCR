#ifndef PATTERN_NEURAL_NETWORK_H
#define PATTERN_NEURAL_NETWORK_H

#include "NeuralNetworkTools.h"

class PatternNeuralNetworkTools : public NeuralNetworkTools
{
public:
	void addObject(BlackObject& obj, int inIndex) override;
protected:
	cv::Mat createInput(BlackObject& obj) override;
private:
	std::vector< float >* createInputVector(BlackObject& obj);
};

#endif //PATTERN_NEURAL_NETWORK_H