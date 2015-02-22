#ifndef TWO_STEPS_LEAF_RECGNIZER_H
#define TWO_STEPS_LEAF_RECGNIZER_H

#include <vector>

#include "ComponentsTree.h"
#include "LeafRecognizer.h"

//opencv includes
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class TwoStepsLeafRecognizer : public LeafRecognizer
{
public:
	virtual std::vector< std::string > recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities = NULL);
};

#endif // TWO_STEPS_LEAF_RECGNIZER_H