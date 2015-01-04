#ifndef LEAFRECOGNIZER_H
#define LEAFRECOGNIZER_H

#include "Defs.h"
#include "LeafRecognizer.h"

class ComponentsTree;
class NeuralNetworkTools;

namespace cv
{
	class Mat;
};

class NeuralNetworkLeafRecognizer : public LeafRecognizer
{
public:
	NeuralNetworkLeafRecognizer();
	~NeuralNetworkLeafRecognizer();

	std::vector< std::string > recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities = NULL);
private:
	std::string recognizeLeaf( cv::Mat* leaf );
private:
	NeuralNetworkTools* tools;
};


#endif //LEAFRECOGNIZER_H