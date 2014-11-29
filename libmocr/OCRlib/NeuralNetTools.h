#ifndef NEURAL_NET_TOOLS_H
#define NEURAL_NET_TOOLS_H
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include "Defs.h"
#include <opencv2\ml\ml.hpp>

class NeuralNetTools
{
public:
	NeuralNetTools();
	~NeuralNetTools();

	void performeTraining();
	void setOutput( std::vector<char*>* outs );
	void addObject( BlackObject& obj, int outIndex );
	const char* predict(BlackObject& object);
	bool load();
	bool save(const char* name);
private:
	void createTrainInputs();

private:
	const int TRAIN_SIZE;
	const char* NETWORK_FILE;
	std::vector<char*>* outChars;
	std::vector<std::vector<float>*> objects;
	std::vector<std::vector<float>*> objectOuts;
	CvANN_MLP net;
};

#endif //NEURAL_NET_TOOLS_H