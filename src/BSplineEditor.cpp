#include "BSplineEditor.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

BSplineEditor::BSplineEditor( string name, BSpline2f spline, const Format &format )
	: ControlWithLabel(), mUseRef( false ), mSplineRef( new BSpline2f( spline ) ), mCallbackFn( nullptr ), mFormat( format )
{
	setName( name );
	setSpline( spline );
}

BSplineEditor::~BSplineEditor()
{
	if( !mUseRef ) {
		delete mSplineRef;
	}
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
	}
}

void BSplineEditor::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
		addSubView( mLabelRef );
	}
	if( !mFormat.mTimeRef ) {
		mFormat.mTimeRef = new float( mFormat.mTime );
	}
	View::setup();
}

void BSplineEditor::update()
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

void BSplineEditor::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getSpline() );
	}
	Control::trigger( recursive );
}

JsonTree BSplineEditor::save()
{
	JsonTree tree = View::save();
	JsonTree subtree = JsonTree::makeArray( "POINTS" );
	for( auto &it : mControlPoints ) {
		vec2 mapped = norm( it );
		JsonTree subsubtree;
		subsubtree.addChild( JsonTree( "X", mapped.x ) );
		subsubtree.addChild( JsonTree( "Y", mapped.y ) );
		subtree.addChild( subsubtree );
	}
	if( subtree.getNumChildren() ) {
		tree.addChild( subtree );
	}
	return tree;
}

void BSplineEditor::load( const ci::JsonTree &data )
{
	mControlPoints.clear();
	if( data.hasChild( "POINTS" ) ) {
		auto pts = data.getChild( "POINTS" );
		int total = pts.getNumChildren();
		if( (int)mControlPoints.size() < total ) {
			mControlPoints.resize( total );
		}
		for( int i = 0; i < total; i++ ) {
			auto child = pts.getChild( i );
			mControlPoints[i] = expand( vec2( child.getValueForKey<float>( "X" ), child.getValueForKey<float>( "Y" ) ) );
		}
		updateSplineRef();
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

void BSplineEditor::setSpline( BSpline2f spline )
{
	int total = spline.getNumControlPoints();
	mControlPoints.clear();
	for( int i = 0; i < total; i++ ) {
		mControlPoints.emplace_back( spline.getControlPoint( i ) );
	}
	mOpen = spline.isOpen();
	mLoop = spline.isLoop();
	mDegree = ( total == -1 ) ? 1 : spline.getDegree();
	updateSplineRef( true );
}

void BSplineEditor::setSplineRef( BSpline2f *spline )
{
	if( !mUseRef ) {
		mUseRef = false;
		delete mSplineRef;
	}
	mSplineRef = spline;
	setSpline( *mSplineRef );
}

BSpline2f BSplineEditor::getSpline()
{
	return *mSplineRef;
}

int BSplineEditor::getDegree()
{
	return mDegree;
}

bool BSplineEditor::isLoop()
{
	return mLoop;
}

bool BSplineEditor::isOpen()
{
	return mOpen;
}

void BSplineEditor::setProperties( int degree, bool loop, bool open )
{
	mDegree = degree;
	mLoop = loop;
	mOpen = open;
	updateSplineRef( true );
	trigger();
}

void BSplineEditor::setDegree( int degree )
{
	if( mDegree != degree ) {
		mDegree = degree;
		updateSplineRef( true );
		trigger();
	}
}

void BSplineEditor::setLoop( bool loop )
{
	if( mLoop != loop ) {
		mLoop = loop;
		updateSplineRef( true );
		trigger();
	}
}

void BSplineEditor::setOpen( bool open )
{
	if( mOpen != open ) {
		mOpen = open;
		updateSplineRef( true );
		trigger();
	}
}

void BSplineEditor::setShowTime( bool showTime )
{
	mFormat.mShowTime = showTime;
}

void BSplineEditor::setTime( float Time )
{
	setShowTime( true );
	*mFormat.mTimeRef = Time;
}

void BSplineEditor::setTimeRef( float *TimeRef )
{
	setShowTime( true );
	if( !mFormat.mUseTimeRef ) {
		delete mFormat.mTimeRef;
		mFormat.mUseTimeRef = true;
	}
	mFormat.mTimeRef = TimeRef;
}

void BSplineEditor::updateSplineRef( bool force )
{
	setNeedsDisplay();
	if( !isValid() ) {
		return;
	}

	if( ( mControlPoints.size() != mSplineRef->getNumControlPoints() ) || force ) {
		delete mSplineRef;
		mSplineRef = new BSpline2f( mControlPoints, mDegree, mLoop, mOpen );
	}
	else {
		int total = mControlPoints.size();
		for( int i = 0; i < total; i++ ) {
			mSplineRef->setControlPoint( i, mControlPoints[i] );
		}
	}
}

void BSplineEditor::setCallback( const std::function<void( BSpline2f )> &callback )
{
	mCallbackFn = callback;
}

void BSplineEditor::setMax( vec2 max, bool keepValueTheSame )
{
	setMinAndMax( max, mFormat.mMin, keepValueTheSame );
}

vec2 BSplineEditor::getMax()
{
	return mFormat.mMax;
}

void BSplineEditor::setMin( vec2 min, bool keepValueTheSame )
{
	setMinAndMax( mFormat.mMax, min, keepValueTheSame );
}

vec2 BSplineEditor::getMin()
{
	return mFormat.mMin;
}

void BSplineEditor::setMinAndMax( vec2 min, vec2 max, bool keepValueTheSame )
{
	if( !keepValueTheSame ) {
		for( auto &it : mControlPoints ) {
			float x = lmap<float>( it.x, mFormat.mMin.x, mFormat.mMax.x, min.x, max.x );
			float y = lmap<float>( it.y, mFormat.mMin.y, mFormat.mMax.y, min.y, max.y );
			it.x = std::isnan( x ) ? it.x : x;
			it.y = std::isnan( y ) ? it.y : y;
		}
		updateSplineRef();
		trigger();
	}

	mFormat.mMax = max;
	mFormat.mMin = min;
}

std::vector<RenderData> BSplineEditor::render()
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

void BSplineEditor::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	if( mFormat.mGrid ) {
		addPointGrid( data, color, mHitRect, (float)mFormat.mGridSize );
	}
	Control::drawOutline( data, color );
}

void BSplineEditor::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Control::drawOutline( data, color );
}

