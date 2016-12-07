#include "Spectrum.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Spectrum::Spectrum( std::string name, int bufferSize, float *values, glm::vec2 range )
	: Graph( name, bufferSize, nullptr, values, vec2( 0.0, 1.0 ), range )
{
	mXValues = new float[mBufferSize];
	for( int i = 0; i < mBufferSize; i++ ) {
		mXValues[i] = lmap<float>( (float)i, 0.0, mBufferSize - 1.0f, 0.0f, 1.0f );
	}
	setClosed( false );
	setDrawFill( true );
}

Spectrum::~Spectrum()
{
	delete[] mXValues;
}

void Spectrum::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addPointGrid( data, color, mHitRect, 8 );
	Control::drawOutline( data, color );
}

void Spectrum::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	float _w = 0.0;
	float _h = 0.0;
	float w = getWidth() - ( mPadding.mLeft + mPadding.mRight );
	float h = getHeight() - ( mPadding.mTop + mPadding.mBottom );

	vec2 offset = vec2( mPadding.mLeft, mPadding.mTop );

	for( int i = 0; i < mBufferSize - 1; i++ ) {
		int prevIndex = i;
		int nextIndex = ( prevIndex + 1 ) % mBufferSize;

		vec2 prev = vec2( lmap<float>( mXValues[prevIndex], mXRange.x, mXRange.y, _w, w ),
						lmap<float>( mYValues[prevIndex], mYRange.x, mYRange.y, _h, h ) )
			+ offset;

		vec2 prevFloor = vec2( lmap<float>( mXValues[prevIndex], mXRange.x, mXRange.y, _w, w ), h ) + offset;

		vec2 next = vec2( lmap<float>( mXValues[nextIndex], mXRange.x, mXRange.y, _w, w ),
						lmap<float>( mYValues[nextIndex], mYRange.x, mYRange.y, _h, h ) )
			+ offset;

		vec2 nextFloor = vec2( lmap<float>( mXValues[nextIndex], mXRange.x, mXRange.y, _w, w ), h ) + offset;

		addTriangle( data, color, prevFloor, prev, next );
		addTriangle( data, color, next, nextFloor, prevFloor );
	}
}
