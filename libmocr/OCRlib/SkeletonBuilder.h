#ifndef SKELETON_BUILDER_H
#define SKELETON_BUILDER_H

#include <vector>
#include <queue>

#include "Utils.h"

#define min( x, y ) (x) < (y) ? (x) : (y);
#define PI (atan(1) * 4)
#define radians(deg)  ((deg) * PI / 180)
#define degrees(rad)  ((rad) * 180 / PI)

struct Node;

struct Line
{
	cv::Point2f* start;
	cv::Point2f* end;
	cv::Point2f centerPt = cv::Point2f(-1, -1);
	float len = -1;

	void setStart(int i, int j)
	{
		start->x = (float)i;
		start->y = (float)j;
	}

	void setEnd(int i, int j)
	{
		end->x = (float)i;
		end->y = (float)j;
	}

	float length()
	{
		if (len == -1)
		{
			float x = start->x - end->x;
			float y = start->y - end->y;

			len = sqrt(x*x + y*y);
		}

		return len;
	}

	cv::Point2f center()
	{
		if (centerPt.x == -1 && centerPt.y == -1)
		{
			centerPt.x = (start->x + end->x) / 2;
			centerPt.y = (start->y + end->y) / 2;
		}

		return centerPt;
	}
};

class SkeletonBuilder
{

public:
	~SkeletonBuilder();
	void thinningGuoHall(cv::Mat& im);
	std::vector< float > calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor);
	std::vector< float > calculateCharacteristicVectorize(cv::Mat& inSkeleton, float inObjectColor);

private:
	void ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst);
	void ThinSubiteration2(cv::Mat & pSrc, cv::Mat & pDst);
	void thinningIteration(cv::Mat& im, int iter);
	void thinning(cv::Mat& im);
	void skeleton(cv::Mat & inputarray, cv::Mat & outputarray);
	void thinningGuoHallIteration(cv::Mat& im, int iter);
	void createNodesPoints(int rows, int cols);
	void deleteNodesPoints();
	void findStartPixel(cv::Mat& inSkeleton, float inObjectColor, int& outRow, int& outCol);

	float distance(float A, float B, float C, cv::Point2f* pt);
	bool isAllowableLine(std::vector<cv::Point2f*>& inLine);
	int neighboursCount(cv::Mat& inSkeleton, float inObjectColor, int inRow, int inCol);

	std::vector< Line* > findLines(cv::Mat& inSkeleton, float inObjectColor, int xStart, int yStart);
	
	cv::Point2f* neighbour(cv::Mat& inSkeleton, float inObjectColor, std::vector<cv::Point2f*>& theLine, int inRow, int inCol);
	cv::Point2f* getPoint(int x, int y);
	Node* getNode(int x, int y);
	Node* createGraph(std::vector< Line*> inLines);
	Node* vectorize(cv::Mat& inSkeleton, float inObjectColor, int& outSectionsCount, cv::Mat& outImg);
	Line* newLine(std::vector< cv::Point2f* >& inLine, int& outNewX, int& outNewY);

private:
	std::vector<std::vector<Node*>> mNodes;
	std::vector<std::vector<cv::Point2f*>> mPoints;
	std::queue< cv::Point2f* > mCrossPoints;
	int mNodesCount = 0;
};

#endif //SKELETON_BUILDER_H