#include "Helpers.h"
#include "Range.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

template <typename T>
RangeT<T>::RangeT( string name, T valueLow, T valueHigh, T min, T max, Format format )
	: ControlWithLabel(), mUseLowRef( false ), mUseHighRef( false ), mValueLowRef( new T( valueLow ) ), mValueHighRef( new T( valueHigh ) ), mMin( min ), mMax( max ), mHitHigh( false ), mHitCenter( false ), mHitLow( false ), mCallbackFn( nullptr ), mFormat( format ), mStickyEnabled( false )
{
	setName( name );
	setValue( valueLow, valueHigh );
}

template <typename T>
RangeT<T>::RangeT( std::string name, T *valueLow, T *valueHigh, T min, T max, Format format )
	: ControlWithLabel(), mUseLowRef( true ), mUseHighRef( true ), mValueLowRef( valueLow ), mValueHighRef( valueHigh ), mMin( min ), mMax( max ), mHitHigh( false ), mHitCenter( false ), mHitLow( false ), mCallbackFn( nullptr ), mFormat( format ), mStickyEnabled( false )
{
	setName( name );
	setValue( *valueLow, *valueHigh );
}

template <typename T>
RangeT<T>::~RangeT()
{
	if( !mUseLowRef ) {
		delete mValueLowRef;
	}
	if( !mUseHighRef ) {
		delete mValueHighRef;
	}
}

template <typename T>
void RangeT<T>::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName + ":[" + toString( getValueLow(), mFormat.mPrecision ) + "," + toString( getValueHigh(), mFormat.mPrecision ) + "]", FontSize::SMALL );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
		addSubView( mLabelRef );
	}
	View::setup();
}

template <typename T>
void RangeT<T>::update()
{
	if( mUseLowRef ) {
		T scaledValue = lmap<double>( mValueLow, 0.0, 1.0, mMin, mMax );
		if( ( *mValueLowRef ) != scaledValue ) {
			setValueLow( *mValueLowRef );
		}
	}
	if( mUseHighRef ) {
		T scaledValue = lmap<double>( mValueHigh, 0.0, 1.0, mMin, mMax );
		if( ( *mValueHighRef ) != scaledValue ) {
			setValueHigh( *mValueHighRef );
		}
	}
	View::update();
}

template <typename T>
void RangeT<T>::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValueLow(), getValueHigh() );
	}
	Control::trigger( recursive );
}

template <typename T>
JsonTree RangeT<T>::save()
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "LVALUE", getValueLow() ) );
	tree.addChild( JsonTree( "HVALUE", getValueHigh() ) );
	return tree;
}

template <typename T>
void RangeT<T>::load( const ci::JsonTree &data )
{
	if( data.hasChild( "LVALUE" ) && data.hasChild( "HVALUE" ) ) {
		setValue( data.getValueForKey<T>( "LVALUE" ), data.getValueForKey<T>( "HVALUE" ) );
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

template <typename T>
void RangeT<T>::setValue( T valueLow, T valueHigh )
{
	if( mStickyEnabled && mFormat.mSticky ) {
		valueLow = (float)ceil( (double)valueLow / mFormat.mStickyValue ) * mFormat.mStickyValue;
		valueHigh = (float)ceil( (double)valueHigh / mFormat.mStickyValue ) * mFormat.mStickyValue;
	}

	mValueLow = lmap<double>( valueLow, mMin, mMax, 0.0, 1.0 );
	mValueHigh = lmap<double>( valueHigh, mMin, mMax, 0.0, 1.0 );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
}

template <typename T>
void RangeT<T>::setValueLow( T value )
{
	mValueLow = lmap<double>( value, mMin, mMax, 0.0, 1.0 );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
}

template <typename T>
void RangeT<T>::setValueHigh( T value )
{
	mValueHigh = lmap<double>( value, mMin, mMax, 0.0, 1.0 );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
}

template <typename T>
T RangeT<T>::getValueLow()
{
	return *mValueLowRef;
}

template <typename T>
T RangeT<T>::getValueHigh()
{
	return *mValueHighRef;
}

template <typename T>
float RangeT<T>::getNormalizedValueLow()
{
	return std::max( std::min( mValueLow, 1.0 ), 0.0 );
}

template <typename T>
float RangeT<T>::getNormalizedValueHigh()
{
	return std::max( std::min( mValueHigh, 1.0 ), 0.0 );
}

template <typename T>
void RangeT<T>::updateValueRef()
{
	*mValueLowRef = lmap<double>( mValueLow, 0.0, 1.0, mMin, mMax );
	*mValueHighRef = lmap<double>( mValueHigh, 0.0, 1.0, mMin, mMax );
}

template <typename T>
void RangeT<T>::updateLabel()
{
	setLabel( mName + ":[" + toString( getValueLow(), mFormat.mPrecision ) + "," + toString( getValueHigh(), mFormat.mPrecision ) + "]" );
}

template <typename T>
RangeT<T> *RangeT<T>::setCallback( const std::function<void( T, T )> &callback )
{
	mCallbackFn = callback;
	return this;
}

template <typename T>
void RangeT<T>::setMax( T max, bool keepValueTheSame )
{
	setMinAndMax( mMin, max, keepValueTheSame );
}

template <typename T>
T RangeT<T>::getMax()
{
	return mMax;
}

template <typename T>
void RangeT<T>::setMin( T min, bool keepValueTheSame )
{
	setMinAndMax( min, mMax, keepValueTheSame );
}

template <typename T>
T RangeT<T>::getMin()
{
	return mMin;
}

template <typename T>
vec2 RangeT<T>::getMinAndMax()
{
	return vec2( mMin, mMax );
}

template <typename T>
void RangeT<T>::setMinAndMax( T min, T max, bool keepValueTheSame )
{
	mMax = max;
	mMin = min;

	if( !keepValueTheSame ) {
		setValue( lmap<double>( getNormalizedValueLow(), 0.0, 1.0, min, max ),
			lmap<double>( getNormalizedValueHigh(), 0.0, 1.0, min, max ) );
	}
	else {
		setValue( std::max( std::min( *mValueLowRef, mMax ), mMin ), std::max( std::min( *mValueHighRef, mMax ), mMin ) );
	}
}

template <typename T>
void RangeT<T>::setValueLowRef( T *value )
{
	if( !mUseLowRef ) {
		mUseLowRef = false;
		delete mValueLowRef;
	}
	mValueLowRef = value;
}

template <typename T>
void RangeT<T>::setValueHighRef( T *value )
{
	if( !mUseHighRef ) {
		mUseHighRef = false;
		delete mValueHighRef;
	}
	mValueHighRef = value;
}

template <typename T>
void RangeT<T>::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Rectf rect = mHitRect;
	rect.x1 = mHitRect.x1 + mHitRect.getWidth() * getNormalizedValueLow();
	rect.x2 = mHitRect.x1 + mHitRect.getWidth() * getNormalizedValueHigh();
	addRect( data, color, rect );
}

template <typename T>
void RangeT<T>::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawFill( data, color );
}

