#include "Dialer.h"
#include "Helpers.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

template <typename T>
DialerT<T>::DialerT( std::string name, T value, T min, T max, Format format )
	: ControlWithLabel(), mUseRef( false ), mValueRef( new T( value ) ), mMin( min ), mMax( max ), mFormat( format ), mCallbackFn( nullptr )
{
	setName( name );
	setValue( value );
}

template <typename T>
DialerT<T>::DialerT( std::string name, T *value, T min, T max, Format format )
	: ControlWithLabel(), mUseRef( true ), mValueRef( value ), mMin( min ), mMax( max ), mFormat( format ), mCallbackFn( nullptr )
{
	setName( name );
	setValue( *value );
}

template <typename T>
DialerT<T>::~DialerT()
{
	if( !mUseRef ) {
		delete mValueRef;
	}
}

template <typename T>
void DialerT<T>::setup()
{
	string maxLengthString = "";
	string minString = toString( mMin, mFormat.mPrecision );
	string maxString = toString( mMax, mFormat.mPrecision );

	if( minString.length() > maxString.length() ) {
		maxLengthString = minString;
	}
	else {
		maxLengthString = "+" + maxString;
	}
	mSetWidth = maxLengthString.length() - 1;
	mNumOfPrecisionZones = maxLengthString.length();

	if( !mFormat.mSign ) {
		//        mSetWidth--;
		mNumOfPrecisionZones--;
		maxLengthString = maxLengthString.substr( 1 );
	}

	size_t dotIndex = maxLengthString.find( "." );
	if( dotIndex == string::npos ) {
		mDotIndex = -1;
		mFormat.mPrecision = 0;
	}
	else {
		mDotIndex = mNumOfPrecisionZones - dotIndex;
	}

	if( !mLabelRef ) {
		mLabelRef = Label::create( mName + "_LABEL", maxLengthString, mFormat.mFontSize );
		addSubView( mLabelRef );
	}
	mLabelWidth = mLabelRef->getStringWidth( maxLengthString );
	mDigitWidth = mLabelWidth / (float)mNumOfPrecisionZones;
	mLabelRef->setOrigin( vec2( mPadding.mLeft, mPadding.mTop ) );
	vec2 size = mLabelRef->getSize();
	setSize( vec2( size.x + mPadding.mLeft + mPadding.mRight, size.y + mPadding.mTop + mPadding.mBottom ) );
	updateLabel();
	View::setup();
}

template <typename T>
void DialerT<T>::update()
{
	if( mUseRef ) {
		T scaledValue = lmap<double>( mValue, 0.0, 1.0, mMin, mMax );
		if( ( *mValueRef ) != scaledValue ) {
			setValue( *mValueRef );
		}
	}
	View::update();
}

template <typename T>
void DialerT<T>::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValue() );
	}
	Control::trigger( recursive );
}

template <typename T>
JsonTree DialerT<T>::save()
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "VALUE", getValue() ) );
	return tree;
}

