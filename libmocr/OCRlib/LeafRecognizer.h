#ifndef LEAF_RECOGNIZER_H
#define LEAF_RECOGNIZER_H

#include <vector>
#include <string>

class ComponentsTree;
class LeafRecognizer 
{
	virtual std::vector< std::string > recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities = NULL) = 0;
};



#endif //LEAF_RECOGNIZER_H