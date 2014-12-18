#include <vector>
#include <string>
#include "ComponentsTree.h"
#include "Utils.h"
#include <algorithm>

class Structurer
{
public:
	Structurer() 
	{
		currMode = -1;
		prevLine = NULL;
	}
	
	std::string structure( cv::Mat img, std::vector< Component > leafs, std::vector< std::string > strs )
	{
		if( strs.size() == 0 )
			return std::string();
		
		std::string resultStr;

		// Equals
		for( int i = 0; i < leafs.size() - 1; i++ )
		{
			BlackObject curr = bound( &submat( img, leafs[ i ].getBound( ) ), 0 );
			BlackObject next = bound( &submat( img, leafs[ i + 1 ].getBound( ) ), 0 );
			shiftBounds( curr, next, leafs, i );

			if( strs[i] == "-" && strs[i+1] == "-" )
			{
				strs.erase( strs.begin() + (i + 1) );
				leafs.erase( leafs.begin() + (i + 1) );
				strs[i] = "=";
				Bound b;
				b.l = curr.minY;
				b.r = curr.maxY;
				b.t = curr.minX;
				b.b = next.maxX;

				Component c( leafs[i].getParent(), b );
				leafs[i] = c;
			}
		}

		// Summ
		for( int i = 1; i < leafs.size() - 1; i++ )
		{
			// check for sigma
			if( strs[i] == "sigma_big")
			{
				int sumBeginIndex = i;
				int sumEndIndex = i;

				// UNDER
				std::vector< Component > underSumLeafs;
				std::vector< std::string > underSumStrs;
				
				BlackObject sum = bound( &submat( img, leafs[ i ].getBound( ) ), 0 );
				shiftBounds( sum, BlackObject(), leafs, i );

				int j = i - 1;
				BlackObject prev = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
				shiftBounds( prev, BlackObject(), leafs, j );

				int minX = img.rows;
				while ( j >= 0 && isUnder( sum, prev ) )
				{
					if ( minX > prev.minX ) 
						minX = prev.minX;

					underSumLeafs.push_back( leafs[j] );
					underSumStrs.push_back( strs[j] );

					j -= 1;
					
					if( j >= 0 )
					{
						prev = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
						shiftBounds( prev, BlackObject(), leafs, j );
					}
				}

				sumBeginIndex = j + 1;

				std::reverse( underSumLeafs.begin(), underSumLeafs.end() );
				std::reverse( underSumStrs.begin(), underSumStrs.end() );

				std::string underSumStr = structure( img, underSumLeafs, underSumStrs );

				// BELOW
				std::vector< Component > belowSumLeafs;
				std::vector< std::string > belowSumStrs;
				
				j = i + 1;
				BlackObject next = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
				shiftBounds( next, BlackObject(), leafs, j );

				int maxX = 0;
				while ( j < leafs.size() && isBelow( sum, next ) )
				{
					if ( maxX < prev.maxX ) 
						maxX = prev.maxX;

					belowSumLeafs.push_back( leafs[j] );
					belowSumStrs.push_back( strs[j] );

					j += 1;
					
					if( j < leafs.size() )
					{
						next = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
						shiftBounds( next, BlackObject(), leafs, j );
					}
				}

				sumEndIndex = j - 1;

				std::string belowSumStr = structure( img, belowSumLeafs, belowSumStrs );

				if( !underSumStr.empty() && !belowSumStr.empty() )
				{
					std::string sumStr = "\\sum_{" + belowSumStr + "}^{" + underSumStr + "}";
					
					strs.erase( strs.begin() + sumBeginIndex, strs.begin() + sumEndIndex + 1);
					
					if( sumBeginIndex < strs.size() )
						strs.insert( strs.begin() + sumBeginIndex, sumStr );
					else
						strs.push_back( sumStr );

					Bound b;
					b.l = sum.minY;
					b.r = sum.maxY;
					b.t = minX;
					b.b = maxX;

					Component c( leafs[i].getParent(), b );
					leafs.erase( leafs.begin() + sumBeginIndex, leafs.begin() + sumEndIndex + 1 );

					if( sumBeginIndex < leafs.size() )
						leafs.insert( leafs.begin() + sumBeginIndex, c );
					else
						leafs.push_back( c );
				}
			}
		}

		// Fractal
		for( int i = 1; i < leafs.size() - 1; i++ )
		{
			// check for fractal 
			if( strs[i] == "-")
			{
				int fracBeginIndex = i;
				int fracEndIndex = i;
				// UNDER
				std::vector< Component > underFracLeafs;
				std::vector< std::string > underFracStrs;
				
				BlackObject fractal = bound( &submat( img, leafs[ i ].getBound( ) ), 0 );
				shiftBounds( fractal, BlackObject(), leafs, i );

				int j = i - 1;
				BlackObject prev = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
				shiftBounds( prev, BlackObject(), leafs, j );

				int minX = img.rows;
				while ( j >= 0 && isUnder( fractal, prev ) )
				{
					if ( minX > prev.minX ) 
						minX = prev.minX;

					underFracLeafs.push_back( leafs[j] );
					underFracStrs.push_back( strs[j] );

					j -= 1;
					
					if( j >= 0 )
					{
						prev = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
						shiftBounds( prev, BlackObject(), leafs, j );
					}
				}

				fracBeginIndex = j + 1;

				std::reverse( underFracLeafs.begin(), underFracLeafs.end() );
				std::reverse( underFracStrs.begin(), underFracStrs.end() );

				std::string underFracStr = structure( img, underFracLeafs, underFracStrs );

				// BELOW
				std::vector< Component > belowFracLeafs;
				std::vector< std::string > belowFracStrs;
				
				j = i + 1;
				BlackObject next = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
				shiftBounds( next, BlackObject(), leafs, j );

				int maxX = 0;
				while ( j < leafs.size() && isBelow( fractal, next ) )
				{
					if ( maxX < next.maxX ) 
						maxX = next.maxX;

					belowFracLeafs.push_back( leafs[j] );
					belowFracStrs.push_back( strs[j] );

					j += 1;
					
					if( j < leafs.size() )
					{
						next = bound( &submat( img, leafs[ j ].getBound( ) ), 0 );
						shiftBounds( next, BlackObject(), leafs, j );
					}
				}

				fracEndIndex = j - 1;

				std::string belowFracStr = structure( img, belowFracLeafs, belowFracStrs );

				if( !underFracStr.empty() && !belowFracStr.empty() )
				{
					std::string fracStr = "\\frac{" + underFracStr + "}{" + belowFracStr + "}";
					
					strs.erase( strs.begin() + fracBeginIndex, strs.begin() + fracEndIndex + 1);
					
					if( fracBeginIndex < strs.size() )
						strs.insert( strs.begin() + fracBeginIndex, fracStr );
					else
						strs.push_back( fracStr );

					Bound b;
					b.l = fractal.minY;
					b.r = fractal.maxY;
					b.t = minX;
					b.b = maxX;

					Component c( leafs[i].getParent(), b );
					leafs.erase( leafs.begin() + fracBeginIndex, leafs.begin() + fracEndIndex + 1 );

					if( fracBeginIndex < leafs.size() )
						leafs.insert( leafs.begin() + fracBeginIndex, c );
					else
						leafs.push_back( c );
				}
			}
		}

		for( int i = 0; i < leafs.size(); )
		{
			BlackObject currRow;
			BlackObject nextRow;
 			int currRowBegin = i;
			bool hascurrRow = parseRow( img, leafs, strs, currRowBegin, currRow );
			prevLine = &currRow;
			int nextRowBegin = currRowBegin;
			bool hasNextRow = parseRow( img, leafs, strs, nextRowBegin, nextRow );
		
			if( !hasNextRow )
			{
				std::string row;
				for( int j = i; j < currRowBegin; j++ )
					row += strs[j];
				
				i = nextRowBegin;

				resultStr += row;
				prevLine = NULL;
				continue;
			}

			if( isSuperscript( currRow, nextRow ) )
			{
				std::string base;
				for( int j = i; j < currRowBegin; j++ )
					base += strs[i];

				std::string pow;
				for( int j = currRowBegin; j < nextRowBegin; j++ )
					pow += strs[j];

				i = nextRowBegin;

				resultStr += base + "^{" + pow + "}";
				prevLine = &nextRow;
				continue;
			}

			if( isSubscript( currRow, nextRow ) )
			{
				std::string base;
				for( int j = i; j < currRowBegin; j++ )
					base += strs[j];

				std::string index;
				for( int j = currRowBegin; j < nextRowBegin; j++ )
					index += strs[j];

				i = nextRowBegin;

				resultStr += base + "_{" + index + "}";
				prevLine = &nextRow;
				continue;
			}

			// the last chance...
			for( int j = i; j < currRowBegin; j++ )
				resultStr += strs[j];
				
			i = currRowBegin;
			prevLine = &currRow;
		}

		return resultStr;
	}

