#include "Button.h"
#include "Globals.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

Button::Format::Format()
{
    fontSize( FontSize::SMALL ).label( true ).align( Alignment::RIGHT ).size( ButtonDimension );
}

Button::Button( std::string name, bool value, const Format &format ) : ControlWithLabel(), mUseRef( false ), mValueRef( new bool( value ) ), mValue( value ), mFormat( format ), mCallbackFn( nullptr )
{
    setName( name );
    setSize( vec2( mFormat.mSize, mFormat.mSize ) );
    setValue( value );
}

Button::Button( std::string name, bool *value, const Format &format ) : ControlWithLabel(), mUseRef( true ), mValueRef( value ), mValue( value ), mFormat( format ), mCallbackFn( nullptr )
{
    setName( name );
    setSize( vec2( mFormat.mSize, mFormat.mSize ) );
    setValue( *value );
}

Button::~Button()
{
    if( !mUseRef )
    {
        delete mValueRef;
    }
}

bool Button::isHit( const glm::vec2 &pt )
{
    if( mVisible )
    {
        if( mLabelRef )
        {
            return ( Control::isHit( pt ) || mLabelRef->isHit( pt ) );
        }
        return Control::isHit( pt );
    }
    return false; 
}

void Button::changeState()
{
    switch ( mState )
    {
        case State::NORMAL:
        {
            setDrawOutline( true );
            setDrawFill( mValue );
            setDrawBack( true );
            setDrawFillHighLight( false );
            setDrawOutlineHighLight( false );
        }
            break;
            
        case State::DOWN:
        {
            setDrawOutline( true );
            setDrawFill( mValue );
            setDrawBack( true );
            setDrawFillHighLight( true );
            setDrawOutlineHighLight( true );
        }
            break;
            
        case State::OVER:
        {
            setDrawOutline( true );
            setDrawFill( mValue );
            setDrawBack( true );
            setDrawFillHighLight( false );
            setDrawOutlineHighLight( true );
        }
            break;
            
        case State::DISABLED:
        {
            setDrawOutline( false );
            setDrawFill( mValue );
            setDrawBack( true );
            setDrawFillHighLight( false );
            setDrawOutlineHighLight( false );
        }
            break;
            
        default:
            break;
    }
    if( mLabelRef ) mLabelRef->setState( mState );
}

void Button::setup()
{
    if( !mLabelRef && mFormat.mLabel )
    {
        mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
        vec2 labelPosition;
        switch ( mFormat.mAlign )
        {
            default:
            case Alignment::NONE:
            case Alignment::RIGHT:
            {
                labelPosition = vec2( getWidth(), ( getHeight() - mLabelRef->getHeight() )*0.5f );
            }
                break;

            case Alignment::LEFT:
            {
                labelPosition = vec2( 0.0f, ( getHeight() - mLabelRef->getHeight() )*0.5f );
                mHitRect.offset( vec2( mLabelRef->getWidth(), 0.0f ) );
            }
                break;

            case Alignment::CENTER:
            {
                vec2 ls = mLabelRef->getSize();
                vec2 sz = getSize();
                if( ls.x > sz.x || ls.y > sz.y ) {
                    labelPosition = vec2( mPadding.mLeft, mPadding.mTop );
                    setSize( ls + vec2( mPadding.mLeft + mPadding.mRight, mPadding.mTop + mPadding.mBottom ) );
                }
                else {
                    labelPosition = ( sz - ls ) * 0.5f;
                }
            }
                break;

            case Alignment::BOTTOM:
            {
                labelPosition = vec2( 0.0f, getHeight() );
            }
                break;

            case Alignment::TOP:
            {
                labelPosition = vec2( 0.0f, 0.0f );
                mHitRect.offset( vec2( 0.0f, mLabelRef->getHeight() ) );
            }
                break;
        }
        mLabelRef->setOrigin( labelPosition );
        addSubView( mLabelRef );
    }
    
    View::setup();
}

void Button::update()
{
    if( mUseRef )
    {
        if( (*mValueRef) != mValue )
        {
            setValue( *mValueRef );
        }
    }
    
    View::update();
}

void Button::trigger( bool recursive )
{
    if( mCallbackFn )
    {
        mCallbackFn( getValue() );
    }
    
    Control::trigger( recursive );
}

bool Button::getValue()
{
    return mValue;
}

void Button::setValue( bool value )
{
    mValue = value;
    *mValueRef = mValue;
    setDrawFill( mValue );
    setNeedsDisplay();
}

void Button::setValuePtr( bool *value )
{
    if( !mUseRef )
    {
        mUseRef = false;
        delete mValueRef;
    }
    mValueRef = value;
    setValue( *value );
}

void Button::toggleValue()
{
    setValue( !getValue() );
}

Button* Button::setCallback( const std::function<void(bool)> &callback )
{
    mCallbackFn = callback;
    return this; 
}

#if defined( CINDER_COCOA_TOUCH )
void touchesBegan( ci::app::TouchEvent &event )
{
    
}

void touchesMoved( ci::app::TouchEvent &event )
{
    
}

void touchesEnded( ci::app::TouchEvent &event )
{
    
}

#else
void Button::mouseDown( ci::app::MouseEvent &event )
{
    if( isHit( event.getPos() ) )
    {
        mHit = true;
        setState( State::DOWN );
        toggleValue();
        if( (int)mTrigger & (int)Trigger::BEGIN )
        {
            trigger();
        }
    }
    else
    {
        setState( State::NORMAL );
    }
    
    View::mouseDown( event );
}

void Button::mouseUp( ci::app::MouseEvent &event )
{
    if( mHit )
    {
#if defined( CINDER_COCOA_TOUCH )
        setState( State::DOWN );
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
        toggleValue();
        if( (int)mTrigger & (int)Trigger::END )
        {
            trigger(); 
        }
    }
    else
    {
        setState( State::NORMAL );
    }
    mHit = false;

    View::mouseUp( event );
}

void Button::mouseMove( ci::app::MouseEvent &event )
{
    if( isHit( event.getPos() ) )
    {
        setState( State::OVER );
    }
    else
    {
        setState( State::NORMAL );
    }

    View::mouseMove( event );
}

void Button::mouseDrag( ci::app::MouseEvent &event )
{
    if( mHit )
    {
        if( isHit( event.getPos() ) )
        {
            setState( State::DOWN );
        }
        else
        {
            mHit = false;
            setState( State::NORMAL );
            toggleValue();
            if( (int)mTrigger & (int)Trigger::END)
            {
                trigger();
            }
        }
    }
    
    View::mouseDrag( event );
}

void Button::keyDown( ci::app::KeyEvent &event )
{
    if( isBindedToKeyEvent( event ) && !mKeyHit )
    {
        mKeyHit = true;
        toggleValue();
        if( (int)mTrigger & (int)Trigger::BEGIN )
        {
            trigger();
        }
    }
}

void Button::keyUp( ci::app::KeyEvent &event )
{
    if( mKeyHit )
    {
        mKeyHit = false;
        toggleValue();
        if( (int)mTrigger & (int)Trigger::END )
        {
            trigger();
        }
    }
}

#endif