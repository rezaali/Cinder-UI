#include "Helpers.h"
#include "QuaternionOrderer.h"

#include "cinder/Easing.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

QuaternionOrderer::QuaternionOrderer( std::string name, const Format &format )
	: ControlWithLabel(), mFormat( format ), mEasingFn( easeInOutSine )
{
	setName( name );
	setDrawBack( true );
	setDrawFill( true );
	setDrawOutline( true );
}

QuaternionOrderer::~QuaternionOrderer()
{
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
	}
}

void QuaternionOrderer::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
		addSubView( mLabelRef );
	}
	View::setup();
}

void QuaternionOrderer::update()
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

void QuaternionOrderer::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Control::drawOutline( data, color );
}

void QuaternionOrderer::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	vec2 ul = mHitRect.getUpperLeft();
	vec2 lr = mHitRect.getLowerRight();
	float x, qx, qy, qz, qw;
	float lx, lqx, lqy, lqz, lqw;
	float index = 0;

	map<float, quat> map;

	for( int i = 0; i < (int)mTimes.size(); i++ ) {
		map[mTimes[i]] = mQuats[i];
	}

	for( auto &it : map ) {
		x = lmap<float>( it.first, 0.0, 1.0, ul.x, lr.x );
		qx = lmap<float>( it.second.x, -1.0, 1.0, lr.y, ul.y );
		qy = lmap<float>( it.second.y, -1.0, 1.0, lr.y, ul.y );
		qz = lmap<float>( it.second.z, -1.0, 1.0, lr.y, ul.y );
		qw = lmap<float>( it.second.w, -1.0, 1.0, lr.y, ul.y );
		addLine( data, color, vec2( x, ul.y ), vec2( x, lr.y ) );

		if( index > 0 ) {
			addLine( data, color, vec2( lx, lqx ), vec2( x, qx ) );
			addLine( data, color, vec2( lx, lqy ), vec2( x, qy ) );
			addLine( data, color, vec2( lx, lqz ), vec2( x, qz ) );
			addLine( data, color, vec2( lx, lqw ), vec2( x, qw ) );
		}
		lx = x;
		lqx = qx;
		lqy = qy;
		lqz = qz;
		lqw = qw;
		index++;
	}

	if( mHitIndex != -1 ) {
		float hpx = lmap<float>( mTimes[mHitIndex], 0.0, 1.0, ul.x, lr.x );
		addLine( data, ColorA( 1.0, 0.0, 0.0, color.a ), vec2( hpx, ul.y ), vec2( hpx, lr.y ), 2.0 );
	}
	else {
		addLine( data, mColorClear, vec2( 0.0 ), vec2( 0.0 ), 2.0 );
	}

	if( mFormat.mShowTime ) {
		float tpx = lmap<float>( mFormat.mTime, 0.0, 1.0, ul.x, lr.x );
		addLine( data, ColorA( 1.0, 0.0, 0.0, color.a ), vec2( tpx, ul.y ), vec2( tpx, lr.y ) );
	}
	else {
		addLine( data, mColorClear, vec2( 0.0 ), vec2( 0.0 ) );
	}
}

std::vector<RenderData> QuaternionOrderer::render()
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

JsonTree QuaternionOrderer::save()
{
	JsonTree tree = View::save();
	JsonTree subtree = JsonTree::makeArray( "POINTS" );
	int total = mTimes.size();
	for( int i = 0; i < total; i++ ) {
		quat q = mQuats[i];
		JsonTree subsubtree;
		subsubtree.addChild( JsonTree( "T", mTimes[i] ) );
		subsubtree.addChild( JsonTree( "W", q.w ) );
		subsubtree.addChild( JsonTree( "X", q.x ) );
		subsubtree.addChild( JsonTree( "Y", q.y ) );
		subsubtree.addChild( JsonTree( "Z", q.z ) );
		subtree.addChild( subsubtree );
	}
	if( subtree.getNumChildren() ) {
		tree.addChild( subtree );
	}
	return tree;
}

void QuaternionOrderer::load( const JsonTree &data )
{
	mTimes.clear();
	mQuats.clear();
	if( data.hasChild( "POINTS" ) ) {
		auto pts = data.getChild( "POINTS" );
		int total = pts.getNumChildren();
		for( int i = 0; i < total; i++ ) {
			auto child = pts.getChild( i );
			mTimes.emplace_back( child.getValueForKey<float>( "T" ) );
			mQuats.emplace_back( quat( child.getValueForKey<float>( "W" ),
				child.getValueForKey<float>( "X" ),
				child.getValueForKey<float>( "Y" ),
				child.getValueForKey<float>( "Z" ) ) );
		}
		setNeedsDisplay();
		trigger();
	}
	View::load( data );
}