void BSplineEditor::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	bool valid = mSplineRef != nullptr && mValid;
	if( valid ) {
		vec2 last = vec2( 0.0 );
		vec2 curr = vec2( 0.0 );
		for( int i = 0; i <= mFormat.mResolution; ++i ) {
			float t = i / (float)mFormat.mResolution;
			curr = map( mSplineRef->getPosition( t ) );
			if( i != 0 ) {
				addLine( data, color, last, curr );
			}
			last = curr;
		}
	}

	for( auto it : mControlPoints ) {
		vec2 curr = map( it );
		addPoint( data, color, curr, 2.0 );
	}

	if( mHitIndex != -1 ) {
		addPoint( data, ColorA( 1.0, 0.0, 0.0, 1.0 ), map( mControlPoints[mHitIndex] ), 3.0 );
	}
	else {
		addPoint( data, mColorClear, vec2( 0.0 ) );
	}

	if( mFormat.mShowTime && valid ) {
		addPoint( data, ColorA( 1.0, 0.0, 0.0, color.a ), map( mSplineRef->getPosition( mFormat.mTime ) ), 3.0 );
	}
	else {
		addPoint( data, mColorClear, vec2( 0.0 ) );
	}
}

void BSplineEditor::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawFill( data, color );
}

void BSplineEditor::input( const ci::app::MouseEvent &event )
{
	vec2 hp = getHitPercent( event.getPos() );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );
	hp.y = std::min( std::max( hp.y, 0.0f ), 1.0f );
	hp = vec2( lmap<float>( hp.x, 0.0, 1.0, mFormat.mMin.x, mFormat.mMax.x ),
		lmap<float>( hp.y, 0.0, 1.0, mFormat.mMin.y, mFormat.mMax.y ) );

	if( event.isMetaDown() || mFormat.mSticky ) {
		if( mFormat.mGrid ) {
			float w = mHitRect.getWidth();
			float h = mHitRect.getHeight();
			float sv = h > w ? h : w;
			sv = mFormat.mGridSize / sv;
			hp.x = ceil( hp.x / sv ) * sv;
			hp.y = ceil( hp.y / sv ) * sv;
		}
		else {
			hp.x = ceil( hp.x / mFormat.mStickyValue ) * mFormat.mStickyValue;
			hp.y = ceil( hp.y / mFormat.mStickyValue ) * mFormat.mStickyValue;
		}
	}

	if( getState() == State::NORMAL || getState() == State::OVER ) {
		mHitIndex = -1;
	}
	else if( mHitIndex == -1 ) {
		float distance = 100000.0;
		int index = -1;
		std::map<float, int> distMap;
		int totalCtrlPts = mControlPoints.size();
		bool addToEnd = true;

		for( int i = 0; i < totalCtrlPts; i++ ) {
			float len = length( hp - mControlPoints[i] );
			distMap[len] = i;
			if( len < distance ) {
				distance = len;
				index = i;
			}
		}

		if( distance < ( length( mFormat.mMax - mFormat.mMin ) * mFormat.mThreshold ) ) {
			mHitIndex = index;
			if( ( event.isRight() || event.isMetaDown() ) ) {
				mControlPoints.erase( mControlPoints.begin() + mHitIndex );
				updateSplineRef();
				mHitIndex = -1;
			}
			addToEnd = false;
		}
		else {
			float thres = 0.1f * length( mFormat.mMax - mFormat.mMin );
			float tpp = 1.0f / float( totalCtrlPts - 1.0f );
			float bestDistance = 1000000;
			float bestTime = -1;
			int bestIndex = -1;
			if( mValid ) {
				for( auto &it : distMap ) {
					int id = it.second;

					int bid = ( id > 0 ) ? ( id - 1 ) : id;
					int eid = ( ( id + 1 ) == totalCtrlPts ) ? id : ( id + 1 );
					if( bid == eid ) {
						continue;
					}
					float startTime = bid * tpp;
					float endTime = eid * tpp;

					for( float t = startTime; t <= endTime; t += 0.01f ) {
						vec2 pt = mSplineRef->getPosition( t );
						float d = length( hp - pt );
						if( d < bestDistance ) {
							bestDistance = d;
							bestTime = t;
							bestIndex = id;
						}
					}

					if( bestDistance < thres ) {
						int insertIndex = bid + 1;
						if( bestTime > ( id * tpp ) ) {
							insertIndex = id + 1;
						}
						mControlPoints.insert( mControlPoints.begin() + insertIndex, hp );
						updateSplineRef();
						addToEnd = false;
						break;
					}
				}
			}
		}

		if( addToEnd ) {
			mHitIndex = mControlPoints.size();
			mControlPoints.push_back( hp );
			updateSplineRef();
		}
	}
	else {
		mControlPoints[mHitIndex] = hp;
		updateSplineRef();
	}
}

