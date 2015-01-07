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

class SkeletonBuilder
{
private:
	void ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst);
	void ThinSubiteration2(cv::Mat & pSrc, cv::Mat & pDst);
	/**
	* Perform one thinning iteration.
	* Normally you wouldn't call this function directly from your code.
	*
	* @param  im    Binary image with range = 0-1
	* @param  iter  0=even, 1=odd
	*/
	void thinningIteration(cv::Mat& im, int iter);

	/**
	* Function for thinning the given binary image
	*
	* @param  im  Binary image with range = 0-255
	*/
	void thinning(cv::Mat& im);
	void skeleton(cv::Mat & inputarray, cv::Mat & outputarray);
	int neighboursCount(cv::Mat& inSkeleton, float inObjectColor, int inRow, int inCol);

	/**
	* Perform one thinning iteration.
	* Normally you wouldn't call this function directly from your code.
	*
	* @param  im    Binary image with range = 0-1
	* @param  iter  0=even, 1=odd
	*/
	void thinningGuoHallIteration(cv::Mat& im, int iter);
	std::vector< Line* > vectorize(cv::Mat& inSkeleton, float inObjectColor);
	cv::Point* neighbour(cv::Mat& inSkeleton, float inObjectColor, cv::Point*** inPoints, int inRow, int inCol);
	float distance(float A, float B, float C, cv::Point* pt);
	bool checkCurvature(std::vector<cv::Point*>& inLine);
	cv::Point* getPoint(cv::Point*** inPoints, int x, int y);
	std::vector< Line* > findLines(cv::Mat& inSkeleton, float inObjectColor, int xStart, int yStart, cv::Point*** inPoints);

public:
	/**
	* Function for thinning the given binary image
	*
	* @param  im  Binary image with range = 0-255
	*/
	void thinningGuoHall(cv::Mat& im);

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
	std::vector< float > calculateCharacteristic(cv::Mat& inSkeleton, float inObjectColor);
	std::vector< float > calculateCharacteristicVectorize(cv::Mat& inSkeleton, float inObjectColor);
};

#endif //SKELETON_BUILDER_H