void QuaternionOrderer::setShowTime( bool showTime )
{
	mFormat.mShowTime = showTime;
}

void QuaternionOrderer::setTime( float Time )
{
	setShowTime( true );
	*mFormat.mTimeRef = Time;
}

void QuaternionOrderer::setTimeRef( float *TimeRef )
{
	setShowTime( true );
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
		mFormat.mUseTimeRef = true;
	}
	mFormat.mTimeRef = TimeRef;
}

void QuaternionOrderer::setEasingFn( std::function<float( float )> easingFn )
{
	mEasingFn = easingFn;
}

void QuaternionOrderer::addQuat( float time, glm::quat quat )
{
	int index = 0;
	for( auto &it : mTimes ) {
		if( time == it ) {
			mTimes[index] = time;
			mQuats[index] = quat;
			return;
		}
		index++;
	}
	mTimes.emplace_back( time );
	mQuats.emplace_back( quat );
	setNeedsDisplay();
}

quat QuaternionOrderer::getQuat( float time )
{
	if( mQuats.size() == 0 ) {
		return quat();
	}
	else if( mQuats.size() == 1 ) {
		return mQuats[0];
	}
	else if( mQuats.size() > 1 ) {
		map<float, quat> quatMap;
		for( int i = 0; i < (int)mQuats.size(); i++ ) {
			quatMap[mTimes[i]] = mQuats[i];
		}

		vector<float> times;
		vector<quat> quats;

		for( auto &it : quatMap ) {
			times.emplace_back( it.first );
			quats.emplace_back( it.second );
		}

		float t = time;
		if( t > 1.0 ) {
			t = ci::fract( t );
		}
		int bi = -1;
		float bd = 1000;

		for( int i = 0; i < (int)times.size(); i++ ) {
			float d = fabsf( times[i] - t );
			if( d < bd ) {
				bd = d;
				bi = i;
			}
		}

		if( times[bi] == t ) {
			return quats[bi];
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
			float nt = lmap<float>( t, st, et, 0.0, 1.0 );
			return glm::slerp( quats[leftIndex], quats[rightIndex], mEasingFn( nt ) );
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
			float nt = lmap<float>( t, st, et, 0.0, 1.0 );
			return glm::slerp( quats[leftIndex], quats[rightIndex], mEasingFn( nt ) );
		}
	}
	return quat();
}

void QuaternionOrderer::reset()
{
	mHitIndex = -1;
	mQuats.clear();
	mTimes.clear();
	setNeedsDisplay();
}

void QuaternionOrderer::input( const ci::app::MouseEvent &event )
{
	vec2 hp = getHitPercent( event.getPos() );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );

	if( event.isMetaDown() ) {
		hp.x = ceil( hp.x / mFormat.mStickyValue ) * mFormat.mStickyValue;
	}

	if( getState() == State::NORMAL || getState() == State::OVER ) {
		mHitIndex = -1;
	}
	else if( mHitIndex == -1 ) {
		float bd = 1000;
		for( int i = 0; i < (int)mTimes.size(); i++ ) {
			float d = fabsf( mTimes[i] - hp.x );
			if( d < bd ) {
				bd = d;
				mHitIndex = i;
			}
		}

		if( bd > mFormat.mThreshold ) {
			mHitIndex = -1;
		}

		if( ( event.isRight() || event.isMetaDown() ) && mHitIndex != -1 ) {
			mTimes.erase( mTimes.begin() + mHitIndex );
			mQuats.erase( mQuats.begin() + mHitIndex );
			mHitIndex = -1;
			setNeedsDisplay();
		}
	}
	else {
		mTimes[mHitIndex] = hp.x;
		setNeedsDisplay();
	}
}

void QuaternionOrderer::mouseDown( ci::app::MouseEvent &event )
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

void QuaternionOrderer::mouseUp( ci::app::MouseEvent &event )
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

void QuaternionOrderer::mouseWheel( ci::app::MouseEvent &event )
{
	View::mouseWheel( event );
}

void QuaternionOrderer::mouseMove( ci::app::MouseEvent &event )
{
	View::mouseMove( event );
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

void QuaternionOrderer::mouseDrag( ci::app::MouseEvent &event )
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