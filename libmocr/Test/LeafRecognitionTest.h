#include "Defs.h"
#include <vector>
#include <string>

struct LeafRecognitionTestData
{
	std::string imagePath;
	std::vector< std::string > expectedChars;

	LeafRecognitionTestData(std::string path)
	{
		imagePath = path;
	}
};

struct LeafRecognitionTestMetrics
{
	size_t charactersCount = 0;
	double fullRecognitionTime = 0;
	double averageRecognitionTime = 0;
	float rightOCRPercent = 0;
	size_t rightOCRCount = 0;
	size_t wrongOCRCount = 0;
};

class LeafRecognitionTest
{
public:
	void InitTestData(std::string inTestDataFilePath);
	void RunTest();
	LeafRecognitionTestMetrics GetMetrics();
	void PrintResult();
private:
	std::vector< LeafRecognitionTestData > mTestData;
	LeafRecognitionTestMetrics mMetrics;
	LeafRecognizerType mType;
};