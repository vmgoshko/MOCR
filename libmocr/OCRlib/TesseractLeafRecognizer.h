#ifndef LEAFRECOGNIZER_H
#define LEAFRECOGNIZER_H
/*
#include "Defs.h"
#include "LeafRecognizer.h"

class ComponentsTree;
class NeuralNetTools;
namespace cv
{
	class Mat;
};

namespace tesseract
{
	class TessBaseAPI;
};

class TesseractLeafRecognizer : public LeafRecognizer 
{
public:
	TesseractLeafRecognizer();
	TesseractLeafRecognizer( std::string imagePath );
	~TesseractLeafRecognizer();
	std::vector< std::string > recognizeLeafs( ComponentsTree* tree );
private:
	std::string recognizeLeaf( cv::Mat* leaf );
	std::string recognizeLeaf( std::string leafFile );
	std::string recognizeLeaf( Bound bound );
private:
	tesseract::TessBaseAPI* mApi;
	NeuralNetTools* tools;
};
*/

#endif //LEAFRECOGNIZER_H