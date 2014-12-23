#ifndef SKELETON_BUILDER_H
#define SKELETON_BUILDER_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <queue>

#include "Utils.h"

#define min( x, y ) (x) < (y) ? (x) : (y);

class SkeletonBuilder
{
private:
	


	static void ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst) 
	{
		int rows = pSrc.rows;
		int cols = pSrc.cols;
		pSrc.copyTo(pDst);
		for(int i = 0; i < rows; i++) 
		{
			for(int j = 0; j < cols; j++) 
			{
				if(pSrc.at<float>(i, j) == 1.0f) 
				{
				/// get 8 neighbors
				/// calculate C(p)
					int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
					int neighbor1 = (int) pSrc.at<float>( i-1, j);
					int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
					int neighbor3 = (int) pSrc.at<float>( i, j+1);
					int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
					int neighbor5 = (int) pSrc.at<float>( i+1, j);
					int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
					int neighbor7 = (int) pSrc.at<float>( i, j-1);
					int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
							int(~neighbor3 & ( neighbor4 | neighbor5)) +
							int(~neighbor5 & ( neighbor6 | neighbor7)) +
							int(~neighbor7 & ( neighbor0 | neighbor1));
						
					if(C == 1) 
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
						int N = min(N1,N2);
						if ((N == 2) || (N == 3)) 
						{
								/// calculate criteria 3
								int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
								if(c3 == 0) 
								{
										pDst.at<float>( i, j) = 0.0f;
								}
						}
					}
				}
			}
		}
	}

	static void ThinSubiteration2(cv::Mat & pSrc, cv::Mat & pDst) {
    int rows = pSrc.rows;
    int cols = pSrc.cols;
    pSrc.copyTo( pDst);
    for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                    if (pSrc.at<float>( i, j) == 1.0f) {
                            /// get 8 neighbors
                            /// calculate C(p)
                        int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                        int neighbor1 = (int) pSrc.at<float>( i-1, j);
                        int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                        int neighbor3 = (int) pSrc.at<float>( i, j+1);
                        int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                        int neighbor5 = (int) pSrc.at<float>( i+1, j);
                        int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                        int neighbor7 = (int) pSrc.at<float>( i, j-1);
                            int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                                    int(~neighbor3 & ( neighbor4 | neighbor5)) +
                                    int(~neighbor5 & ( neighbor6 | neighbor7)) +
                                    int(~neighbor7 & ( neighbor0 | neighbor1));
                            if(C == 1) {
                                    /// calculate N
                                    int N1 = int(neighbor0 | neighbor1) +
                                            int(neighbor2 | neighbor3) +
                                            int(neighbor4 | neighbor5) +
                                            int(neighbor6 | neighbor7);
                                    int N2 = int(neighbor1 | neighbor2) +
                                            int(neighbor3 | neighbor4) +
                                            int(neighbor5 | neighbor6) +
                                            int(neighbor7 | neighbor0);
                                    int N = min(N1,N2);
                                    if((N == 2) || (N == 3)) {
                                            int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
                                            if(E == 0) {
                                                    pDst.at<float>(i, j) = 0.0f;
                                            }
                                    }
                            }
                    }
            }
    }
}

