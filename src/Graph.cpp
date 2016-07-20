#include "Graph.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Graph::Graph( std::string name, int bufferSize, float *xValues, float *yValues, glm::vec2 xRange, glm::vec2 yRange )
	: Control(), mBufferSize( bufferSize ), mXValues( xValues ), mYValues( yValues ), mXRange( xRange ), mYRange( yRange ), mLineWidth( 1.0f ), mClosed( true )
{
	setName( name );
	setDrawBack( true );
	setDrawFill( true );
	setDrawOutline( true );
}

void Graph::setClosed( bool closed )
{
	if( mClosed != closed ) {
		mClosed = closed;
		setNeedsDisplay();
	}
}

void Graph::setBufferX( float *values )
{
	mXValues = values;
}

void Graph::setBufferY( float *values )
{
	mYValues = values;
}

void Graph::setBuffers( float *xvalues, float *yvalues )
{
	mXValues = xvalues;
	mYValues = yvalues;
}

void Graph::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addPointGrid( data, color, mHitRect, 8 );
	addCenterLines( data, color, mHitRect );
	Control::drawOutline( data, color );
}

void Graph::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	float _w = 0.0;
	float _h = 0.0;
	float w = getWidth() - ( mPadding.mLeft + mPadding.mRight );
	float h = getHeight() - ( mPadding.mTop + mPadding.mBottom );

	vec2 offset = vec2( mPadding.mLeft, mPadding.mTop );
	int limit = mClosed ? mBufferSize : mBufferSize - 1;
	for( int i = 0; i < limit; i++ ) {
		int prevIndex = i;
		int nextIndex = ( prevIndex + 1 ) % mBufferSize;

		vec2 prev = vec2( lmap<float>( mXValues[prevIndex], mXRange.x, mXRange.y, _w, w ),
						lmap<float>( mYValues[prevIndex], mYRange.x, mYRange.y, _h, h ) )
			+ offset;

		vec2 next = vec2( lmap<float>( mXValues[nextIndex], mXRange.x, mXRange.y, _w, w ),
						lmap<float>( mYValues[nextIndex], mYRange.x, mYRange.y, _h, h ) )
			+ offset;

		addLine( data, color, prev, next, mLineWidth );
	}
}