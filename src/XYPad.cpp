#include "XYPad.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

XYPad::XYPad( string name, vec2 value, const Format &format )
	: ControlWithLabel(), mUseRef( false ), mValueRef( new vec2( value ) ), mCallbackFn( nullptr ), mFormat( format ), mStickyEnabled( false )
{
	setName( name );
	setValue( value );
}

XYPad::XYPad( string name, vec2 *value, const Format &format )
	: ControlWithLabel(), mUseRef( true ), mValueRef( value ), mCallbackFn( nullptr ), mFormat( format ), mStickyEnabled( false )
{
	setName( name );
	setValue( *value );
}

XYPad::~XYPad()
{
	if( !mUseRef ) {
		delete mValueRef;
	}
}

void XYPad::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName + ":" + to_string( getValue().x ), mFormat.mFontSize );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
		addSubView( mLabelRef );
	}
	View::setup();
}

void XYPad::update()
{
	if( mUseRef && mVisible ) {
		vec2 scaledValue = vec2( lmap<float>( mValue.x, 0.0, 1.0, mFormat.mMin.x, mFormat.mMax.x ),
			lmap<float>( mValue.y, 0.0, 1.0, mFormat.mMin.y, mFormat.mMax.y ) );
		if( ( *mValueRef ) != scaledValue ) {
			setValue( *mValueRef );
		}
	}
	View::update();
}

void XYPad::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValue() );
	}
	Control::trigger( recursive );
}

JsonTree XYPad::save()
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "XVALUE", getValue().x ) );
	tree.addChild( JsonTree( "YVALUE", getValue().y ) );
	return tree;
}

void XYPad::load( const ci::JsonTree &data )
{
	if( data.hasChild( "XVALUE" ) && data.hasChild( "YVALUE" ) ) {
		setValue( vec2( data.getValueForKey<float>( "XVALUE" ),
			data.getValueForKey<float>( "YVALUE" ) ) );
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

void XYPad::setValue( vec2 value )
{
	mValue = vec2( lmap<float>( value.x, mFormat.mMin.x, mFormat.mMax.x, 0.0, 1.0 ),
		lmap<float>( value.y, mFormat.mMin.y, mFormat.mMax.y, 0.0, 1.0 ) );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
}

vec2 XYPad::getValue()
{
	return *mValueRef;
}

vec2 XYPad::getNormalizedValue()
{
	return vec2( std::min( std::max( mValue.x, 0.0f ), 1.0f ), std::min( std::max( mValue.y, 0.0f ), 1.0f ) );
}

void XYPad::updateValueRef()
{
	*mValueRef = vec2( lmap<float>( mValue.x, 0.0, 1.0, mFormat.mMin.x, mFormat.mMax.x ),
		lmap<float>( mValue.y, 0.0, 1.0, mFormat.mMin.y, mFormat.mMax.y ) );
}

void XYPad::updateLabel()
{
	setLabel( mName + ":[" + to_string( getValue().x ) + "," + to_string( getValue().y ) + "]" );
}

XYPad *XYPad::setCallback( const std::function<void( vec2 )> &callback )
{
	mCallbackFn = callback;
	return this;
}

void XYPad::setMax( vec2 max, bool keepValueTheSame )
{
	setMinAndMax( max, mFormat.mMin, keepValueTheSame );
}

vec2 XYPad::getMax()
{
	return mFormat.mMax;
}

void XYPad::setMin( vec2 min, bool keepValueTheSame )
{
	setMinAndMax( mFormat.mMax, min, keepValueTheSame );
}

vec2 XYPad::getMin()
{
	return mFormat.mMin;
}

void XYPad::setMinAndMax( vec2 min, vec2 max, bool keepValueTheSame )
{
	mFormat.mMax = max;
	mFormat.mMin = min;

	if( !keepValueTheSame ) {
		vec2 nval = getNormalizedValue();
		setValue( vec2( lmap<float>( nval.x, 0.0, 1.0, min.x, max.x ),
			lmap<float>( nval.y, 0.0, 1.0, min.y, max.y ) ) );
	}
}

void XYPad::setValueRef( vec2 *value )
{
	if( !mUseRef ) {
		mUseRef = false;
		delete mValueRef;
	}
	mValueRef = value;
}

void XYPad::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	vec2 value = getNormalizedValue();

	addLine( data, color, mHitRect.getUpperLeft() + vec2( 0.0f, value.y * mHitRect.getHeight() ), mHitRect.getUpperLeft() + vec2( mHitRect.getWidth(), value.y * mHitRect.getHeight() ) );

	addLine( data, color, mHitRect.getUpperLeft() + vec2( value.x * mHitRect.getWidth(), 0.0f ), mHitRect.getUpperLeft() + vec2( value.x * mHitRect.getWidth(), mHitRect.getHeight() ) );

	addPointGrid( data, color, mHitRect, 8 );
	Control::drawOutline( data, color );
}

void XYPad::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Control::drawOutline( data, color );
}

void XYPad::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	vec2 value = getNormalizedValue();

	addLine( data, color, mHitRect.getUpperLeft() + vec2( 0.0f, value.y * mHitRect.getHeight() ), mHitRect.getUpperLeft() + vec2( mHitRect.getWidth(), value.y * mHitRect.getHeight() ) );

	addLine( data, color, mHitRect.getUpperLeft() + vec2( value.x * mHitRect.getWidth(), 0.0f ), mHitRect.getUpperLeft() + vec2( value.x * mHitRect.getWidth(), mHitRect.getHeight() ) );

	addPoint( data, color, mHitRect.getUpperLeft() + value * mHitRect.getSize(), 4.0f );
}

void XYPad::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawFill( data, color );
}

void XYPad::input( const glm::vec2 &pt )
{
	vec2 hp = getHitPercent( pt );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );
	hp.y = std::min( std::max( hp.y, 0.0f ), 1.0f );
	hp = vec2( lmap<float>( hp.x, 0.0, 1.0, mFormat.mMin.x, mFormat.mMax.x ),
		lmap<float>( hp.y, 0.0, 1.0, mFormat.mMin.y, mFormat.mMax.y ) );

	if( mStickyEnabled && mFormat.mSticky ) {
		hp.x = ceil( hp.x / mFormat.mStickyValue ) * mFormat.mStickyValue;
		hp.y = ceil( hp.y / mFormat.mStickyValue ) * mFormat.mStickyValue;
	}

	setValue( hp );
}

void XYPad::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		setState( State::DOWN );
		input( event.getPos() );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void XYPad::mouseUp( ci::app::MouseEvent &event )
{
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
		if( (int)mTrigger & (int)Trigger::END ) {
			trigger();
		}
	}
	mHit = false;
}

void XYPad::mouseWheel( ci::app::MouseEvent &event )
{
}

void XYPad::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

void XYPad::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
		input( event.getPos() );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void XYPad::keyDown( ci::app::KeyEvent &event )
{
	if( event.isShiftDown() ) {
		mStickyEnabled = true;
	}
}

void XYPad::keyUp( ci::app::KeyEvent &event )
{
	mStickyEnabled = false;
}