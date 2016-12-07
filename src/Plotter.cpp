#include "Plotter.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Plotter::Plotter( std::string name, float *value, glm::vec2 range, int bufferSize )
	: ControlWithLabel(), mBufferSize( bufferSize ), mValue( value ), mRange( range ), mLineWidth( 1.0f ), mUpdateAlways( false )
{
	setName( name );
	setDrawBack( true );
	setDrawFill( true );
	setDrawOutline( true );
	mValues.resize( mBufferSize, *mValue );
}

void Plotter::setUpdateAlways( bool updateAlways )
{
	mUpdateAlways = updateAlways;
}

void Plotter::setup()
{
	if( !mLabelRef ) {
		mLabelRef = Label::create( mName + "_LABEL", mName + ": " + to_string( *mValue ), FontSize::SMALL );
	}
	mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
	addSubView( mLabelRef );
	View::setup();
}

void Plotter::update()
{
	if( mUpdateAlways || mValues.back() != *mValue ) {
		mValues.push_back( *mValue );
		setLabel( mName + ":" + to_string( *mValue ) );
		while( (int)mValues.size() > mBufferSize ) {
			mValues.pop_front();
		}
		setNeedsDisplay();
	}
	View::update();
}

void Plotter::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addPointGrid( data, color, mHitRect, 8 );
	Control::drawOutline( data, color );
}

void Plotter::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	float _w = 0.0;
	float _h = 0.0;
	float w = getWidth() - ( mPadding.mLeft + mPadding.mRight );
	float h = getHeight() - ( mPadding.mTop + mPadding.mBottom );

	vec2 offset = vec2( mPadding.mLeft, mPadding.mTop );
	int limit = mBufferSize - 1;
	for( int i = 0; i < limit; i++ ) {
		int prevIndex = i;
		int nextIndex = ( prevIndex + 1 );

		vec2 prev = vec2( lmap( (float)prevIndex, 0.0f, (float)limit, _w, w ),
						lmap<float>( mValues[prevIndex], mRange.x, mRange.y, h, _h ) )
			+ offset;

		vec2 next = vec2( lmap( (float)nextIndex, 0.0f, (float)limit, _w, w ),
						lmap<float>( mValues[nextIndex], mRange.x, mRange.y, h, _h ) )
			+ offset;

		addLine( data, color, prev, next, mLineWidth );
	}
}