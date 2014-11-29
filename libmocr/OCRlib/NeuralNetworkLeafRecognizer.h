#ifndef LEAFRECOGNIZER_H
#define LEAFRECOGNIZER_H

#include "Defs.h"
#include "LeafRecognizer.h"

class ComponentsTree;
class NeuralNetTools;

namespace cv
{
	class Mat;
};

class NeuralNetworkLeafRecognizer : public LeafRecognizer
{
public:
	NeuralNetworkLeafRecognizer();
	~NeuralNetworkLeafRecognizer();

	std::vector< std::string > recognizeLeafs( ComponentsTree* tree );
private:
	std::string recognizeLeaf( cv::Mat* leaf );
private:
	NeuralNetTools* tools;
};


#endif //LEAFRECOGNIZER_H