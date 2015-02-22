#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include "SkeletonBuilder.h"
#include "Graph.h"

#define min( x, y ) (x) < (y) ? (x) : (y);

SkeletonBuilder::~SkeletonBuilder()
{
	deleteNodesPoints();
}

void SkeletonBuilder::ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst)
{
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (pSrc.at<float>(i, j) == 1.0f)
			{
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int)pSrc.at<float>(i - 1, j - 1);
				int neighbor1 = (int)pSrc.at<float>(i - 1, j);
				int neighbor2 = (int)pSrc.at<float>(i - 1, j + 1);
				int neighbor3 = (int)pSrc.at<float>(i, j + 1);
				int neighbor4 = (int)pSrc.at<float>(i + 1, j + 1);
				int neighbor5 = (int)pSrc.at<float>(i + 1, j);
				int neighbor6 = (int)pSrc.at<float>(i + 1, j - 1);
				int neighbor7 = (int)pSrc.at<float>(i, j - 1);
				int C = int(~neighbor1 & (neighbor2 | neighbor3)) +
					int(~neighbor3 & (neighbor4 | neighbor5)) +
					int(~neighbor5 & (neighbor6 | neighbor7)) +
					int(~neighbor7 & (neighbor0 | neighbor1));

				if (C == 1)
				{
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = min(N1, N2);
					if ((N == 2) || (N == 3))
					{
						/// calculate criteria 3
						int c3 = (neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
						if (c3 == 0)
						{
							pDst.at<float>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}

void SkeletonBuilder::ThinSubiteration2(cv::Mat & pSrc, cv::Mat & pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (pSrc.at<float>(i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int)pSrc.at<float>(i - 1, j - 1);
				int neighbor1 = (int)pSrc.at<float>(i - 1, j);
				int neighbor2 = (int)pSrc.at<float>(i - 1, j + 1);
				int neighbor3 = (int)pSrc.at<float>(i, j + 1);
				int neighbor4 = (int)pSrc.at<float>(i + 1, j + 1);
				int neighbor5 = (int)pSrc.at<float>(i + 1, j);
				int neighbor6 = (int)pSrc.at<float>(i + 1, j - 1);
				int neighbor7 = (int)pSrc.at<float>(i, j - 1);
				int C = int(~neighbor1 & (neighbor2 | neighbor3)) +
					int(~neighbor3 & (neighbor4 | neighbor5)) +
					int(~neighbor5 & (neighbor6 | neighbor7)) +
					int(~neighbor7 & (neighbor0 | neighbor1));
				if (C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = min(N1, N2);
					if ((N == 2) || (N == 3)) {
						int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
						if (E == 0) {
							pDst.at<float>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}

/**
* Perform one thinning iteration.
* Normally you wouldn't call this function directly from your code.
*
* @param  im    Binary image with range = 0-1
* @param  iter  0=even, 1=odd
*/
void SkeletonBuilder::thinningIteration(cv::Mat& im, int iter)
{
	cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows - 1; i++)
	{
		for (int j = 1; j < im.cols - 1; j++)
		{
			uchar p2 = im.at<uchar>(i - 1, j);
			uchar p3 = im.at<uchar>(i - 1, j + 1);
			uchar p4 = im.at<uchar>(i, j + 1);
			uchar p5 = im.at<uchar>(i + 1, j + 1);
			uchar p6 = im.at<uchar>(i + 1, j);
			uchar p7 = im.at<uchar>(i + 1, j - 1);
			uchar p8 = im.at<uchar>(i, j - 1);
			uchar p9 = im.at<uchar>(i - 1, j - 1);

			int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
				(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
				(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i, j) = 1;
		}
	}

	im &= ~marker;
}

/**
* Function for thinning the given binary image
*
* @param  im  Binary image with range = 0-255
*/
void SkeletonBuilder::thinning(cv::Mat& im)
{
	im /= 255;

	cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
	cv::Mat diff;

	do {
		thinningIteration(im, 0);
		thinningIteration(im, 1);
		cv::absdiff(im, prev, diff);
		im.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	im *= 255;
}

void SkeletonBuilder::skeleton(cv::Mat & inputarray, cv::Mat & outputarray) {
	bool bDone = false;
	int rows = inputarray.rows;
	int cols = inputarray.cols;

	inputarray.convertTo(inputarray, CV_32FC1);

	inputarray.copyTo(outputarray);

	outputarray.convertTo(outputarray, CV_32FC1);

	/// pad source
	cv::Mat p_enlarged_src = cv::Mat(rows + 2, cols + 2, CV_32FC1);
	for (int i = 0; i < (rows + 2); i++) {
		p_enlarged_src.at<float>(i, 0) = 0.0f;
		p_enlarged_src.at<float>(i, cols + 1) = 0.0f;
	}
	for (int j = 0; j < (cols + 2); j++) {
		p_enlarged_src.at<float>(0, j) = 0.0f;
		p_enlarged_src.at<float>(rows + 1, j) = 0.0f;
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (inputarray.at<float>(i, j) >= 20.0f) {
				p_enlarged_src.at<float>(i + 1, j + 1) = 1.0f;
			}
			else
				p_enlarged_src.at<float>(i + 1, j + 1) = 0.0f;
		}
	}

	/// start to thin
	cv::Mat p_thinMat1 = cv::Mat::zeros(rows + 2, cols + 2, CV_32FC1);
	cv::Mat p_thinMat2 = cv::Mat::zeros(rows + 2, cols + 2, CV_32FC1);
	cv::Mat p_cmp = cv::Mat::zeros(rows + 2, cols + 2, CV_8UC1);

	while (bDone != true) {
		/// sub-iteration 1
		ThinSubiteration1(p_enlarged_src, p_thinMat1);
		/// sub-iteration 2
		ThinSubiteration2(p_thinMat1, p_thinMat2);
		/// compare
		compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
		/// check
		int num_non_zero = countNonZero(p_cmp);
		if (num_non_zero == (rows + 2) * (cols + 2)) {
			bDone = true;
		}
		/// copy
		p_thinMat2.copyTo(p_enlarged_src);
	}
	// copy result
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			outputarray.at<float>(i, j) = p_enlarged_src.at<float>(i + 1, j + 1);
		}
	}
}

int SkeletonBuilder::neighboursCount(cv::Mat& inSkeleton, float inObjectColor, int inRow, int inCol)
{
	int theNeighboursCount = 0;
	for (int i = inRow - 1; i <= inRow + 1; i++)
	for (int j = inCol - 1; j <= inCol + 1; j++)
	{
		if (i < 0 || j < 0 || i >= inSkeleton.rows || j >= inSkeleton.cols)
			continue;

		if (i == inRow && j == inCol)
			continue;

		if (inSkeleton.at< uchar >(i, j) == inObjectColor)
			theNeighboursCount++;
	}
	return theNeighboursCount;
}

/**
* Perform one thinning iteration.
* Normally you wouldn't call this function directly from your code.
*
* @param  im    Binary image with range = 0-1
* @param  iter  0=even, 1=odd
*/
void SkeletonBuilder::thinningGuoHallIteration(cv::Mat& im, int iter)
{
	cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows - 1; i++)
	{
		for (int j = 1; j < im.cols - 1; j++)
		{
			uchar p2 = im.at<uchar>(i - 1, j);
			uchar p3 = im.at<uchar>(i - 1, j + 1);
			uchar p4 = im.at<uchar>(i, j + 1);
			uchar p5 = im.at<uchar>(i + 1, j + 1);
			uchar p6 = im.at<uchar>(i + 1, j);
			uchar p7 = im.at<uchar>(i + 1, j - 1);
			uchar p8 = im.at<uchar>(i, j - 1);
			uchar p9 = im.at<uchar>(i - 1, j - 1);

			int C = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
				(!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
			int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
			int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
			int N = N1 < N2 ? N1 : N2;
			int m = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);

			if (C == 1 && (N >= 2 && N <= 3) & m == 0)
				marker.at<uchar>(i, j) = 1;
		}
	}

	im &= ~marker;
}

void SkeletonBuilder::createNodesPoints(int rows, int cols)
{
	std::vector< Node* > theNodeRow(cols, nullptr);
	mNodes.assign(rows, theNodeRow);

	std::vector< cv::Point2f* > thePointRow(cols, nullptr);
	mPoints.assign(rows, thePointRow);
}

void SkeletonBuilder::deleteNodesPoints()
{
	for (int i = 0; i < mNodes.size(); i++)
	{
		for (int j = 0; j < mNodes[i].size(); j++)
		{
			delete mNodes[i][j];
			delete mPoints[i][j];
		}
	}
}


Node* SkeletonBuilder::createGraph(std::vector< Line*> inLines)
{
	if (inLines.size() == 0)
	{
		Node* theNode = getNode(0, 0);
		theNode->coords = new cv::Point2f(0, 0);
		return theNode;
	}

	for (auto theLine : inLines)
	{
		Node* theNode = getNode(theLine->start->x, theLine->start->y);
		theNode->coords = theLine->start;

		Node* theChildNode = getNode(theLine->end->x, theLine->end->y);
		theChildNode->coords = theLine->end;

		theNode->addNeighbour(theChildNode);
		theChildNode->addNeighbour(theNode);
	}

	Node* theNode = getNode(inLines[0]->start->x, inLines[0]->start->y);

	return theNode;
}


void SkeletonBuilder::findStartPixel(cv::Mat& inSkeleton, float inObjectColor, int& outRow, int& outCol)
{
	outRow = -1;
	outCol = -1;

	int sxStart = -1;
	int syStart = -1;
	bool found = false;

	for (int i = 0; i < inSkeleton.rows; i++)
	{
		for (int j = 0; j < inSkeleton.cols; j++)
		{
			uchar thePixel = inSkeleton.at<uchar>(i, j);
			if (thePixel > 0 && sxStart == -1 && syStart == -1)
			{
				sxStart = i;
				syStart = j;
			}

			if (thePixel > 0 && neighboursCount(inSkeleton, inObjectColor, i, j) == 1)
			{
				outRow = i;
				outCol = j;
				inSkeleton.at< uchar >(i, j) = 150;
				found = true;
				break;
			}
		}
		if (found)
			break;
	}

	if (!found)
	{
		outRow = sxStart;
		outCol = syStart;
	}
}

Node* SkeletonBuilder::vectorize(cv::Mat& inSkeleton, float inObjectColor, int& outSectionsCount, cv::Mat& outImg)
{
	createNodesPoints(inSkeleton.rows, inSkeleton.cols);
	int xStart;
	int yStart;

	findStartPixel(inSkeleton, inObjectColor, xStart, yStart);
	outImg = cv::Mat(inSkeleton.rows, inSkeleton.cols, CV_32SC3);
	outImg.setTo(0);
	
	std::vector< Line* > theLines = findLines(inSkeleton, inObjectColor, xStart, yStart);

	outSectionsCount = theLines.size();

	Node* theRoot = createGraph(theLines);
	theRoot->root = true;
	deepSearch(theRoot);
#ifdef _DEBUG
	deepDraw(theRoot, outImg);
#endif

	return theRoot;
}

cv::Point2f* SkeletonBuilder::neighbour(cv::Mat& inSkeleton, float inObjectColor, std::vector<cv::Point2f*>& theLine, int inRow, int inCol)
{
	cv::Point2f* theResult;

	for (int i = inRow - 1; i <= inRow + 1; i++)
		for (int j = inCol - 1; j <= inCol + 1; j++)
		{
			if (i < 0 || j < 0 || i >= inSkeleton.rows || j >= inSkeleton.cols)
				continue;

			if (i == inRow && j == inCol)
				continue;
			
			if (inSkeleton.at<uchar>(i, j) == inObjectColor )
			{
				cv::Point2f* thePoint = getPoint(i, j);
				return thePoint;
			}
		}

	return NULL;
}

float SkeletonBuilder::distance(float A, float B, float C, cv::Point2f* pt)
{
	float theDistance = abs(A * pt->x + B * pt->y + C);
	theDistance /= sqrt(A*A + B*B);

	return theDistance;
}

bool SkeletonBuilder::isAllowableLine(std::vector<cv::Point2f*>& inLine)
{
	if (inLine.size() < 3)
		return true;

	Line theLine;
	theLine.start = *inLine.begin();
	theLine.end = *(inLine.end() - 1);

	float A = theLine.start->y - theLine.end->y;
	float B = theLine.end->x - theLine.start->x;
	float C = theLine.start->x*theLine.end->y - theLine.end->x*theLine.start->y;

	float theDistance = distance(A, B, C, inLine[inLine.size() / 2]);

	return theDistance < Config::cMaxDistance && inLine.size() <= Config::cMaxLineLength;
}

cv::Point2f* SkeletonBuilder::getPoint(int x, int y)
{
	if (mPoints[x][y] == NULL)
	{
		mPoints[x][y] = new cv::Point2f(x, y);
	}

	return mPoints[x][y];
}

Node* SkeletonBuilder::getNode(int x, int y)
{
	if (mNodes[x][y] == NULL)
	{
		mNodes[x][y] = new Node(mNodesCount++);
	}

	return mNodes[x][y];
}


Line* SkeletonBuilder::newLine(std::vector< cv::Point2f* >& inLine, int& outNewX, int& outNewY)
{
	Line* theNewLine = new Line();
	theNewLine->start = *inLine.begin();
	theNewLine->end = *(inLine.end() - 1);

	inLine.clear();

	outNewX = theNewLine->end->x;
	outNewY = theNewLine->end->y;

	inLine.push_back(getPoint(theNewLine->end->x, theNewLine->end->y));
	return theNewLine;
}

std::vector< Line* > SkeletonBuilder::findLines(cv::Mat& inSkeleton, float inObjectColor, int xStart, int yStart)
{
	std::vector<cv::Point2f*> theLine;
	std::vector< Line* > theResult;
	std::queue< cv::Point2f*> mCrossPoints;

	theLine.push_back(getPoint(xStart, yStart));

	int x = xStart;
	int y = yStart;
	while (true)
	{
		cv::Point2f* theNeighbour = neighbour(inSkeleton, inObjectColor, theLine, x, y);
		
		if (!theNeighbour && mCrossPoints.empty())
			break;

		if (!theNeighbour)
		{
			cv::Point2f thePoint = *mCrossPoints.front();
			std::vector< Line* > theCrossPtLines = findLines(inSkeleton, inObjectColor, thePoint.x, thePoint.y);
			theResult.insert(theResult.end(), theCrossPtLines.begin(), theCrossPtLines.end());

			if (neighboursCount(inSkeleton, inObjectColor, thePoint.x, thePoint.y) == 0)
			{
				mCrossPoints.pop();
				continue;
			}
		}

		inSkeleton.at<uchar>(theNeighbour->x, theNeighbour->y) = 150;
		theLine.push_back(theNeighbour);

		if (neighboursCount(inSkeleton, inObjectColor, theNeighbour->x, theNeighbour->y) > 1)
		{
			inSkeleton.at<uchar>(theNeighbour->x, theNeighbour->y) = 100; 
			
			mCrossPoints.push(getPoint(theNeighbour->x, theNeighbour->y));

			Line* theNewLine = newLine(theLine, x, y);
			theResult.push_back(theNewLine);
			continue;
		}
		else if (neighboursCount(inSkeleton, inObjectColor, theNeighbour->x, theNeighbour->y) == 0)
		{
			if (theNeighbour->x == 22 && theNeighbour->y == 57)
				int a = 0;

			theNeighbour = neighbour(inSkeleton, 100, theLine, theNeighbour->x, theNeighbour->y);
			if (!theNeighbour)
			{

				Line* theNewLine = newLine(theLine, x, y);
				theResult.push_back(theNewLine);
				continue;
			}
			else
			{
				if (theLine[0] != theNeighbour)
				{
					theLine.push_back(theNeighbour);
	
					Line* theNewLine = newLine(theLine, x, y);
					theResult.push_back(theNewLine);
				}
			}
		}

		if (!isAllowableLine(theLine))
		{
			inSkeleton.at<uchar>(theNeighbour->x, theNeighbour->y) = 255;
			theLine.pop_back();

			Line* theNewLine = newLine(theLine, x, y);
			theResult.push_back(theNewLine);
		}
		else
		{
			x = theNeighbour->x;
			y = theNeighbour->y;
		}
	}

	return theResult;
}

/**
* Function for thinning the given binary image
*
* @param  im  Binary image with range = 0-255
*/
void SkeletonBuilder::thinningGuoHall(cv::Mat& im)
{
	im /= 255;

	cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
	cv::Mat diff;

	do {
		thinningGuoHallIteration(im, 0);
		thinningGuoHallIteration(im, 1);
		cv::absdiff(im, prev, diff);
		im.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	im *= 255;
}

/*
Функция вычисляет характеристики скилета изображения:
1. Центр тяжести относительно оси OX
2. Центр тяжести относительно оси OY
3. Среднеквадратичное отклонение от центра тяжести отностиельно OX
4. Среднеквадратичное отклонение от центра тяжести отностиельно OY

5. Количество скелетных точек, связанных с соседними по горизонтали, к общему количеству скелетных точек
6. Количество скелетных точек, связанных с соседними по вертикали, к общему количеству скелетных точек
7. Количество скелетных точек, связанных с соседними по главной диагонали, к общему количеству скелетных точек
8. Количество скелетных точек, связанных с соседними по обратной диагонали, к общему количеству скелетных точек
*/
std::vector< float > RasterSkeletonBuilder::calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor)
{
	cv::Mat theSavedSkeleton = inSkeleton;
	cv::Mat theBoundedSkeleton = bound(&inSkeleton, inObjectColor).object;

	int theHeight = theBoundedSkeleton.rows;
	int theWidth = theBoundedSkeleton.cols;

	int theSkeletonPixelsCount = 0;
	int theHorizontalJoinCount = 0;
	int theVerticalJoinCount = 0;
	int theMainDiagonalJoinCount = 0;
	int theSideDiagonalJoinCount = 0;

	std::vector< float > theCharacteristics(11, 0);

	for (int i = 0; i < theHeight; i++)
	for (int j = 0; j < theWidth; j++)
	{
		uchar thePixel = theBoundedSkeleton.at< uchar >(i, j);

		if (thePixel > 0)
		{
			//подсчет соседства
			uchar theTop = (i > 0) ? theBoundedSkeleton.at< uchar >(i - 1, j) : 0;
			uchar theTopRight = (i > 0) && (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i - 1, j + 1) : 0;
			uchar theRight = (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i, j + 1) : 0;
			uchar theBottomRight = (i < theHeight - 1) && (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i + 1, j + 1) : 0;
			uchar theBottom = (i < theHeight - 1) ? theBoundedSkeleton.at< uchar >(i + 1, j) : 0;
			uchar theBottomLeft = (i < theHeight - 1) && (j > 0) ? theBoundedSkeleton.at< uchar >(i + 1, j - 1) : 0;
			uchar theLeft = (j > 0) ? theBoundedSkeleton.at< uchar >(i, j - 1) : 0;
			uchar theTopLeft = (i > 0) && (j > 0) ? theBoundedSkeleton.at< uchar >(i - 1, j - 1) : 0;

			// Количество точек скелета
			theSkeletonPixelsCount++;

			// Центр тяжести относительно оси OX
			theCharacteristics[0] += j;
			// Центр тяжести относительно оси OY
			theCharacteristics[1] += i;

			// Количество скелетных точек, связанных с соседними по горизонтали, к общему количеству скелетных точек:
			if (theLeft == inObjectColor || theRight == inObjectColor)
				theCharacteristics[4] += 1;
			// Количество скелетных точек, связанных с соседними по вертикали, к общему количеству скелетных точек:
			if (theTop == inObjectColor || theBottom == inObjectColor)
				theCharacteristics[5] += 1;
			// Количество скелетных точек, связанных с соседними по главной диагонали, к общему количеству скелетных точек:
			if (theTopLeft == inObjectColor || theBottomRight == inObjectColor)
				theCharacteristics[6] += 1;
			// Количество скелетных точек, связанных с соседними по обратной диагонали, к общему количеству скелетных точек:
			if (theTopRight == inObjectColor || theBottomLeft == inObjectColor)
				theCharacteristics[7] += 1;
				
			if (neighboursCount(theBoundedSkeleton, inObjectColor, i, j) == 1)
				theCharacteristics[8]++;

			if (neighboursCount(theBoundedSkeleton, inObjectColor, i, j) == 3)
				theCharacteristics[9]++;

			if (neighboursCount(theBoundedSkeleton, inObjectColor, i, j) == 4)
				theCharacteristics[10]++;
		}
	}

	theCharacteristics[0] /= theSkeletonPixelsCount;
	theCharacteristics[1] /= theSkeletonPixelsCount;

	theCharacteristics[4] /= theSkeletonPixelsCount;
	theCharacteristics[5] /= theSkeletonPixelsCount;
	theCharacteristics[6] /= theSkeletonPixelsCount;
	theCharacteristics[7] /= theSkeletonPixelsCount;

	for (int i = 0; i < theHeight; i++)
		for (int j = 0; j < theWidth; j++)
		{
			uchar thePixel = theBoundedSkeleton.at< uchar >(i, j);

			if (thePixel > 0)
			{
				// Среднеквадратичное отклонение от центра тяжести отностиельно OX
				theCharacteristics[2] += (j - theCharacteristics[0]) * (j - theCharacteristics[0]);
				// Среднеквадратичное отклонение от центра тяжести отностиельно OY
				theCharacteristics[3] += (i - theCharacteristics[1]) * (i - theCharacteristics[1]);
			}
		}

	theCharacteristics[2] = sqrtf(theCharacteristics[2]);
	theCharacteristics[2] /= theSkeletonPixelsCount;
	theCharacteristics[2] /= theWidth;

	theCharacteristics[3] = sqrtf(theCharacteristics[3]);
	theCharacteristics[3] /= theSkeletonPixelsCount;
	theCharacteristics[3] /= theHeight;

	theCharacteristics[0] /= theWidth;
	theCharacteristics[1] /= theHeight;

	return theCharacteristics;
}

std::vector< float > VectorSkeletonBuilder::calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor)
{
	cv::Mat theVectorImage;
	int theSectionsCount = 0;
	Node* theRoot = vectorize(inSkeleton, inObjectColor, theSectionsCount, theVectorImage);
	std::vector< float > theCharacteristics(8, 0);
	
	float theLength = 0;
	fullLength(theRoot, mNodesCount, theLength);
	massCenter(theRoot, mNodesCount, theCharacteristics[0], theCharacteristics[1]);
	cyclesCount(theRoot, mNodesCount, theCharacteristics[2]);
	
	theCharacteristics[0] /= theLength;
	theCharacteristics[1] /= theLength;

	float f = 0;
	neighbours(theRoot, mNodesCount, theCharacteristics[3], f, theCharacteristics[4], theCharacteristics[5]);
	float theWidth;
	float theHeight;

	findWidthHeigth(theRoot, mNodesCount, theWidth, theHeight);

	int theLeftTurns = 0;
	int theRightTurns = 0;
	turns(theRoot, mNodesCount, theLeftTurns, theRightTurns);
	theCharacteristics[6] = theLeftTurns;
	theCharacteristics[7] = theRightTurns;

	theCharacteristics[0] /= theHeight;
	theCharacteristics[1] /= theWidth;
	
	return theCharacteristics;
}

void BGR2GRAY(cv::Mat& bgr, cv::Mat& gray)
{
	for (int i = 0; i < bgr.rows; i++)
		for (int j = 0; j < bgr.cols; j++)
		{
			auto color = bgr.at<cv::Vec3i>(i, j);
			if (color[0] == 255)
				gray.at<uchar>(i, j) = 255;
			else
				gray.at<uchar>(i, j) = 0;
		}
}

std::vector< float > CombinedSkeletonBuilder::calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor)
{
	cv::Mat theVectorImage;
	int theSectionsCount = 0;
	Node* theRoot = vectorize(inSkeleton, inObjectColor, theSectionsCount, theVectorImage);
	cv::Mat theVectorSkeleton(theVectorImage.rows, theVectorImage.cols, CV_8UC1);
	BGR2GRAY(theVectorImage, theVectorSkeleton);

	SkeletonBuilder* theRasterSkeletonBuilder = new RasterSkeletonBuilder();

	std::vector< float > theCharacteristics = theRasterSkeletonBuilder->calculateCharacteristic(theVectorSkeleton, inObjectColor);

	float theCyclesCount = 0;
	cyclesCount(theRoot, mNodesCount, theCyclesCount);
	theCharacteristics.push_back(theCyclesCount);
	
	int theLeftTurns = 0;
	int theRightTurns = 0;
	turns(theRoot, mNodesCount, theLeftTurns, theRightTurns);
	theCharacteristics.push_back( theLeftTurns );
	theCharacteristics.push_back( theRightTurns );

	delete theRasterSkeletonBuilder;
	return theCharacteristics;
}