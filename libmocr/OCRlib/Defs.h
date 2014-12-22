#ifndef DEFS_H
#define DEFS_H

#include <opencv\cv.h>

namespace Config
{
	const int cNeuralNetworkImageHeight = 150;
}

struct Bound 
{
	int l,r,t,b;

	Bound( ) {
		l = r = t = b = 0;
	}

	Bound( int l, int r, int t, int b )
	{
		this->l = l;
		this->r = r;
		this->t = t;
		this->b = b;
	}

	Bound( const Bound& bound )
	{
		l = bound.l;
		r = bound.r;
		t = bound.t;
		b = bound.b;
	}

	bool operator == ( Bound& bound )
	{
		return ( bound.l == l ) && ( bound.r == r ) && ( bound.t == t ) && ( bound.b == b );
	}
};

struct BlackObject
{
	cv::Mat object;
	int minX;
	int maxX;
	int minY;
	int maxY;

	BlackObject() {};
	BlackObject( Bound& b )
	{
		minX = b.t;
		maxX = b.b;
		minY = b.l;
		maxY = b.r;
	}

	void setBounds( Bound& b )
	{
		minX = b.t;
		maxX = b.b;
		minY = b.l;
		maxY = b.r;
	}
};

#endif //DEFS_H