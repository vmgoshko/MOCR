#ifndef GRAPH_H
#define GRAPH_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include <vector>
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

static void cyclesCountRecursive(Node* inNode, float& outCyclesCount, bool** inMarks)
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

void cyclesCount(Node* inNode, int inNodesCount, float& outCyclesCount)
{
	bool** theMarks = new bool*[inNodesCount];

	for (int i = 0; i < inNodesCount; i++)
	{
		theMarks[i] = new bool[inNodesCount];
		memset(theMarks[i], 0, sizeof(bool) * inNodesCount);
	}

	cyclesCountRecursive(inNode, outCyclesCount, theMarks);

	for (int i = 0; i < inNodesCount; i++)
	{
		delete[] theMarks[i];
	}

	delete[] theMarks;
}

static float distance(cv::Point* a, cv::Point* b)
{
	return cv::norm(*a-*b);
}

static void fullLengthRecursive(Node* inNode, float& outLength, bool** inEdges)
{
	for (auto theNode : inNode->neighbours)
	{
		if (!inEdges[inNode->id][theNode->id] && !inEdges[theNode->id][inNode->id])
		{
			inEdges[inNode->id][theNode->id] = true;
			inEdges[theNode->id][inNode->id] = true;
			outLength += distance(inNode->coords, theNode->coords);
			fullLengthRecursive(theNode, outLength, inEdges);
		}
	}
}

void fullLength(Node* inNode, int inNodesCount, float& outFullLength)
{
	bool** theMarks = new bool*[inNodesCount];

	for (int i = 0; i < inNodesCount; i++)
	{
		theMarks[i] = new bool[inNodesCount];
		memset(theMarks[i], 0, sizeof(bool)* inNodesCount);
	}

	fullLengthRecursive(inNode, outFullLength, theMarks);

	for (int i = 0; i < inNodesCount; i++)
	{
		delete[] theMarks[i];
	}

	delete[] theMarks;
}

void massCenterRecursive(Node* inNode, float& outX, float& outY, bool** inEdges)
{
	for (auto theNode : inNode->neighbours)
	{
		if (!inEdges[inNode->id][theNode->id] && !inEdges[theNode->id][inNode->id])
		{
			inEdges[inNode->id][theNode->id] = true;
			inEdges[theNode->id][inNode->id] = true;
			float theLength = distance(inNode->coords, theNode->coords);

			outX += theLength * (inNode->coords->x + theNode->coords->x) / 2;
			outY += theLength * (inNode->coords->y + theNode->coords->y) / 2;

			massCenterRecursive(theNode, outX, outY, inEdges);
		}
	}
}

void massCenter(Node* inNode, int inNodesCount, float& outX, float& outY)
{
	bool** theEdges = new bool*[inNodesCount];

	for (int i = 0; i < inNodesCount; i++)
	{
		theEdges[i] = new bool[inNodesCount];
		memset(theEdges[i], 0, sizeof(bool)* inNodesCount);
	}

	massCenterRecursive(inNode, outX, outY, theEdges);

	for (int i = 0; i < inNodesCount; i++)
	{
		delete[] theEdges[i];
	}

	delete[] theEdges;
}

void neighboursRecursive(Node* inNode, bool** inEdges, float& outOne, float& outTwo, float& outThree, float& outFour)
{
	switch (inNode->neighbours.size())
	{
		case 1: outOne++; break;
		case 2: outTwo++; break;
		case 3: outThree++; break;
		case 4: outFour++; break;
	}

	for (auto theNode : inNode->neighbours)
	{
		if (!inEdges[inNode->id][theNode->id] && !inEdges[theNode->id][inNode->id])
		{
			inEdges[inNode->id][theNode->id] = true;
			inEdges[theNode->id][inNode->id] = true;
			neighboursRecursive(theNode, inEdges, outOne, outTwo, outThree, outFour);
		}
	}
}

void neighbours(Node* inNode, int inNodesCount, float& outOne, float& outTwo, float& outThree, float& outFour)
{
	bool** theEdges = new bool*[inNodesCount];

	for (int i = 0; i < inNodesCount; i++)
	{
		theEdges[i] = new bool[inNodesCount];
		memset(theEdges[i], 0, sizeof(bool)* inNodesCount);
	}

	neighboursRecursive(inNode, theEdges, outOne, outTwo, outThree, outFour);

	for (int i = 0; i < inNodesCount; i++)
	{
		delete[] theEdges[i];
	}

	delete[] theEdges;
}
#endif