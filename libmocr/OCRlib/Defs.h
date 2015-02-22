#ifndef DEFS_H
#define DEFS_H

#include <opencv\cv.h>

#define MAX_ASSIGN(x,y) if( (x) < (y) ) x = y;
#define MIN_ASSIGN(x,y) if( (x) > (y) ) x = y;

namespace Config
{
	const int cNeuralNetworkImageHeight = 150;
	const int cNeighboursDistance = 10;
	const int cMaxDistance = 5;
	const int cMaxLineLength = 2 * cNeuralNetworkImageHeight;
}

enum NNToolsType
{
	Pattern = 0,
	Skeleton
};

enum SkeletonType
{
	Raster = 0,
	Vector,
	Raster_Vector,
	None
};

enum LeafRecognizerType
{
	TwoStepsRecognizer = 0,
	NeuralNetworkRecognizer,
};

struct Configuration
{
	char neuralNetworkFile[256];
	LeafRecognizerType leafRecognizerType = LeafRecognizerType::TwoStepsRecognizer;
	NNToolsType nnToolsType = NNToolsType::Skeleton;
	SkeletonType skeletonType = SkeletonType::Vector;

	Configuration()
	{
		strcpy(neuralNetworkFile, "nn");
	}
	Configuration(const char file[], LeafRecognizerType lrt, NNToolsType nntt, SkeletonType st )
	{
		strcpy_s(neuralNetworkFile, file);
		leafRecognizerType = lrt;
		skeletonType = st;
		nnToolsType = nntt;
	}
};

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