#ifndef PATTER_LEAF_RECOGNIZER_H
#define PATTER_LEAF_RECOGNIZER_H

#include "CharTemplate.h"
#include "ComponentsTree.h"
#include "LeafRecognizer.h"
#include "Utils.h"

class PatternLeafRecognizer : public LeafRecognizer
{
public:
	PatternLeafRecognizer()
	{
		for( int i = 0; i < 41; i++ )
		{
			char buf[5];
			itoa( i + 3, buf, 10 );

			std::string templName;
			templName = "templ/";
			templName += buf;
			templName += ".jpg";
			cv::Mat templ = cv::imread( templName.c_str(), cv::IMREAD_GRAYSCALE );

			templates.push_back( templ );
		}
	}

std::vector< std::string > recognizeLeafs(ComponentsTree* tree, std::vector< std::vector< float > >* inPosibilities = NULL)
{
	std::vector< std::string > res;
	std::vector< Component > leafs = tree->getLeafs();
	std::vector<char*>* outs = getOutsVector();
	
	CharTemplate t;
	cv::Mat img = tree->getSourceImage();

	for(int i = 0; i < leafs.size(); i++ )
	{
		int maxI = -1;
		float max = -1000000000;

		BlackObject c = bound( &submat( img, leafs[i].getBound() ), 255 );
		
		for(int j = 0 ; j < 41; j++ )
		{
			cv::Mat templ = templates[j];
			float m = t.match( templ, c.object );
			if( m > max )
			{
				max = m;
				maxI = j;
			}
		}

		res.push_back( outs->at( maxI ) );
	}

	return res;
}
private:
	CharTemplate templateMatcher;
	std::vector< cv::Mat > templates;
};

#endif