#include "SkeletonBuilder.h"

#define min( x, y ) (x) < (y) ? (x) : (y);

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


std::vector< Line* > SkeletonBuilder::vectorize(cv::Mat& inSkeleton, float inObjectColor)
{
	int xStart = -1;
	int yStart = -1;

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
				xStart = i;
				yStart = j;
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
		xStart = sxStart;
		yStart = syStart;
	}

	cv::Point*** thePoints = new cv::Point**[inSkeleton.rows];
	for (int i = 0; i < inSkeleton.rows; i++)
	{
		thePoints[i] = new cv::Point*[inSkeleton.cols];
		memset(thePoints[i], 0, sizeof(cv::Point*) * inSkeleton.cols);
	}

	cv::Mat img(inSkeleton.rows, inSkeleton.cols, CV_32SC3);
	img.setTo(0);
	std::vector< Line* > theLines = findLines(inSkeleton, inObjectColor, xStart, yStart, thePoints);
	for (auto theLine : theLines)
	{
		inSkeleton.at<uchar>(theLine->start->x, theLine->start->y) = 255;
		inSkeleton.at<uchar>(theLine->end->x, theLine->end->y) = 255;
		cv::Point start;
		cv::Point end;
		start.x = theLine->start->y;
		start.y = theLine->start->x;

		end.x = theLine->end->y;
		end.y = theLine->end->x;
		line(img, start, end, cv::Scalar(255, 0, 0), 1);
	}

	return theLines;
}

cv::Point* SkeletonBuilder::neighbour(cv::Mat& inSkeleton, float inObjectColor, cv::Point*** inPoints, int inRow, int inCol)
{
	cv::Point* theResult;

	for (int i = inRow - 1; i <= inRow + 1; i++)
	for (int j = inCol - 1; j <= inCol + 1; j++)
	{
		if (i < 0 || j < 0 || i >= inSkeleton.rows || j >= inSkeleton.cols)
			continue;

		if (i == inRow && j == inCol)
			continue;

		if (inSkeleton.at<uchar>(i, j) == inObjectColor)
			return getPoint(inPoints, i, j);
	}

	return NULL;
}

float SkeletonBuilder::distance(float A, float B, float C, cv::Point* pt)
{
	float theDistance = abs(A * pt->x + B * pt->y + C);
	theDistance /= sqrt(A*A + B*B);

	return theDistance;
}

bool SkeletonBuilder::checkCurvature(std::vector<cv::Point*>& inLine)
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

	return theDistance < 10.f;
}

cv::Point* SkeletonBuilder::getPoint(cv::Point*** inPoints, int x, int y)
{
	if (inPoints[x][y] == NULL)
	{
		inPoints[x][y] = new cv::Point(x, y);
	}

	return inPoints[x][y];
}

