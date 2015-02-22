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

void main()
{	
	std::cout << Recognize(cImagePath) << std::endl;
	system("pause");
}