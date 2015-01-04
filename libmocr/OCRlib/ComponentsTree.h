#ifndef COMPONENTSTREE_H
#define COMPONENTSTREE_H

#include <vector>
#include <string>
#include <opencv\cv.h>
#include "Defs.h"

class Component 
{
private:
	const Component* parent;
	Bound bound;
	std::vector< Component >* children;
	std::string str;

public:
	Component( const Component* parent, Bound bound );
	Component( const Component& component );
	
	~Component( );

	void setChildren( std::vector< Component >* children );
	void setString( std::string str ) { this->str = str; };
	
	std::string getString() const { return this->str; }; 
	Bound getBound() const { return bound; }
	const Component* getParent() { return parent; }
	std::vector< Component >* getChildren() { return children; }

	Component& operator=(const Component& component);
};

class ComponentsTree 
{
private:
	Component* root;
	cv::Mat m;
	cv::Mat srcImg;

	void findLeaf( std::vector< Component >* resultVector, Component* component );
public:
	std::vector< Component >* parse( int level, const Bound& bound, cv::Mat& m, Component* parent );
	void build( cv::Mat& image );
	void show( Component* component );

	Component* getRoot() { return root; }
	std::vector< Component > getLeafs();
	void setSourceImage( cv::Mat& img ) { srcImg = img; }
	cv::Mat getSourceImage() { return srcImg; }
	cv::Mat getImage() { return m; }; 
};

#endif