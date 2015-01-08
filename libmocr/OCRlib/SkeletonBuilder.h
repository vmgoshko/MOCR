#ifndef SKELETON_BUILDER_H
#define SKELETON_BUILDER_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <queue>

#include "Utils.h"

#define min( x, y ) (x) < (y) ? (x) : (y);
#define PI (atan(1) * 4)
#define radians(deg)  ((deg) * PI / 180)
#define degrees(rad)  ((rad) * 180 / PI)

struct Line
{
	cv::Point* start;
	cv::Point* end;
	cv::Point centerPt = cv::Point(-1, -1);
	float len = -1;

	void setStart(int i, int j)
	{
		start->x = i;
		start->y = j;
	}

	void setEnd(int i, int j)
	{
		end->x = i;
		end->y = j;
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

	cv::Point center()
	{
		if (centerPt.x == -1 && centerPt.y == -1)
		{
			centerPt.x = (start->x + end->x) / 2;
			centerPt.y = (start->y + end->y) / 2;
		}

		return centerPt;
	}
};

struct Node
{
	cv::Point* coords;
	std::vector< Node* > neighbours;
	bool drawed = false;
	bool looked = false;
	bool deleted = false;

	void addNeighbour(Node* pt)
	{
		neighbours.push_back(pt);
	}

	void addNeighbours(std::vector<Node*> inNeighbours)
	{
		for (int i = 0; i < inNeighbours.size(); i++)
		{
			if (inNeighbours[i] != this)
				neighbours.push_back(inNeighbours[i]);
		}
	}

	void erase(Node* inNode)
	{
		auto theNode = std::find(neighbours.begin(), neighbours.end(), inNode);
		if( theNode != neighbours.end() )
			neighbours.erase(theNode);
	}

	void clearNeighbours()
	{
		for (int i = 0; i < neighbours.size(); i++)
		{
			neighbours[i]->erase(this);
		}
	}

	void replaceThisByNode(Node* inNode)
	{
		for (int i = 0; i < neighbours.size(); i++)
		{
			if (neighbours[i] != inNode)
			{
				neighbours[i]->erase(this);
				neighbours[i]->addNeighbour(inNode);
			}
		}
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
	std::vector< Line* > vectorize(cv::Mat& inSkeleton, float inObjectColor, cv::Mat& outImg);
	cv::Point* neighbour(cv::Mat& inSkeleton, float inObjectColor, std::vector<cv::Point*>& theLine, int inRow, int inCol);
	float distance(float A, float B, float C, cv::Point* pt);
	bool checkCurvature(std::vector<cv::Point*>& inLine);
	cv::Point* getPoint(int x, int y);
	Node* getNode(int x, int y);
	int neighboursCount(cv::Mat& inSkeleton, float inObjectColor, int inRow, int inCol);
	std::vector< Line* > findLines(cv::Mat& inSkeleton, float inObjectColor, int xStart, int yStart);

	void createNodesPoints(int rows, int cols);
	void deleteNodesPoints();
private:
	std::vector<std::vector<Node*>> mNodes;
	std::vector<std::vector<cv::Point*>> mPoints;
};

#endif //SKELETON_BUILDER_H