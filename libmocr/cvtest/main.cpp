#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <fstream>
#include <string>

#include "ocr.h"

using namespace std;

namespace {
	const char* cImagePath = "img/iwona_1.png";
};

Configuration gConfiguration;

void main()
{	
	gConfiguration = { "../nn_skeleton_raster", LeafRecognizerType::TwoStepsRecognizer, NNToolsType::Skeleton, SkeletonType::Vector };
	std::cout << Recognize(cImagePath) << std::endl;
	system("pause");
}