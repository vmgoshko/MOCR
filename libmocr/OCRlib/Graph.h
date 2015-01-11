#ifndef GRAPH_H
#define GRAPH_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

struct Node
{
	cv::Point* coords;
	std::vector< Node* > neighbours;
	int id;

	bool drawed = false;
	bool looked = false;
	bool visitedCycle = false;
	bool visitedMassCenter = false;

	Node(int inID) : id(inID) {}

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
		if (theNode != neighbours.end())
			neighbours.erase(theNode);
	}

	void clearNeighbours()
	{
		for (int i = 0; i < neighbours.size(); i++)
		{
			neighbours[i]->erase(this);
		}
	}

	void replaceThisByNode(Node* src, Node* inNode)
	{
		for (int i = 0; i < neighbours.size(); i++)
		{
			if (neighbours[i] != inNode)
			{
				neighbours[i]->erase(src);
				neighbours[i]->addNeighbour(inNode);
			}
		}
	}
};

void deepDraw(Node* inNode, cv::Mat& outImg)
{
	inNode->drawed = true;
	cv::Point start;
	cv::Point end;
	start.x = inNode->coords->y;
	start.y = inNode->coords->x;

	for (auto theNode : inNode->neighbours)
	{
		end.x = theNode->coords->y;
		end.y = theNode->coords->x;

		line(outImg, start, end, cv::Scalar(255, 0, 0), 1);
		line(outImg, end, end, cv::Scalar(255, 255, 255), 1);

		if (!theNode->drawed)
			deepDraw(theNode, outImg);
	}

	line(outImg, start, start, cv::Scalar(255, 255, 255), 1);
}

bool isNeighbours(cv::Point* f, cv::Point* s)
{
	return abs(f->x - s->x) <= Config::cNeighboursDistance &&
			abs(f->y - s->y) <= Config::cNeighboursDistance &&
			(abs(f->x - s->x) + abs(f->y - s->y) != 0);
}

void deepSearch(Node* inNode)
{
	inNode->looked = true;

	for (int i = 0; i < inNode->neighbours.size(); i++)
	{
		Node* theNode = inNode->neighbours[i];
		if (isNeighbours(inNode->coords, theNode->coords))
		{
			inNode->erase(theNode);
			theNode->erase(inNode);
			theNode->replaceThisByNode(theNode, inNode);
			inNode->addNeighbours(theNode->neighbours);
			--i;
		}
	}

	for (int i = 0; i < inNode->neighbours.size(); i++)
	{
		Node* theNode = inNode->neighbours[i];
		if (!theNode->looked)
		{
			deepSearch(theNode);
		}
	}
}

static void cyclesCountRecursive(Node* inNode, int& outCyclesCount, bool** inMarks)
{
	if (inNode->visitedCycle)
	{
		outCyclesCount++;
		return;
	}

	inNode->visitedCycle = true;

	for (auto theNode : inNode->neighbours)
	{
		if (!inMarks[inNode->id][theNode->id] && !inMarks[theNode->id][inNode->id])
		{
			inMarks[inNode->id][theNode->id] = true;
			inMarks[theNode->id][inNode->id] = true;
			cyclesCountRecursive(theNode, outCyclesCount, inMarks);
		}
	}
}

int cyclesCount(Node* inNode, int inNodesCount)
{
	bool** theMarks = new bool*[inNodesCount];

	for (int i = 0; i < inNodesCount; i++)
	{
		theMarks[i] = new bool[inNodesCount];
		memset(theMarks[i], 0, sizeof(bool) * inNodesCount);
	}

	int theCyclesCount = 0;
	
	cyclesCountRecursive(inNode, theCyclesCount, theMarks);

	for (int i = 0; i < inNodesCount; i++)
	{
		delete[] theMarks[i];
	}

	delete[] theMarks;

	return theCyclesCount;
}

void massCenter(Node* inNode, float& outX, float& outY)
{
	if (inNode->visitedMassCenter)
		return;

	outX += inNode->coords->x;
	outY += inNode->coords->y;
	inNode->visitedMassCenter = true;

	for (auto theNode : inNode->neighbours)
		massCenter(theNode, outX, outY);
}
#endif