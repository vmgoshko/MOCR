﻿#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <fstream>
#include "PyRecognize.h"

namespace {
	const char* imagePath = "img/numbers.png";
};

void main()
{	
	std::cout << NativeRecognize(imagePath) << std::endl;
	system("pause");
	cvWaitKey( 0 );
}