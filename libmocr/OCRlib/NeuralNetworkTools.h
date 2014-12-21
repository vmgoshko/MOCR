#ifndef NEURAL_NET_TOOLS_H
#define NEURAL_NET_TOOLS_H
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include "Defs.h"
#include <opencv2\ml\ml.hpp>

class NeuralNetworkTools
{
public:
	NeuralNetworkTools();
	~NeuralNetworkTools();

	void performeTraining(bool inIsAppend);
	void setOutput( std::vector<char*>* outs );
	void addObject(BlackObject& obj, int outIndex, int height);
	const char* predict(BlackObject& object);
	std::vector< float > getPossibleChars(BlackObject& object);
	bool load();
	bool save(const char* name);
	void clear()
	{
		delete mOutputStrings;
		mObjects.clear();
		mObjectOutputs.clear();
	}

private:
	void createTrainInputs();

private:
	const int mcTrainSize;
	const char* mcNetworkFile;
	std::vector<char*>* mOutputStrings;
	std::vector<std::vector<float>*> mObjects;
	std::vector<std::vector<float>*> mObjectOutputs;
	CvANN_MLP mNetwork;
};

#endif //NEURAL_NET_TOOLS_H