	bool parseRow( cv::Mat& img, std::vector< Component >& leafs, std::vector< std::string >& strs, int &i, BlackObject& bo )
	{
		if( i >= strs.size() )
			return false;

		std::vector< BlackObject > row;

		BlackObject curr = bound( &submat( img, leafs[ i ].getBound( ) ), 0 );
		shiftBounds( curr, BlackObject(), leafs, i );
		row.push_back( curr );
		
		BlackObject next;
		if ( i + 1 < strs.size() )
			next = bound( &submat( img, leafs[ i + 1 ].getBound( ) ), 0 );
		else
		{
			i += 1;
			Bound b( row[0].minY, row[0].maxY, row[0].minX, row[0].maxX );
			bo.setBounds( b );
			return true;
		}

		shiftBounds( BlackObject(), next, leafs, i );
		i += 1;

		while( i < strs.size() - 1 && isRow( curr, next ) )
		{
			if( prevLine && !isRow( next, *prevLine ) )
			{
				row.push_back( next );
				isRow( curr, next );
				curr = bound( &submat( img, leafs[ i ].getBound( ) ), 0 );
				next = bound( &submat( img, leafs[ i + 1 ].getBound( ) ), 0 );
				shiftBounds( curr, next, leafs, i );
				i += 1;
			} 
			else
				break;
		}

		Bound b( row[0].minY, row[0].maxY, row[0].minX, row[0].maxX );
		for( int j = 1; j < row.size(); j++ )
		{
			if( b.l > row[j].minY )
				b.l = row[j].minY;

			if( b.r < row[j].maxY )
				b.r = row[j].maxY;
			
			if( b.t > row[j].minX )
				b.t = row[j].minX;

			if( b.b < row[j].maxX )
				b.b = row[j].maxX;
		}

		bo.setBounds( b );
		return true;
	}
private:
	bool isRow(BlackObject& curr, BlackObject& next )
	{
		return !isSubscript( curr, next ) && 
				!isSuperscript( curr, next ) && 
				isIntersectingIntervals( curr.minX, curr.maxX, next.minX, next.maxX );
	}

