#include <fstream>
#include <vector>
#include <string>

#include "ImagePreProccessor.h"
#include "Utils.h"

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat vectorToMat(vector< vector< int > >& v)
{
	Mat theResult(v[0].size(), v.size(), CV_8UC1);
	theResult.setTo(1);
	for (int i = 0; i < v.size(); i++)
		for (int j = 0; j < v[0].size(); j++)
			theResult.at< unsigned char >(j, i) = v[i][j];

	return theResult;
}

struct Template
{
	int width;
	int height;
	vector< vector< int > > weights;
};

class TemplateBuilder
{
public:
	void createTemplate(cv::Mat& image, char* name)
	{
		BlackObject theObj = bound(&image, 0);
		scaleToHeight(theObj.object, Config::cNeuralNetworkImageHeight);
		Template theTemplate;
		theTemplate.height = theObj.object.rows;
		theTemplate.width = theObj.object.cols;

		vector< vector< int > >& theWeights = theTemplate.weights;

		vector< int > theColumn;
		theColumn.assign(theTemplate.height, 0);
		theWeights.assign(theTemplate.width, theColumn);
	
		for (int i = 0; i < theTemplate.width; i++)
			for (int j = 0; j < theTemplate.height; j++)
				theWeights[i][j] = 2*weight(theObj.object, j, i);
			
		string theFileName = "templates/";
		theFileName += name;
		
		ofstream out(theFileName.c_str());

		out << theTemplate.height << ' ' << theTemplate.height << endl;
		for (int i = 0; i < theWeights.size(); i++)
			for (int j = 0; j < theWeights[0].size(); j++)
			{
				out << theWeights[i][j] << ' ';
			}
		
		out.close();
	}

private:

	int weight(cv::Mat& image, int row, int col)
	{
		if (image.at<unsigned char>(row, col) == 0)
			return 1;

		int theWidth = image.cols;
		int theHeight = image.rows;
		int min = max(theHeight, theWidth) * max(theHeight, theWidth);

		for (int i = 0; i < theHeight; i++)
		for (int j = 0; j < theWidth; j++)
		{
			if (image.at< unsigned char >(i, j) == 0)
			{
				int distance = (i - row) * (i - row) + (j - col) * (j - col);

				if (distance < min)
					min = distance;
			}
		}

		return -(int)sqrt(min);
	}
};

void main()
{
	ImagePreProccessor preProcessor;

	Mat image = preProcessor.process("img/arev_traindata_fixed.png");
	vector< BlackObject > mObjects = preProcessor.getBlackObjects();
	vector< char* >* chars = getOutsVector();

	TemplateBuilder theTemplateBuilder;
	for (int i = 0; i < mObjects.size(); i++)
	{
		theTemplateBuilder.createTemplate(mObjects[i].object, chars->at(i));
		cout << "Template " << chars->at(i) << " created" << endl;
	}
	
	delete chars;
}