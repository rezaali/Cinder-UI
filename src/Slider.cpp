#include "Slider.h"
#include "Helpers.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

template<typename T>
SliderT<T>::SliderT( string name, T value, T min, T max, Format format ) : ControlWithLabel(), mUseRef(false), mValueRef(new T(value)), mMin(min), mMax(max), mCallbackFn(nullptr), mFormat(format)
{
    setName( name );
    setValue( value );
}

template<typename T>
SliderT<T>::SliderT( std::string name, T *value, T min, T max, Format format ) : ControlWithLabel(), mUseRef(true), mValueRef(value), mMin(min), mMax(max), mCallbackFn(nullptr), mFormat(format)
{
    setName( name );
    setValue( *value );
}

template<typename T>
SliderT<T>::~SliderT()
{
    if( !mUseRef )
    {
        delete mValueRef;
    }
}

template<typename T>
void SliderT<T>::setup()
{
    if( !mLabelRef & mFormat.mLabel )
    {
        mLabelRef = Label::create( mName + "_LABEL", mName + ": " + toString( getValue(), mFormat.mPrecision ), FontSize::SMALL );
        addSubView( mLabelRef );
        mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
    }
    View::setup();    
}

template<typename T>
void SliderT<T>::update()
{
    if( mUseRef )
    {
        T scaledValue = lmap<double>( mValue, 0.0, 1.0, mMin, mMax );
        if( (*mValueRef) != scaledValue )
        {
            setValue( *mValueRef );
        }
    }
    View::update();
}

template<typename T>
void SliderT<T>::trigger( bool recursive )
{
    if( mCallbackFn )
    {
        mCallbackFn( getValue() );
    }
    Control::trigger( recursive );
}

template<typename T>
JsonTree SliderT<T>::save()
{
    JsonTree tree = View::save();
    tree.addChild( JsonTree( "VALUE",  isnan( getValue() ) ? mMin : getValue() ) );
    return tree;
}

template<typename T>
void SliderT<T>::load( const ci::JsonTree &data )
{
    if( data.hasChild( "VALUE" ) )
    {
        setValue( data.getValueForKey<T>("VALUE") );
        trigger();
    }
    View::load( data );
}

template<typename T>
void SliderT<T>::setValue( T value )
{
    mValue = lmap<double>( value, mMin, mMax, 0.0, 1.0 );
    updateValueRef();
    updateLabel();
    setNeedsDisplay();
}

template<typename T>
T SliderT<T>::getValue()
{
    return *mValueRef;
}

template<typename T>
double SliderT<T>::getNormalizedValue()
{
    return max( min( mValue, 1.0 ), 0.0 );
}

template<typename T>
void SliderT<T>::updateValueRef()
{
    *mValueRef = lmap<double>( mValue, 0.0, 1.0, mMin, mMax );
}

template<typename T>
void SliderT<T>::updateLabel()
{
    setLabel( mName + ":" + toString( getValue(), mFormat.mPrecision ) );
}

template<typename T>
SliderT<T>* SliderT<T>::setCallback( const std::function<void(T)> &callback )
{
    mCallbackFn = callback;
    return this; 
}

template<typename T>
void SliderT<T>::setMax( T max, bool keepValueTheSame )
{
    setMinAndMax( max, mMin, keepValueTheSame );
}

template<typename T>
T SliderT<T>::getMax()
{
    return mMax;
}

template<typename T>
void SliderT<T>::setMin( T min, bool keepValueTheSame )
{
    setMinAndMax( mMax, min, keepValueTheSame );
}

template<typename T>
T SliderT<T>::getMin()
{
    return mMin;
}

template<typename T>
vec2 SliderT<T>::getMaxAndMin()
{
    return vec2( mMax, mMin );
}

template<typename T>
void SliderT<T>::setMinAndMax( T min, T max, bool keepValueTheSame )
{
    mMax = max;
    mMin = min;
    
    if( !keepValueTheSame )
    {
        setValue( lmap<double>( getNormalizedValue(), 0.0, 1.0, min, max ) );
    }
}

template<typename T>
void SliderT<T>::setValueRef( T *value )
{
    if( !mUseRef )
    {
        mUseRef = false;
        delete mValueRef;
    }
    mValueRef = value;
}

template<typename T>
void SliderT<T>::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
    Rectf rect = mHitRect;
    if( mFormat.mCrossFader )
    {
        float h = mHitRect.getHeight();
        float hh = h*0.5;
        rect.x2 = rect.x1 + ( rect.getWidth() - h ) * getNormalizedValue() + hh;
        rect.x1 = rect.x2 - hh;
        rect.x2 = rect.x1 + h;
    }
    else
    {
        rect.x2 = rect.x1 + rect.getWidth()*getNormalizedValue();
    }
    addRect( data, color, rect );
}

template<typename T>
void SliderT<T>::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
    drawFill( data, color );
}

template<typename T>
void SliderT<T>::input( ci::app::MouseEvent &event )
{
    vec2 hp = getHitPercent( event.getPos() );
    hp.x = min( max( hp.x, 0.0f ), 1.0f );

    float value = lmap<double>( hp.x, 0.0, 1.0, mMin, mMax );
    if( mFormat.mSticky || event.isMetaDown() )
    {
        value = ceil( value / mFormat.mStickyValue ) * mFormat.mStickyValue;
    }
    
    setValue( value );
}

template<typename T>
void SliderT<T>::mouseDown( ci::app::MouseEvent &event )
{
    
    if( isHit( event.getPos() ) )
    {
        mHit = true;
        setState( State::DOWN );
        input( event );
        if( (int)mTrigger & (int)Trigger::BEGIN )
        {
            trigger();
        }
    }
    else
    {
        setState( State::NORMAL );
    }
}

template<typename T>
void SliderT<T>::mouseUp( ci::app::MouseEvent &event )
{
    if( mHit )
    {
#ifdef CINDER_COCOA_TOUCH
        setState( State::NORMAL );
#else
        if( isHit( event.getPos() ) )
        {
            setState( State::OVER );
        }
        else
        {
            setState( State::NORMAL );
        }
#endif
        if( (int)mTrigger & (int)Trigger::END )
        {
            trigger();
        }
    }
    mHit = false;
}

template<typename T>
void SliderT<T>::mouseWheel( ci::app::MouseEvent &event )
{
    
}

template<typename T>
void SliderT<T>::mouseMove( ci::app::MouseEvent &event )
{
    if( isHit( event.getPos() ) )
    {
        setState( State::OVER );
    }
    else
    {
        setState( State::NORMAL );
    }
}

template<typename T>
void SliderT<T>::mouseDrag( ci::app::MouseEvent &event )
{
    if( mHit )
    {
        setState( State::DOWN );
        input( event );
        if( (int)mTrigger & (int)Trigger::CHANGE )
        {
            trigger();
        }
    }
    else
    {
        setState( State::NORMAL );
    }
}

template<typename T>
void SliderT<T>::keyDown( ci::app::KeyEvent &event )
{
   
}

template<typename T>
void SliderT<T>::keyUp( ci::app::KeyEvent &event )
{

}

template class reza::ui::SliderT<int>;
template class reza::ui::SliderT<float>;
template class reza::ui::SliderT<double>;