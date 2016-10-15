#include "Envelope.h"
#include "Helpers.h"

#include "cinder/Easing.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Envelope::Envelope( std::string name, const Format &format )
	: ControlWithLabel(), mFormat( format ), mEasingFn( easeNone )
{
	setName( name );
	setDrawBack( true );
	setDrawFill( true );
	setDrawOutline( true );
}

Envelope::~Envelope()
{
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
	}
}

void Envelope::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
		addSubView( mLabelRef );
	}
	View::setup();
}

void Envelope::update()
{
	if( mFormat.mShowTime && mFormat.mUseTimeRef && mVisible ) {
		float time = *mFormat.mTimeRef;
		if( time != mFormat.mTime ) {
			mFormat.mTime = time;
			setNeedsDisplay();
		}
	}
	View::update();
}

void Envelope::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValue( mFormat.mTime ) );
	}
	Control::trigger( recursive );
}

void Envelope::setMax( float max, bool keepValueTheSame )
{
	setMinAndMax( max, mFormat.mMin, keepValueTheSame );
}

float Envelope::getMax()
{
	return mFormat.mMax;
}

void Envelope::setMin( float min, bool keepValueTheSame )
{
	setMinAndMax( mFormat.mMax, min, keepValueTheSame );
}

float Envelope::getMin()
{
	return mFormat.mMin;
}

void Envelope::setMinAndMax( float min, float max, bool keepValueTheSame )
{
	if( !keepValueTheSame ) {
		for( auto &it : mKeys ) {
			float x = lmap<float>( it, mFormat.mMin, mFormat.mMax, min, max );
			it = std::isnan( x ) ? it : x;
		}
	}
	mFormat.mMax = max;
	mFormat.mMin = min;
}

void Envelope::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Control::drawOutline( data, color );
}

void Envelope::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	vec2 ul = mHitRect.getUpperLeft();
	vec2 lr = mHitRect.getLowerRight();
	vec2 pt, lpt;
	float index = 0;

	map<float, float> map;

	for( int i = 0; i < mTimes.size(); i++ ) {
		map[mTimes[i]] = mKeys[i];
	}

	for( auto &it : map ) {
		pt.x = lmap<float>( it.first, 0.0, 1.0, ul.x, lr.x );
		pt.y = lmap<float>( it.second, mFormat.mMin, mFormat.mMax, lr.y, ul.y );

		if( index > 0 ) {
			addLine( data, color, lpt, pt );
		}
		addPoint( data, color, pt, 2.0 );
		lpt = pt;
		index++;
	}

	if( mHitIndex != -1 ) {
		float hpx = lmap<float>( mTimes[mHitIndex], 0.0, 1.0, ul.x, lr.x );
		float hpy = lmap<float>( mKeys[mHitIndex], mFormat.mMin, mFormat.mMax, lr.y, ul.y );
		addPoint( data, ColorA( 1.0, 0.0, 0.0, color.a ), vec2( hpx, hpy ), 3.0 );
	}
	else {
		addPoint( data, mColorClear, vec2( 0.0 ), 3.0 );
	}

	if( mFormat.mShowTime ) {
		float tpx = lmap<float>( mFormat.mTime, 0.0, 1.0, ul.x, lr.x );
		addLine( data, ColorA( 1.0, 0.0, 0.0, color.a ), vec2( tpx, ul.y ), vec2( tpx, lr.y ) );
		addPoint( data, ColorA( 1.0, 0.0, 0.0, color.a ), vec2( tpx, lmap<float>( getValue( mFormat.mTime ), mFormat.mMin, mFormat.mMax, lr.y, ul.y ) ), 3.0 );
	}
	else {
		addLine( data, mColorClear, vec2( 0.0 ), vec2( 0.0 ) );
	}
}