public:
	/**
	* Perform one thinning iteration.
	* Normally you wouldn't call this function directly from your code.
	*
	* @param  im    Binary image with range = 0-1
	* @param  iter  0=even, 1=odd
	*/
	static void thinningIteration(cv::Mat& im, int iter)
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
	static void thinning(cv::Mat& im)
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

	static void skeleton(cv::Mat & inputarray, cv::Mat & outputarray) {
        bool bDone = false;
        int rows = inputarray.rows;
        int cols = inputarray.cols;

        inputarray.convertTo(inputarray,CV_32FC1);

        inputarray.copyTo(outputarray);

        outputarray.convertTo(outputarray,CV_32FC1);

        /// pad source
        cv::Mat p_enlarged_src = cv::Mat(rows + 2, cols + 2, CV_32FC1);
        for(int i = 0; i < (rows+2); i++) {
            p_enlarged_src.at<float>(i, 0) = 0.0f;
            p_enlarged_src.at<float>( i, cols+1) = 0.0f;
        }
        for(int j = 0; j < (cols+2); j++) {
                p_enlarged_src.at<float>(0, j) = 0.0f;
                p_enlarged_src.at<float>(rows+1, j) = 0.0f;
        }
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if (inputarray.at<float>(i, j) >= 20.0f) {
                                p_enlarged_src.at<float>( i+1, j+1) = 1.0f;
                        }
                        else
                                p_enlarged_src.at<float>( i+1, j+1) = 0.0f;
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
                if(num_non_zero == (rows + 2) * (cols + 2)) {
                        bDone = true;
                }
                /// copy
                p_thinMat2.copyTo(p_enlarged_src);
        }
        // copy result
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        outputarray.at<float>( i, j) = p_enlarged_src.at<float>( i+1, j+1);
                }
        }
	}
	
	struct Node
	{
		int x;
		int y;
		Node* prev;

		Node()
		{
			x = -1;
			y = -1;
		}

		Node(int i, int j, Node* inPrev)
		{
			x = i;
			y = j;
			prev = inPrev;
		}

		bool isPrev(Node* node)
		{
			if (x == -1 && y == -1)
				return false;
			else if ((node->x == prev->x) && (node->y == prev->y))
				return true;
			else
				return prev->isPrev(node);
		}
	};

	static void cycleSearch(cv::Mat& image, int i, int j, int& count)
	{
		std::queue< Node* > theQueue;

		theQueue.push(new Node(i, j, new Node()));

		while (!theQueue.empty())
		{
			Node* theNode = theQueue.front();
			theQueue.pop();
			
			if (theNode->x < 0 || theNode->x >= image.rows || theNode->y < 0 || theNode->y >= image.cols)
				continue;

			float thePx = image.at<float>(theNode->x, theNode->y);

			if (thePx == 0)
				continue;

			if (thePx > 0 && thePx < 1)
			{	
				count++;
				image.at<float>(theNode->x, theNode->y) -= 0.1f;
				continue;
			}

			if (thePx == 1)
				image.at<float>(theNode->x, theNode->y) -= 0.1f;
			
			bool isFound = false;
			for (int k = theNode->x - 1; k <= theNode->x + 1; k++)
			{
				for (int l = theNode->y - 1; l <= theNode->y + 1; l++)
				{
					if (k < 0 || k >= image.rows || l < 0 || l >= image.cols || (k == theNode->x && l == theNode->y))
						continue;

					Node* n = new Node(k, l, theNode);
					if (image.at<float>(k, l) && !theNode->isPrev(n))
					{
						theQueue.push(n);
						isFound = true;
						break;
					}
					else
						delete n;
				}
				if (isFound)
					break;
			}
		}
	}

	static int neighboursCount(cv::Mat& inSkeleton, float inObjectColor,int inRow, int inCol)
	{
		int theNeighboursCount = 0;
		for (int i = inRow - 1; i <= inRow + 1; i++)
			for (int j = inCol - 1; j <= inCol + 1; j++)
			{
				if (i < 0 || j < 0 || i >= inSkeleton.rows || j >= inSkeleton.cols)
					continue;

				if (i == inRow && j == inCol)
					continue;

				if (inSkeleton.at< unsigned char >(i, j) == inObjectColor)
					theNeighboursCount++;
			}
			return theNeighboursCount;
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

		9. Количество скелетных точек выше центра тяжести по y 
		10. Количество скелетных точек ниже центра тяжести по y
		11. Количество скелетных точек левее центра тяжести по x
		12. Количество скелетных точек правее центра тяжести по x
	*/
	static std::vector< float > calculateCharacteristic( cv::Mat& inSkeleton, float inObjectColor )
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

		std::vector< float > theCharacteristics( 9, 0 );

		for( int i = 0; i < theHeight; i++ )
			for( int j = 0; j < theWidth; j++ )
			{
				if (i == 127 && j == 0)
					int a = 0;
				unsigned char thePixel = theBoundedSkeleton.at< unsigned char >( i, j );
				

				if( thePixel > 0 )
				{
					//подсчет соседства
					unsigned char theTop = ( i > 0 ) ? theBoundedSkeleton.at< unsigned char >( i - 1, j ) : 0;
					unsigned char theTopRight = ( i > 0 ) && ( j < theWidth - 1 ) ? theBoundedSkeleton.at< unsigned char >( i - 1, j + 1 ) : 0;
					unsigned char theRight = ( j < theWidth - 1 ) ? theBoundedSkeleton.at< unsigned char >( i, j + 1 ) : 0;
					unsigned char theBottomRight = ( i < theHeight - 1 ) && ( j < theWidth - 1 ) ? theBoundedSkeleton.at< unsigned char >( i + 1, j + 1 ) : 0;
					unsigned char theBottom = ( i < theHeight - 1 ) ? theBoundedSkeleton.at< unsigned char >( i + 1, j ) : 0;
					unsigned char theBottomLeft = ( i < theHeight - 1 ) && ( j > 0 ) ? theBoundedSkeleton.at< unsigned char >( i + 1, j - 1 ) : 0;
					unsigned char theLeft = ( j > 0 ) ? theBoundedSkeleton.at< unsigned char >( i, j - 1 ) : 0;
					unsigned char theTopLeft = ( i > 0 ) && ( j > 0 ) ? theBoundedSkeleton.at< unsigned char >( i - 1, j - 1 ) : 0;

					// Количество точек скелета
					theSkeletonPixelsCount++;

					// Центр тяжести относительно оси OX
					theCharacteristics[ 0 ] += j;
					// Центр тяжести относительно оси OY
					theCharacteristics[ 1 ] += i;
					 
					// Количество скелетных точек, связанных с соседними по горизонтали, к общему количеству скелетных точек:
					if (theLeft == inObjectColor || theRight == inObjectColor)
						theCharacteristics[ 4 ] += 1;
						// Количество скелетных точек, связанных с соседними по вертикали, к общему количеству скелетных точек:
					/*else*/ if (theTop == inObjectColor || theBottom == inObjectColor)
						theCharacteristics[ 5 ] += 1;
					// Количество скелетных точек, связанных с соседними по главной диагонали, к общему количеству скелетных точек:
					/*else*/ if( theTopLeft == inObjectColor || theBottomRight == inObjectColor )
						theCharacteristics[ 6 ] += 1;
					// Количество скелетных точек, связанных с соседними по обратной диагонали, к общему количеству скелетных точек:
					/*else*/ if( theTopRight == inObjectColor || theBottomLeft == inObjectColor )
						theCharacteristics[ 7 ] += 1;

					if (neighboursCount(theBoundedSkeleton, inObjectColor, i, j) == 1)
						theCharacteristics[8]++;
				}
			}

			theCharacteristics[ 0 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 1 ] /= theSkeletonPixelsCount;

			theCharacteristics[ 4 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 5 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 6 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 7 ] /= theSkeletonPixelsCount;

			for( int i = 0; i < theHeight; i++ )
				for( int j = 0; j < theWidth; j++ )
				{
					unsigned char thePixel = theBoundedSkeleton.at< unsigned char >( i, j );

					if( thePixel > 0 )
					{
						// Среднеквадратичное отклонение от центра тяжести отностиельно OX
						theCharacteristics[ 2 ] += ( j - theCharacteristics[ 0 ] ) * ( j - theCharacteristics[ 0 ] );
						// Среднеквадратичное отклонение от центра тяжести отностиельно OY
						theCharacteristics[ 3 ] += ( i - theCharacteristics[ 1 ] ) * ( i - theCharacteristics[ 1 ] );
					}
				}

			theCharacteristics[ 2 ] = sqrtf( theCharacteristics[ 2 ] );
			theCharacteristics[ 2 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 2 ] /= theWidth;

			theCharacteristics[ 3 ] = sqrtf( theCharacteristics[ 3 ] );
			theCharacteristics[ 3 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 3 ] /= theHeight;
		
			theCharacteristics[ 0 ] /= theWidth;
			theCharacteristics[ 1 ] /= theHeight;
			
			return theCharacteristics;
	}
};

#endif //SKELETON_BUILDER_H