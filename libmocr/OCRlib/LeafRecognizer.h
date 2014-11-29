#ifndef LEAF_RECOGNIZER_H
#define LEAF_RECOGNIZER_H

#include <vector>
#include <string>

class ComponentsTree;
class LeafRecognizer 
{
	virtual std::vector< std::string > recognizeLeafs( ComponentsTree* tree ) = 0;
};



#endif //LEAF_RECOGNIZER_H