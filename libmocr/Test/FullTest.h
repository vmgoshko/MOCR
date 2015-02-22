#include <vector>
#include <string>

struct FullTestData
{
	std::string imagePath;
	std::string expectedResult;

	FullTestData(std::string path, std::string result)
	{
		imagePath = path;
		expectedResult = result;
	}
};

struct FullTestMetrics
{
	size_t imagesCount;
	time_t overallTime;
	time_t averageTime;
	float rightOCRPercent;
};

class FullTest
{
public:
	void InitTestData(std::string inTestDataFilePath);
	void RunTest();
	FullTestMetrics GetMetrics();
private:
	std::vector< FullTestData > mTestData;
	FullTestMetrics mMetrics;
};