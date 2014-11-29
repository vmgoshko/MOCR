/*
//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//tesseract includes
#include <tesseract\baseapi.h>
#include <leptonica\allheaders.h>

//OCRlib includes
#include "Utils.h"
#include "TesseractLeafRecognizer.h"
#include "ComponentsTree.h"
#include "NeuralNetTools.h"
#include "ImagePreProccessor.h"

namespace {
	const std::string cLanguage = "eng";
};

TesseractLeafRecognizer::TesseractLeafRecognizer()
{
	// Tesseract prediction
	mApi = new tesseract::TessBaseAPI();
	
	if( mApi->Init( NULL, cLanguage.c_str() ) )
		std::cout << "Error initializing tesseract" << std::endl;
	
	mApi->SetPageSegMode( static_cast<tesseract::PageSegMode>( tesseract::PSM_SINGLE_CHAR ) );
}

TesseractLeafRecognizer::TesseractLeafRecognizer( std::string imagePath )
{
	mApi = new tesseract::TessBaseAPI();
	
	mApi->SetVariable( "language_model_penalty_non_dict_word", "0" );

	if( mApi->Init( NULL, cLanguage.c_str(), tesseract::OEM_CUBE_ONLY ) )
		std::cout << "Error initializing tesseract" << std::endl;
	
	PIX* theImage = pixRead( imagePath.c_str() );
	mApi->SetImage( theImage );

	mApi->SetPageSegMode( static_cast<tesseract::PageSegMode>( tesseract::PSM_SINGLE_CHAR ) );
}

TesseractLeafRecognizer::~TesseractLeafRecognizer()
{
	// Tesseract prediction
	mApi->End();
}

std::vector<std::string> TesseractLeafRecognizer::recognizeLeafs( ComponentsTree* tree )
{
	std::vector< Component > theLeafs = tree->getLeafs();
	std::vector< std::string > theRes;

	for( size_t i = 0; i < theLeafs.size(); i++ )
	{
		std::string theString = recognizeLeaf( &submat( tree->getImage(), theLeafs[i].getBound() ) );
		
		std::string::size_type j = 0;
		while (j < theString.length()) {
			j = theString.find('\n', j);
			if (j == std::string::npos) {
				break;
			}
			theString.erase(j);
		}

		theRes.push_back( theString );
	}

	return theRes;
}

std::string TesseractLeafRecognizer::recognizeLeaf( cv::Mat* leaf )
{
	// Tesseract prediction
	cv::Mat tmp =  bound( leaf, 255 ).object;
	int newHeight = 100;
	double coeff = (double)newHeight / tmp.rows;

	cv::resize( tmp, tmp, cv::Size( tmp.cols * coeff, newHeight ) );
	std::string theTempFileName = saveTempFile( &tmp );
	return recognizeLeaf( theTempFileName );
}

std::string TesseractLeafRecognizer::recognizeLeaf( Bound b )
{
	mApi->SetRectangle( b.l, b.t, b.r - b.l, b.b - b.t );
	return std::string( mApi->GetUTF8Text() );
}

std::string TesseractLeafRecognizer::recognizeLeaf( std::string leafFile )
{
	const char* theImagePath = leafFile.c_str();
	PIX* theImage = pixRead( theImagePath );
	mApi->SetImage( theImage );
	return std::string( mApi->GetUTF8Text() );
}
*/