std::vector<RenderData> Envelope::render()
{
	vector<RenderData> data;
	drawBounds( data, ( mDrawBounds && mVisible ) ? mColorBounds : mColorClear );
	drawBoundsOutline( data, ( mDrawBoundsOutline && mVisible ) ? mColorBoundsOutline : mColorClear );
	drawBack( data, ( mDrawBack && mVisible ) ? mColorBack : mColorClear );
	drawFill( data, ( mDrawFill && mVisible ) ? mColorFill : mColorClear );
	drawFillHighlight( data, ( mDrawFillHighlight && mVisible ) ? mColorFillHighlight : mColorClear );
	drawOutline( data, ( mDrawOutline && mVisible ) ? mColorOutline : mColorClear );
	drawOutlineHighlight( data, ( mDrawOutlineHighlight && mVisible ) ? mColorOutlineHighlight : mColorClear );
	for( int i = data.size(); i < 5994; i++ ) {
		data.emplace_back( RenderData() );
	}
	return data;
}

JsonTree Envelope::save()
{
	JsonTree tree = View::save();
	JsonTree subtree = JsonTree::makeArray( "KEYFRAMES" );
	int total = mTimes.size();
	for( int i = 0; i < total; i++ ) {
		JsonTree subsubtree;
		subsubtree.addChild( JsonTree( "TIME", mTimes[i] ) );
		subsubtree.addChild( JsonTree( "KEY", lmap<float>( mKeys[i], mFormat.mMin, mFormat.mMax, 0.0f, 1.0f ) ) );
		subtree.addChild( subsubtree );
	}
	if( subtree.getNumChildren() ) {
		tree.addChild( subtree );
	}
	return tree;
}

void Envelope::load( const JsonTree &data )
{
	mTimes.clear();
	mKeys.clear();
	if( data.hasChild( "KEYFRAMES" ) ) {
		auto pts = data.getChild( "KEYFRAMES" );
		int total = pts.getNumChildren();
		for( int i = 0; i < total; i++ ) {
			auto child = pts.getChild( i );
			mTimes.emplace_back( child.getValueForKey<float>( "TIME" ) );
			mKeys.emplace_back( lmap<float>( child.getValueForKey<float>( "KEY" ), 0.0, 1.0, mFormat.mMin, mFormat.mMax ) );
		}
		setNeedsDisplay();
		trigger();
	}
	View::load( data );
}

void Envelope::setShowTime( bool showTime )
{
	mFormat.mShowTime = showTime;
}

void Envelope::setTime( float Time )
{
	setShowTime( true );
	*mFormat.mTimeRef = Time;
}

void Envelope::setTimeRef( float *TimeRef )
{
	setShowTime( true );
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
		mFormat.mUseTimeRef = true;
	}
	mFormat.mTimeRef = TimeRef;
}

void Envelope::setEasingFn( std::function<float( float )> easingFn )
{
	mEasingFn = easingFn;
}

void Envelope::addKey( float time, float key )
{
	int index = 0;
	for( auto &it : mTimes ) {
		if( time == it ) {
			mTimes[index] = time;
			mKeys[index] = key;
			return;
		}
		index++;
	}
	mTimes.emplace_back( time );
	mKeys.emplace_back( key );
	setNeedsDisplay();
}

float Envelope::getValue( float time )
{
	if( mKeys.size() == 0 ) {
		return 0.0;
	}
	else if( mKeys.size() == 1 ) {
		return mKeys[0];
	}
	else if( mKeys.size() > 1 ) {
		map<float, float> keyMap;
		for( int i = 0; i < mKeys.size(); i++ ) {
			keyMap[mTimes[i]] = mKeys[i];
		}

		vector<float> times;
		vector<float> keys;

		for( auto &it : keyMap ) {
			times.emplace_back( it.first );
			keys.emplace_back( it.second );
		}

		float t = time;
		if( t > 1.0 ) {
			t = ci::fract( t );
		}
		int bi = -1;
		float bd = 1000;

		for( int i = 0; i < times.size(); i++ ) {
			float d = fabsf( times[i] - t );
			if( d < bd ) {
				bd = d;
				bi = i;
			}
		}

		if( times[bi] == t ) {
			return keys[bi];
		}
		else if( t > times[bi] ) {
			int leftIndex = bi;
			float st = times[leftIndex];

			int rightIndex = ( bi + 1 );
			float et = 0;
			if( rightIndex == times.size() ) {
				rightIndex = 0;
				et = times[rightIndex];
				et += 1.0f;
			}
			else {
				et = times[rightIndex];
			}
			float nt = mEasingFn( lmap<float>( t, st, et, 0.0, 1.0 ) );
			return ( keys[leftIndex] * ( 1.0 - nt ) + keys[rightIndex] * nt );
		}
		else if( t < times[bi] ) {
			int rightIndex = bi;
			float et = times[rightIndex];
			int leftIndex = bi - 1;
			float st = 0;
			if( leftIndex == -1 ) {
				leftIndex = times.size() - 1;
				st = times[leftIndex];
				st -= 1.0f;
			}
			else {
				st = times[leftIndex];
			}
			float nt = mEasingFn( lmap<float>( t, st, et, 0.0, 1.0 ) );
			return ( keys[leftIndex] * ( 1.0 - nt ) + keys[rightIndex] * nt );
		}
	}
	return 0.0;
}

