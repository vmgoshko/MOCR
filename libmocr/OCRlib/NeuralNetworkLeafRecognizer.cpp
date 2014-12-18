#include "NeuralNetworkLeafRecognizer.h"

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "Utils.h"
#include "ComponentsTree.h"
#include "NeuralNetTools.h"
#include "ImagePreProccessor.h"

NeuralNetworkLeafRecognizer::NeuralNetworkLeafRecognizer()
{
	// Custom neural network prediction
	tools = new NeuralNetTools();
	tools->setOutput( getOutsVector() );
	tools->load();
}

NeuralNetworkLeafRecognizer::~NeuralNetworkLeafRecognizer()
{
	// Custom neural network prediction
	delete tools;
}

std::vector<std::string> NeuralNetworkLeafRecognizer::recognizeLeafs( ComponentsTree* tree )
{
	std::vector< Component > theLeafs = tree->getLeafs();
	std::vector< std::string > theRes;

	for( size_t i = 0; i < theLeafs.size(); i++ )
	{
		std::string theString = recognizeLeaf( &submat( tree->getImage(), theLeafs[i].getBound() ) );
		trim( theString );

		theRes.push_back( theString );
	}

	return theRes;
}

std::string NeuralNetworkLeafRecognizer::recognizeLeaf( cv::Mat* leaf )
{
	BlackObject theObject;
	theObject.object = *leaf;

	// Custom neural network prediction
	const char* pr = tools->predict(theObject);
	return std::string( pr );
}