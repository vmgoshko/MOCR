#ifndef SKELETON_BUILDER_H
#define SKELETON_BUILDER_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

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
		cv::Mat theBoundedSkeleton = bound( &inSkeleton, 1 ).object;
		
		int theHeight = theBoundedSkeleton.rows;
		int theWidth = theBoundedSkeleton.cols;

		int theSkeletonPixelsCount = 0;
		int theHorizontalJoinCount = 0;
		int theVerticalJoinCount = 0;
		int theMainDiagonalJoinCount = 0;
		int theSideDiagonalJoinCount = 0;

		std::vector< float > theCharacteristics( 8, 0 );

		for( int i = 0; i < theHeight; i++ )
			for( int j = 0; j < theWidth; j++ )
			{
				float thePixel = theBoundedSkeleton.at< float >( i, j );
				if( thePixel > 0 )
				{
					//подсчет соседства
					float theTop = ( i > 0 ) ? theBoundedSkeleton.at< float >( i - 1, j ) : 0;
					float theTopRight = ( i > 0 ) && ( j < theWidth - 1 ) ? theBoundedSkeleton.at< float >( i - 1, j + 1 ) : 0;
					float theRight = ( j < theWidth - 1 ) ? theBoundedSkeleton.at< float >( i, j + 1 ) : 0;
					float theBottomRight = ( i < theHeight - 1 ) && ( j < theWidth - 1 ) ? theBoundedSkeleton.at< float >( i + 1, j + 1 ) : 0;
					float theBottom = ( i < theHeight - 1 ) ? theBoundedSkeleton.at< float >( i + 1, j ) : 0;
					float theBottomLeft = ( i < theHeight - 1 ) && ( j > 0 ) ? theBoundedSkeleton.at< float >( i + 1, j - 1 ) : 0;
					float theLeft = ( j > 0 ) ? theBoundedSkeleton.at< float >( i, j - 1 ) : 0;
					float theTopLeft = ( i > 0 ) && ( j > 0 ) ? theBoundedSkeleton.at< float >( i - 1, j - 1 ) : 0;

					// Количество точек скелета
					theSkeletonPixelsCount++;

					// Центр тяжести относительно оси OX
					theCharacteristics[ 0 ] += j;
					// Центр тяжести относительно оси OY
					theCharacteristics[ 1 ] += i;
					
					// Количество скелетных точек, связанных с соседними по горизонтали, к общему количеству скелетных точек:
					if( theLeft == inObjectColor || theRight == inObjectColor )
						theCharacteristics[ 4 ] += 1;
					// Количество скелетных точек, связанных с соседними по вертикали, к общему количеству скелетных точек:
					if( theTop == inObjectColor || theBottom == inObjectColor )
						theCharacteristics[ 5 ] += 1;
					// Количество скелетных точек, связанных с соседними по главной диагонали, к общему количеству скелетных точек:
					if( theTopLeft == inObjectColor || theBottomRight == inObjectColor )
						theCharacteristics[ 6 ] += 1;
					// Количество скелетных точек, связанных с соседними по обратной диагонали, к общему количеству скелетных точек:
					if( theTopRight == inObjectColor || theBottomLeft == inObjectColor )
						theCharacteristics[ 7 ] += 1;
				}
			}

			theCharacteristics[ 0 ] /= theSkeletonPixelsCount;
			//theCharacteristics[ 0 ] /= theWidth;

			theCharacteristics[ 1 ] /= theSkeletonPixelsCount;
			//theCharacteristics[ 1 ] /= theHeight;

			theCharacteristics[ 4 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 5 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 6 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 7 ] /= theSkeletonPixelsCount;

			for( int i = 0; i < theHeight; i++ )
				for( int j = 0; j < theWidth; j++ )
				{
					float thePixel = theBoundedSkeleton.at< float >( i, j );

					if( thePixel > 0 )
					{
					/*	if( i / (float)theSkeletonPixelsCount < theCharacteristics[ 0 ] )
							theCharacteristics[ 8 ] += 1;
						else
							theCharacteristics[ 9 ] += 1;

						if( j / (float)theSkeletonPixelsCount < theCharacteristics[ 1 ] )
							theCharacteristics[ 10 ] += 1;
						else
							theCharacteristics[ 11 ] += 1;*/

						// Среднеквадратичное отклонение от центра тяжести отностиельно OX
						theCharacteristics[ 2 ] += ( j - theCharacteristics[ 0 ] ) * ( j - theCharacteristics[ 0 ] );
						// Среднеквадратичное отклонение от центра тяжести отностиельно OY
						theCharacteristics[ 3 ] += ( i - theCharacteristics[ 1 ] ) * ( i - theCharacteristics[ 1 ] );
					}
				}

			theCharacteristics[ 2 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 2 ] = sqrtf( theCharacteristics[ 2 ] );

			theCharacteristics[ 3 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 3 ] = sqrtf( theCharacteristics[ 3 ] );

				/*
			theCharacteristics[ 8 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 9 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 10 ] /= theSkeletonPixelsCount;
			theCharacteristics[ 11 ] /= theSkeletonPixelsCount;
			*/
			return theCharacteristics;
	}
};

#endif //SKELETON_BUILDER_H