#ifndef TEMPLATE_TOOLS_H
#define TEMPLATE_TOOLS_H

#include <vector>
#include <fstream>
#include <string>
#include <functional>


//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include "Filters.h"
#include "ComponentsTree.h"
#include "Utils.h"

class TemplateTools
{
public:
	TemplateTools(std::vector< char* >* inOutStrings)
	{
		mOutStrings = inOutStrings;
	}

	std::string recognizePossibilities(std::vector< float >& inPossibleStrings, BlackObject& inObject)
	{
		inObject = bound(&inObject.object, 0);
		int theMaxMatchIndex = -1;
		int theMaxMatch = INT_MIN;

		
		float max = *max_element(inPossibleStrings.begin(), inPossibleStrings.end());
		if ( max != 0 )
			transform(inPossibleStrings.begin(), inPossibleStrings.end(), inPossibleStrings.begin(), std::bind2nd(std::divides<float>(), max));
		
		for (size_t i = 0; i < inPossibleStrings.size(); i++)
		{
			if (inPossibleStrings[i] != 0)
			{
				int theMatch = matchTemplate(mOutStrings->at(i), inObject);
				if (theMaxMatch < theMatch)
				{
					theMaxMatch = theMatch;
					theMaxMatchIndex = i;
				}
			}
		}

		return std::string(mOutStrings->at(theMaxMatchIndex));
	}

	int matchTemplate(char* inName, BlackObject& inObject)
	{
		int theMatch = 0;
		std::string theFileName = "templates/";
		if ( strcmp( inName, "/" ) != 0)
			theFileName += inName;
		else
			theFileName += "\\";

		std::ifstream in(theFileName.c_str());
		
		if (!in)
			return INT_MIN;

		int theHeight;
		int theWidth;


		in >> theHeight >> theWidth;
		std::vector< int > theColumn;
		theColumn.assign(theHeight, 0);
		std::vector< std::vector < int > > theWeights;
		theWeights.assign(theWidth, theColumn);
		
		cv::Mat resizedImage = inObject.object;
		//scaleToHeight(resizedImage, theHeight);
		cv::resize(resizedImage, resizedImage, cv::Size(theWidth, theHeight));
		ThresholdFilter theFilter;
		theFilter.init(&resizedImage);
		theFilter.doFilter();
		resizedImage = *theFilter.getImage();

		for (int i = 0; i < theWidth; i++)
			for (int j = 0; j < theHeight; j++)
			{
				in >> theWeights[i][j];
			}

		for (int i = 0; i < theWidth && i < resizedImage.cols; i++)
			for (int j = 0; j < theHeight && i < resizedImage.rows; j++)
					if (resizedImage.at<uchar>(j, i) == 0)
						theMatch += theWeights[i][j];

		cv::Mat img(theWeights[0].size(), theWeights.size(), CV_32FC1);
		for (int i = 0; i < theWidth; i++)
			for (int j = 0; j < theHeight && i < resizedImage.rows; j++)
				img.at<float>(j, i) = (float)theWeights[i][j];

		return theMatch;
	}
	
private:
	std::vector< char* >* mOutStrings;
};


#endif