void Envelope::reset()
{
	mHitIndex = -1;
	mKeys.clear();
	mTimes.clear();
	setNeedsDisplay();
}

Envelope *Envelope::setCallback( const std::function<void( float )> &callback )
{
	mCallbackFn = callback;
	return this;
}

void Envelope::updateLabel( float time )
{
	setLabel( mName + ":" + toString( getValue( time ), 3.0 ) );
}

void Envelope::input( const ci::app::MouseEvent &event )
{
	vec2 hp = getHitPercent( event.getPos() );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );
	hp.y = std::min( std::max( hp.y, 0.0f ), 1.0f );

	if( event.isMetaDown() ) {
		hp.x = ceil( hp.x / mFormat.mStickyValue ) * mFormat.mStickyValue;
		hp.y = ceil( hp.y / mFormat.mStickyValue ) * mFormat.mStickyValue;
	}

	if( getState() == State::NORMAL || getState() == State::OVER ) {
		mHitIndex = -1;
	}
	else if( mHitIndex == -1 ) {
		float bd = 1000;
		for( int i = 0; i < mTimes.size(); i++ ) {
			float d = fabsf( mTimes[i] - hp.x );
			if( d < bd ) {
				bd = d;
				mHitIndex = i;
			}
		}

		if( bd > mFormat.mThreshold ) {
			mHitIndex = mTimes.size();
			addKey( hp.x, lmap<float>( hp.y, 1.0, 0.0, mFormat.mMin, mFormat.mMax ) );
			updateLabel( hp.x );
		}

		if( ( event.isRight() || event.isMetaDown() ) && mHitIndex != -1 ) {
			mTimes.erase( mTimes.begin() + mHitIndex );
			mKeys.erase( mKeys.begin() + mHitIndex );
			mHitIndex = -1;
			setNeedsDisplay();
		}
	}
	else {
		mTimes[mHitIndex] = hp.x;
		mKeys[mHitIndex] = lmap<float>( hp.y, 1.0, 0.0, mFormat.mMin, mFormat.mMax );
		updateLabel( hp.x );
		setNeedsDisplay();
	}
}

void Envelope::mouseDown( ci::app::MouseEvent &event )
{
	View::mouseDown( event );
	if( isHit( event.getPos() ) ) {
		mHit = true;
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void Envelope::mouseUp( ci::app::MouseEvent &event )
{
	View::mouseUp( event );
	if( mHit ) {
#ifdef CINDER_COCOA_TOUCH
		setState( State::NORMAL );
#else
		if( isHit( event.getPos() ) ) {
			setState( State::OVER );
		}
		else {
			setState( State::NORMAL );
		}
#endif
		input( event );
		if( (int)mTrigger & (int)Trigger::END ) {
			trigger();
		}
	}
	mHit = false;
}

void Envelope::mouseWheel( ci::app::MouseEvent &event )
{
	View::mouseWheel( event );
}

void Envelope::mouseMove( ci::app::MouseEvent &event )
{
	View::mouseMove( event );
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

void Envelope::mouseDrag( ci::app::MouseEvent &event )
{
	View::mouseDrag( event );
	if( mHit ) {
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}