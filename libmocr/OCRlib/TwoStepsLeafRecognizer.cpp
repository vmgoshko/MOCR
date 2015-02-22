#include "TwoStepsLeafRecognizer.h"

#include "Factories.h"
#include "Utils.h"
#include "TemplateTools.h"
#include "SkeletonNeuralNetworkTools.h"

std::vector< std::string > TwoStepsLeafRecognizer::recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities)
{
	std::vector< Component > theLeafs = tree->getLeafs();
	std::vector< std::string > theResult(theLeafs.size());

	NeuralNetworkTools* theNeuralNetworkTools = OCRFactories::CreateTools();
	theNeuralNetworkTools->setOutput(getOutsVector());
	theNeuralNetworkTools->load();

	TemplateTools theTemplateTools(getOutsVector());

	for (int i = 0; i < theLeafs.size(); i++)
	{
		cv::Mat theLeaf = submat(tree->getImage(), theLeafs[i].getBound());
		BlackObject theObject;
		theObject.object = theLeaf;
		std::vector< float > thePossibleChars = theNeuralNetworkTools->getPossibleChars(theObject);
		if (inPosibilities)
			inPosibilities->push_back(thePossibleChars);
		theObject.object = theLeaf;
		std::string theChar = theTemplateTools.recognizePossibilities(thePossibleChars, theObject);
		theResult[i] = theChar;
		//theResult[i] = theNeuralNetworkTools.predict(theObject);
	}

	return theResult;
}