#include "NeuralNetworkLeafRecognizer.h"

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "Utils.h"
#include "ComponentsTree.h"
#include "NeuralNetworkTools.h"
#include "ImagePreProccessor.h"

NeuralNetworkLeafRecognizer::NeuralNetworkLeafRecognizer()
{
	// Custom neural network prediction
	tools.setOutput( getOutsVector() );
	tools.load();
}

std::vector<std::string> NeuralNetworkLeafRecognizer::recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities)
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

	const char* theCharacter = tools.predict(theObject);
	return std::string(theCharacter);
}