	bool isIntersectingIntervals( int a, int b, int c, int d ) 
	{
		return ( a <= c && c <= b ) ||
			( a <= d && d <= b ) ||
			( c <= a && a <= d ) ||
			( c <= b && b <= d );
	}

	bool isSuperscript( BlackObject& curr, BlackObject& next )
	{
		int shift = floor( ( curr.maxX - curr.minX ) * 0.3f );
		return next.maxX < curr.maxX - shift && next.maxX > curr.minX && next.minX < curr.minX;
	}

	bool isSubscript( BlackObject& curr, BlackObject& next )
	{
		int shift = floor( ( curr.maxX - curr.minX ) * 0.3f );
		return next.minX > curr.minX + shift && next.minX < curr.maxX && next.maxX > curr.maxX;
	}

	/*
		  next
		--------
		  curr
	*/
	bool isUnder( BlackObject& curr, BlackObject& next )
	{
		int delta = floor( (curr.maxY - curr.minY) * 0.1f );
		return ( next.maxY < curr.maxY + delta ) && ( next.minY > curr.minY - delta ) &&
			( next.maxX < curr.minX );
	}

	/*
		  curr
		--------
		  next
	*/
	bool isBelow( BlackObject& curr, BlackObject& next )
	{
		int delta = floor( (curr.maxY - curr.minY) * 0.1f );
		return ( next.maxY < curr.maxY + delta ) && ( next.minY > curr.minY - delta ) &&
			( next.minX > curr.maxX );
	}

	void shiftBounds(BlackObject& curr, BlackObject& next, std::vector<Component>& leafs, int i)
	{
			curr.minX += leafs[i].getBound().t;
			curr.maxX += leafs[i].getBound().t;
			curr.minY += leafs[i].getBound().l;
			curr.maxY += leafs[i].getBound().l;

			if( i + 1 >= leafs.size() )
				return;

			next.minX += leafs[i+1].getBound().t;
			next.maxX += leafs[i+1].getBound().t;
			next.minY += leafs[i+1].getBound().l;
			next.maxY += leafs[i+1].getBound().l;
	}

	void setMode( int mode )
	{
		prevMode = currMode;
		currMode = mode;
	}

	int currMode;
	int prevMode;
	std::string currStr;
	BlackObject* prevLine;
};