template <typename T>
void RangeT<T>::input( const glm::vec2 &pt )
{
	vec2 hp = getHitPercent( pt );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );

	if( mHitHigh ) {
		mValueHigh = hp.x;
	}
	else if( mHitLow ) {
		mValueLow = hp.x;
	}
	else if( mHitCenter ) {
		mValueHigh += ( hp.x - mHitPoint );
		mValueLow += ( hp.x - mHitPoint );
		mHitPoint = hp.x;
	}
	else {
		double dvh = abs( mValueHigh - hp.x );
		double dvl = abs( mValueLow - hp.x );
		if( dvh < .05 || hp.x > mValueHigh ) {
			mValueHigh = hp.x;
			mHitHigh = true;
		}
		else if( dvl < .05 || hp.x < mValueLow ) {
			mValueLow = hp.x;
			mHitLow = true;
		}
		else {
			mHitCenter = true;
			mHitPoint = hp.x;
		}
	}

	if( mValueHigh < mValueLow && mHitHigh ) {
		mValueHigh = mHitValueLow;
		mHitHigh = false;
		mHitLow = true;
	}
	else if( mValueLow > mValueHigh && mHitLow ) {
		mValueLow = mHitValueHigh;
		mHitHigh = true;
		mHitLow = false;
	}

	mValueLow = std::max( std::min( mValueLow, 1.0 ), 0.0 );
	mValueHigh = std::max( std::min( mValueHigh, 1.0 ), 0.0 );
	setValue( lmap<double>( mValueLow, 0.0, 1.0, mMin, mMax ), lmap<double>( mValueHigh, 0.0, 1.0, mMin, mMax ) );
}

template <typename T>
void RangeT<T>::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		setState( State::DOWN );
		mHitValueHigh = mValueHigh;
		mHitValueLow = mValueLow;
		input( event.getPos() );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void RangeT<T>::mouseUp( ci::app::MouseEvent &event )
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
	mHitHigh = false;
	mHitLow = false;
	mHitCenter = false;
}

template <typename T>
void RangeT<T>::mouseWheel( ci::app::MouseEvent &event )
{
}

template <typename T>
void RangeT<T>::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void RangeT<T>::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
		mHitValueHigh = mValueHigh;
		mHitValueLow = mValueLow;
		input( event.getPos() );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void RangeT<T>::keyDown( ci::app::KeyEvent &event )
{
	if( event.isShiftDown() ) {
		mStickyEnabled = true;
	}
}

template <typename T>
void RangeT<T>::keyUp( ci::app::KeyEvent &event )
{
	mStickyEnabled = false;
}

template class reza::ui::RangeT<int>;
template class reza::ui::RangeT<float>;
template class reza::ui::RangeT<double>;