void BSplineEditor::mouseDown( ci::app::MouseEvent &event )
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

void BSplineEditor::mouseUp( ci::app::MouseEvent &event )
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

void BSplineEditor::mouseWheel( ci::app::MouseEvent &event )
{
	View::mouseWheel( event );
}

void BSplineEditor::mouseMove( ci::app::MouseEvent &event )
{
	View::mouseMove( event );
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

void BSplineEditor::mouseDrag( ci::app::MouseEvent &event )
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

bool BSplineEditor::isValid()
{
	int cpSize = (int)mControlPoints.size();
	mValid = ( ( cpSize >= 2 ) && ( mDegree > 0 ) && ( mDegree <= ( cpSize - 1 ) ) );
	return mValid;
}

vec2 BSplineEditor::map( const vec2 &pt )
{
	vec2 result;
	result.x = lmap<float>( pt.x, mFormat.mMin.x, mFormat.mMax.x, mHitRect.getUpperLeft().x, mHitRect.getLowerRight().x );
	result.y = lmap<float>( pt.y, mFormat.mMin.y, mFormat.mMax.y, mHitRect.getUpperLeft().y, mHitRect.getLowerRight().y );
	return result;
}

vec2 BSplineEditor::norm( const vec2 &pt )
{
	vec2 result;
	result.x = lmap<float>( pt.x, mFormat.mMin.x, mFormat.mMax.x, 0.0f, 1.0f );
	result.y = lmap<float>( pt.y, mFormat.mMin.y, mFormat.mMax.y, 0.0f, 1.0f );
	return result;
}

vec2 BSplineEditor::expand( const vec2 &pt )
{
	vec2 result;
	result.x = lmap<float>( pt.x, 0.0f, 1.0f, mFormat.mMin.x, mFormat.mMax.x );
	result.y = lmap<float>( pt.y, 0.0f, 1.0f, mFormat.mMin.y, mFormat.mMax.y );
	return result;
}