std::vector< Line* > SkeletonBuilder::findLines(cv::Mat& inSkeleton, float inObjectColor, int xStart, int yStart, cv::Point*** inPoints)
{
	std::vector<cv::Point*> theLine;
	std::vector< Line* > theResult;
	std::queue< cv::Point*> theCrossPoints;

	theLine.push_back(getPoint(inPoints, xStart, yStart));

	int x = xStart;
	int y = yStart;
	while (true)
	{
		cv::Point* theNeighbour = neighbour(inSkeleton, inObjectColor, inPoints, x, y);

		if (!theNeighbour && theCrossPoints.empty())
			break;

		if (!theNeighbour)
		{
			cv::Point thePoint = *theCrossPoints.front();
			std::vector< Line* > theCrossPtLines = findLines(inSkeleton, inObjectColor, thePoint.x, thePoint.y, inPoints);
			theResult.insert(theResult.end(), theCrossPtLines.begin(), theCrossPtLines.end());

			if (neighboursCount(inSkeleton, inObjectColor, thePoint.x, thePoint.y) == 0)
			{
				theCrossPoints.pop();
				continue;
			}
		}

		inSkeleton.at<uchar>(theNeighbour->x, theNeighbour->y) = 150;
		theLine.push_back(theNeighbour);

		if (neighboursCount(inSkeleton, inObjectColor, theNeighbour->x, theNeighbour->y) > 1)
		{
			theCrossPoints.push(getPoint(inPoints, theNeighbour->x, theNeighbour->y));

			Line* theNewLine = new Line();
			theNewLine->start = *theLine.begin();
			theNewLine->end = *(theLine.end() - 1);

			theResult.push_back(theNewLine);
			theLine.clear();
			x = theNewLine->end->x;
			y = theNewLine->end->y;

			theLine.push_back(getPoint(inPoints, theNewLine->end->x, theNewLine->end->y));
			continue;
		}
		else if (neighboursCount(inSkeleton, inObjectColor, theNeighbour->x, theNeighbour->y) == 0)
		{
			Line* theNewLine = new Line();
			theNewLine->start = *theLine.begin();
			theNewLine->end = *(theLine.end() - 1);

			theResult.push_back(theNewLine);
			theLine.clear();
			x = theNewLine->end->x;
			y = theNewLine->end->y;

			theLine.push_back(getPoint(inPoints, theNewLine->end->x, theNewLine->end->y));
			continue;
		}

		if (!checkCurvature(theLine))
		{
			inSkeleton.at<uchar>(theNeighbour->x, theNeighbour->y) = 255;
			theLine.pop_back();
			Line* theNewLine = new Line();
			theNewLine->start = *theLine.begin();
			theNewLine->end = *(theLine.end() - 1);

			theResult.push_back(theNewLine);
			theLine.clear();
			x = theNewLine->end->x;
			y = theNewLine->end->y;
			theLine.push_back(getPoint(inPoints, theNewLine->end->x, theNewLine->end->y));
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
������� ��������� �������������� ������� �����������:
1. ����� ������� ������������ ��� OX
2. ����� ������� ������������ ��� OY
3. ������������������ ���������� �� ������ ������� ������������ OX
4. ������������������ ���������� �� ������ ������� ������������ OY

5. ���������� ��������� �����, ��������� � ��������� �� �����������, � ������ ���������� ��������� �����
6. ���������� ��������� �����, ��������� � ��������� �� ���������, � ������ ���������� ��������� �����
7. ���������� ��������� �����, ��������� � ��������� �� ������� ���������, � ������ ���������� ��������� �����
8. ���������� ��������� �����, ��������� � ��������� �� �������� ���������, � ������ ���������� ��������� �����
*/
std::vector< float > SkeletonBuilder::calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor)
{
	cv::Mat theSavedSkeleton = inSkeleton;
	cv::Mat theBoundedSkeleton = bound(&inSkeleton, inObjectColor).object;

	vectorize(theSavedSkeleton, inObjectColor);

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
			//������� ���������
			uchar theTop = (i > 0) ? theBoundedSkeleton.at< uchar >(i - 1, j) : 0;
			uchar theTopRight = (i > 0) && (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i - 1, j + 1) : 0;
			uchar theRight = (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i, j + 1) : 0;
			uchar theBottomRight = (i < theHeight - 1) && (j < theWidth - 1) ? theBoundedSkeleton.at< uchar >(i + 1, j + 1) : 0;
			uchar theBottom = (i < theHeight - 1) ? theBoundedSkeleton.at< uchar >(i + 1, j) : 0;
			uchar theBottomLeft = (i < theHeight - 1) && (j > 0) ? theBoundedSkeleton.at< uchar >(i + 1, j - 1) : 0;
			uchar theLeft = (j > 0) ? theBoundedSkeleton.at< uchar >(i, j - 1) : 0;
			uchar theTopLeft = (i > 0) && (j > 0) ? theBoundedSkeleton.at< uchar >(i - 1, j - 1) : 0;

			// ���������� ����� �������
			theSkeletonPixelsCount++;

			// ����� ������� ������������ ��� OX
			theCharacteristics[0] += j;
			// ����� ������� ������������ ��� OY
			theCharacteristics[1] += i;

			// ���������� ��������� �����, ��������� � ��������� �� �����������, � ������ ���������� ��������� �����:
			if (theLeft == inObjectColor || theRight == inObjectColor)
				theCharacteristics[4] += 1;
			// ���������� ��������� �����, ��������� � ��������� �� ���������, � ������ ���������� ��������� �����:
			if (theTop == inObjectColor || theBottom == inObjectColor)
				theCharacteristics[5] += 1;
			// ���������� ��������� �����, ��������� � ��������� �� ������� ���������, � ������ ���������� ��������� �����:
			if (theTopLeft == inObjectColor || theBottomRight == inObjectColor)
				theCharacteristics[6] += 1;
			// ���������� ��������� �����, ��������� � ��������� �� �������� ���������, � ������ ���������� ��������� �����:
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
			// ������������������ ���������� �� ������ ������� ������������ OX
			theCharacteristics[2] += (j - theCharacteristics[0]) * (j - theCharacteristics[0]);
			// ������������������ ���������� �� ������ ������� ������������ OY
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

std::vector< float > SkeletonBuilder::calculateCharacteristicVectorize(cv::Mat& inSkeleton, float inObjectColor)
{
	std::vector< Line* > theLines = vectorize(inSkeleton, inObjectColor);
	std::vector< float > theCharacteristics(9, 0);
	float theLength = 0;

	for (Line* theLine : theLines)
	{
		theLength += theLine->length();
	}

	for (Line* theLine : theLines)
	{
		theCharacteristics[0] += theLine->length() * theLine->center().x;
		theCharacteristics[1] += theLine->length() * theLine->center().y;
	}

	theCharacteristics[0] /= theLength;
	theCharacteristics[1] /= theLength;

	for (Line* theLine : theLines)
	{
		theCharacteristics[2] += (theLine->center().x - theCharacteristics[0]) * (theLine->center().x - theCharacteristics[0]);
		theCharacteristics[3] += (theLine->center().y - theCharacteristics[0]) * (theLine->center().y - theCharacteristics[0]);
	}

	theCharacteristics[2] = sqrtf(theCharacteristics[2]);
	theCharacteristics[2] /= theLines.size();

	theCharacteristics[3] = sqrtf(theCharacteristics[3]);
	theCharacteristics[3] /= theLines.size();

	return theCharacteristics;
}