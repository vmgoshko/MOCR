#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "ImagePreProccessor.h"
#include "Utils.h"

using namespace std;
using namespace cv;

const string gBaseFolder = "../images/base_fonts_78px/";
const string gOutFolder = "../traindata/";

const string gFileNames[] = { 
								"iwona.png",
								"anttor.png",
								"arev.png",
								"ccfonts.png",
								"cmbright.png",
								"concmath.png",
								"fourier.png",
								"mathdesign.png",
								"mathpazo.png",
								"mathpple.png",
								"mathptmx.png",
								"modern.png",
								"pxfonts.png",
								"txfonts.png"
							}; // 14

const size_t gFileNamesCount = 14;

namespace {
	void showImage(const char* name, cv::Mat& img)
	{
		cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
		cv::imshow(name, img);
	}

	void makeCorrectSize(cv::Mat* obj)
	{
		int addHeight = 90 - obj->rows;
		int addWidth = 90 - obj->cols;

		int t;
		int b;
		int l;
		int r;
		
		if (addHeight % 2 == 0)
			t = b = addHeight / 2;
		else
		{
			t = addHeight / 2;
			b = addHeight / 2 + 1;
		}

		if (addWidth % 2 == 0)
			l = r = addWidth / 2;
		else
		{
			l = addWidth / 2;
			r = addWidth / 2 + 1;
		}

		copyMakeBorder(*obj, *obj, t, b, l, r, BORDER_CONSTANT, 255);
	}
};

void main()
{
	ImagePreProccessor preProcessor;
	vector<char*>* outChars = getOutsVector();

	for (int i = 0; i < outChars->size(); i++)
	{
		string theSavePath = gOutFolder;

		if (strcmp(outChars->at(i), "/") == 0)
			theSavePath += "slash";
		else if (strcmp(outChars->at(i), "*") == 0)
			theSavePath += "asterisk";
		else
			theSavePath += outChars->at(i);

		string mkpath = "mkdir \"";
		mkpath += theSavePath;
		mkpath += "\"";
		system(mkpath.c_str());
	}

	for (int i = 0; i < gFileNamesCount; i++)
	{
		cv::Mat theProcessedImage = preProcessor.process((gBaseFolder + gFileNames[i]).c_str());
		std::vector< BlackObject > theBlackObjects = preProcessor.getBlackObjects();
		
		for (int j = 0; j < theBlackObjects.size(); j++)
		{
			cv::Mat obj = theBlackObjects[j].object;
			string theSavePath = gOutFolder;
		
			if (strcmp(outChars->at(j), "/") == 0)
				theSavePath += "slash";
			else if (strcmp(outChars->at(j), "*") == 0)
				theSavePath += "asterisk";
			else
				theSavePath += outChars->at(j);

			char buff[10];
			char filename[] = "%i.png";

			sprintf(buff, filename, i);

			theSavePath += "/";
			theSavePath += buff;
			scaleToHeight(theBlackObjects[j].object, 50);
			makeCorrectSize(&theBlackObjects[j].object);
			imwrite(theSavePath, theBlackObjects[j].object);
		}

		preProcessor.reset();
	}
	system("pause");
	cvWaitKey();
}