template <typename T>
void DialerT<T>::load( const ci::JsonTree &data )
{
	if( data.hasChild( "VALUE" ) ) {
		setValue( data.getValueForKey<T>( "VALUE" ) );
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

template <typename T>
void DialerT<T>::setValue( T value )
{
	mValue = lmap<double>( value, mMin, mMax, 0.0, 1.0 );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
}

template <typename T>
T DialerT<T>::getValue()
{
	return *mValueRef;
}

template <typename T>
double DialerT<T>::getNormalizedValue()
{
	return std::max( std::min( mValue, 1.0 ), 0.0 );
}
template <typename T>
void DialerT<T>::updateValueRef()
{
	*mValueRef = lmap<double>( mValue, 0.0, 1.0, mMin, mMax );
}

template <typename T>
void DialerT<T>::updateLabel()
{
	T value = getValue();
	string label = "";
	if( mFormat.mSign ) {
		label += ( value > 0 ? "+" : "-" );
	}

	label += toString( fabs( (float)value ), mFormat.mPrecision, mSetWidth, '0' );

	if( mFormat.mLabel ) {
		label += " " + mName;
	}

	setLabel( label );
}

template <typename T>
DialerT<T> *DialerT<T>::setCallback( const std::function<void( T )> &callback )
{
	mCallbackFn = callback;
	return this;
}

template <typename T>
void DialerT<T>::setMax( T max, bool keepValueTheSame )
{
	setMinAndMax( mMin, max, keepValueTheSame );
}

template <typename T>
T DialerT<T>::getMax()
{
	return mMax;
}

template <typename T>
void DialerT<T>::setMin( T min, bool keepValueTheSame )
{
	setMinAndMax( min, mMax, keepValueTheSame );
}

template <typename T>
T DialerT<T>::getMin()
{
	return mMin;
}

template <typename T>
vec2 DialerT<T>::getMaxAndMin()
{
	return vec2( mMax, mMin );
}

template <typename T>
void DialerT<T>::setMinAndMax( T min, T max, bool keepValueTheSame )
{
	mMax = max;
	mMin = min;

	if( !keepValueTheSame ) {
		setValue( lmap<double>( getNormalizedValue(), 0.0, 1.0, min, max ) );
	}
}

template <typename T>
void DialerT<T>::setValueRef( T *value )
{
	if( !mUseRef ) {
		mUseRef = false;
		delete mValueRef;
	}
	mValueRef = value;
}

template <typename T>
Rectf DialerT<T>::getHitRect()
{
	string label = mLabelRef->getLabel();
	float spacing = mLabelRef->getSpacing() * 0.25;
	int index = std::min( std::max( (float)mCurrentIndex, 0.0f ), (float)mNumOfPrecisionZones - 1.0f );
	float off = index > 0 ? ( mPadding.mLeft * 2.0 - spacing + mLabelRef->getStringWidth( label.substr( 0, index ) ) ) : 0.0;
	float off2 = mPadding.mLeft * 2.0 + mLabelRef->getStringWidth( mLabelRef->getLabel().substr( 0, index + 1 ) );
	Rectf rect( mHitRect.x1 + off, mHitRect.y1, mHitRect.x1 + off2, mHitRect.y2 );
	return rect;
}

template <typename T>
void DialerT<T>::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addRectOutline( data, color, getHitRect() );
	Control::drawOutlineHighlight( data, color );
}

template <typename T>
void DialerT<T>::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

template <typename T>
void DialerT<T>::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addRectOutline( data, color, getHitRect() );
}

template <typename T>
void DialerT<T>::input( const glm::vec2 &pt )
{
	T value = *mValueRef + mZoneMultiplier * ( mHitPoint.y - pt.y );
	setValue( std::max( std::min( value, mMax ), mMin ) );
}

template <typename T>
void DialerT<T>::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		mHitPoint = event.getPos();
		calculatePrecisionZone();
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

template <typename T>
void DialerT<T>::mouseUp( ci::app::MouseEvent &event )
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

template <typename T>
void DialerT<T>::mouseWheel( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		T value = *mValueRef + mZoneMultiplier * ( signbit( event.getWheelIncrement() ) > 0 ? 1.0 : -1.0 );
		setValue( std::max( std::min( value, mMax ), mMin ) );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
}

template <typename T>
vec2 DialerT<T>::getHitPercent( const vec2 &pt )
{
	vec2 hp;
	Rectf r = mHitRect;
	r.offset( getOrigin() );
	r.x1 += mPadding.mLeft;
	r.x2 -= mPadding.mRight;
	r.y1 += mPadding.mTop;
	r.y2 -= mPadding.mBottom;
	hp = ( pt - r.getUpperLeft() ) / r.getSize();
	return hp;
}

template <typename T>
void DialerT<T>::calculatePrecisionZone()
{
	vec2 hp = getHitPercent( mHitPoint );
	hp.x *= mLabelWidth;
	hp.x /= mDigitWidth;
	mCurrentIndex = hp.x;
	mCurrentPrecisionZone = mNumOfPrecisionZones - hp.x;
	if( mCurrentPrecisionZone >= mDotIndex && mDotIndex != -1 ) {
		mCurrentPrecisionZone -= 1;
	}
	mZoneMultiplier = pow( 10.0, mCurrentPrecisionZone - mFormat.mPrecision );
}

template <typename T>
void DialerT<T>::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
		mHitPoint = event.getPos();
		calculatePrecisionZone();
		setNeedsDisplay();
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void DialerT<T>::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
		input( event.getPos() );
		mHitPoint = event.getPos();
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void DialerT<T>::keyDown( ci::app::KeyEvent &event )
{
}

template <typename T>
void DialerT<T>::keyUp( ci::app::KeyEvent &event )
{
}

template class reza::ui::DialerT<int>;
template class reza::ui::DialerT<float>;
template class reza::ui::DialerT<double>;