#ifndef TWO_STEPS_LEAF_RECGNIZER_H
#define TWO_STEPS_LEAF_RECGNIZER_H
#include <vector>

#include "LeafRecognizer.h"
#include "NeuralNetworkTools.h"
#include "TemplateTools.h"
#include "ComponentsTree.h"
#include "Utils.h"

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

class TwoStepsLeafRecognizer : public LeafRecognizer
{
public:
	virtual std::vector< std::string > recognizeLeafs(ComponentsTree* tree)
	{
		std::vector< Component > theLeafs = tree->getLeafs();
		std::vector< std::string > theResult( theLeafs.size() );
		
		NeuralNetworkTools theNeuralNetworkTools;
		theNeuralNetworkTools.setOutput(getOutsVector());
		theNeuralNetworkTools.load();

		TemplateTools theTemplateTools(getOutsVector());

		for (int i = 0; i < theLeafs.size(); i++)
		{
			cv::Mat theLeaf = submat(tree->getImage(), theLeafs[i].getBound());
			BlackObject theObject;
			theObject.object = theLeaf;
			std::vector< float > thePossibleChars = theNeuralNetworkTools.getPossibleChars(theObject);

			theObject.object = theLeaf;
			std::string theChar = theTemplateTools.recognizePossibilities(thePossibleChars, theObject);
			theResult[i] = theChar;
		}
	
		return theResult;
	}
};

#endif // TWO_STEPS_LEAF_RECGNIZER_H
