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

	void performTraining();
	void setOutput( std::vector<char*>* outs );
	virtual void addObject(BlackObject& obj, int inIndex) = 0;
	bool load();
	bool save(const char* name);
	const char* predict(BlackObject& object);
	std::vector< float > getPossibleChars(BlackObject& object);
protected:
	virtual cv::Mat createInput(BlackObject& obj) = 0;
private:
	void fillCriteriaAndParamsEps(CvTermCriteria& outCriteria, CvANN_MLP_TrainParams& outTrainParams);
	void fillCriteriaAndParamsIter(CvTermCriteria& outCriteria, CvANN_MLP_TrainParams& outTrainParams);
	void createNetwork();
	void createInputOutput(cv::Mat& inInput, cv::Mat& inOutput);
	
protected:
	std::vector<char*>* mOutputStrings;
	std::vector<std::vector<float>*> mObjects;
	std::vector<std::vector<float>*> mObjectOutputs;
private:
	const char* mcNetworkFile;
	CvANN_MLP mNetwork;
};

#endif //NEURAL_NET_TOOLS_H