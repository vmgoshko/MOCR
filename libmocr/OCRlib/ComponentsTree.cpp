//opencv includes
#include <opencv2/highgui/highgui.hpp>

//OCRlib includes
#include "ComponentsTree.h"
#include "Defs.h"
#include "Utils.h"
#include "SkeletonBuilder.h"

using namespace cv;

namespace {
	void showImage(const char* name, cv::Mat& img)
	{
		cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
		cv::imshow( name, img );
	}
};

Component::Component( const Component* parent, Bound bound ): 
			parent( parent ),
			bound( bound ),
			children( NULL )
{
}
	
Component::Component( const Component& component )
{
	this->parent = component.parent;
	this->bound = component.bound;
	if( component.children )
		this->children = new vector< Component>( *component.children );
	else
		this->children = NULL;

	this->str = component.str;
}

Component::~Component( )
{
	delete children;
}

void Component::setChildren( std::vector< Component >* children )
{
	this->children = children;
}

//Is vertical black line
bool isVBL( Mat& m, int x, const Bound& bound )
{
	for( int i = bound.t; i < bound.b; i++ )
	{
		float px = m.at< float >( i, x );
		//m.at< byte >( i, x ) = 0;
		if ( px > 0 )
			return false;
	}

	return true;
}

//Is horizontal black line
bool isHBL( Mat& m, int y, const Bound& bound )
{
	for( int i = bound.l; i < bound.r; i++ )
	{
		float px = m.at<float>( y, i );
		//m.at< byte >( y, i ) = 50;
		if ( px > 0 )
			return false;
	}

	return true;
}

vector<Bound>* ICE( Mat m, const Bound& bound )
{
	vector< Bound > res;
	//TODO: Implement
	return NULL;
}

vector<Component>* VCE( Mat& m, const Bound& bound, Component* parent )
{
	vector< Component >* res = new vector< Component >();
	
	int x = bound.l;
	int currentStart = -1;

	while( x < bound.r )
	{
		bool theVWL = isVBL( m, x, bound );

		// the last pixel column is scanning
		bool theNextVWL =  x < bound.r - 1 ? isVBL( m, x + 1, bound ) : true;

		if( !theNextVWL && theVWL && currentStart == -1 )
		{
			currentStart = x + 1;
			x++;
			continue;
		}
		
		if( theVWL && currentStart != -1 )
		{
			Bound b( currentStart, x, bound.t, bound.b );
			res->push_back( Component( parent, b ) );
			
			currentStart = -1;
			x--;
		}

		x++;
	}

	if( currentStart != -1 )
	{
		Bound b( currentStart, bound.r, bound.t, bound.b );
		res->push_back( Component( parent, b ) );
	}

	if( res->size() > 1 )
		return res;
	else 
	{
		//ICE( m, res->at( 0 ).getBound() );
		return NULL;
	}
}

vector<Component>* HCE( Mat& m, const Bound& bound, Component* parent  )
{
	vector< Component >* res = new vector< Component >();
	
	int y = bound.t;
	int currentStart = -1;

	while( y < bound.b )
	{
		bool theHWL = isHBL( m, y, bound );

		// the last pixel column is scanning
		bool theNextHWL =  y < bound.b - 1 ? isHBL( m, y + 1, bound ) : true;

		if( !theNextHWL && ( theHWL && currentStart == -1 ) )
		{
			currentStart = y + 1;
			y++;
			continue;
		}
		
		if( theHWL && currentStart != -1 )
		{
			Bound b( bound.l, bound.r, currentStart, y );
			res->push_back( Component( parent, b ) );
			currentStart = -1;
			y--;
		}

		y++;
	}

	if( currentStart != -1 )
	{
		Bound b( bound.l, bound.r, currentStart, bound.b );
		res->push_back( Component( parent, b ) );
	}
	
	if( res->size() > 1 )
		return res;
	else 
	{
		//ICE( m, res->at( 0 ).getBound() );
		return NULL;
	}
}

void ComponentsTree::build( cv::Mat& image )
{
	int w = image.cols;
	int h = image.rows;
	this->m = image;
	Bound bound( 0, w, 0, h );

	root = new Component( NULL, bound ); 

	vector< Component >* children = parse( 1, bound, image, root );

	root->setChildren( children );
}

vector< Component >* ComponentsTree::parse( int level, const Bound& bound, Mat& m, Component* parent )
{
	vector< Component >* res;
	if( level % 2 )
	{
		res = VCE( m, bound, parent );
	}
	else
	{
		res = HCE( m, bound, parent );
	}

	if( !res )
		return NULL;

	for( size_t i = 0; i < res->size(); i++ )
	{
		res->at(i).setChildren( parse( level + 1, res->at(i).getBound(), m, &res->at(i) ) );
	}

	return res;
}

void ComponentsTree::show( Component* component )
{
	static size_t i = 0;

	vector< Component >* children = component->getChildren();
	
	if( !children )
	{	
		char buff[10];
		char filename[] = "%i.jpg";

		itoa( i++, buff, 10 );
		sprintf( buff, filename, i );

		Mat save = submat( m, component->getBound() );

		//SkeletonBuilder::skeleton( save, save );
		showImage( buff, save );
		imwrite( buff, save );

		return;
	}

	for( size_t i = 0; i < children->size(); i++ )
		show( &children->at( i ) );
}


std::vector< Component > ComponentsTree::getLeafs()
{
	std::vector< Component > theLeafs;
	findLeaf( &theLeafs, getRoot() );
	return theLeafs;
}

void ComponentsTree::findLeaf( std::vector< Component >* resultVector, Component* component )
{
	std::vector< Component >* children = component->getChildren();
	
	if( !children )
	{
		//show( component );
		resultVector->push_back( *component );
		return;
	}

	for( size_t i = 0; i < children->size(); i++ )
		findLeaf( resultVector, &children->at( i ) );
}