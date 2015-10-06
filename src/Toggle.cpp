#include "Toggle.h"
#include "Label.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

Toggle::Toggle( std::string name, bool value, const Format &format ) : Button( name, value, format ) { }

Toggle::Toggle( std::string name, bool *value, const Format &format ) : Button( name, value, format ) { }

JsonTree Toggle::save()
{
    JsonTree tree = View::save();
    tree.addChild( JsonTree( "VALUE",  getValue() ) );
    return tree;
}

void Toggle::load( const ci::JsonTree &data )
{
    if( data.hasChild( "VALUE" ) )
    {
        setValue( data.getValueForKey<bool>( "VALUE" ) );
        trigger( true );
    }
    View::load( data );
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
void Toggle::mouseDown( ci::app::MouseEvent &event )
{
    if( isHit( event.getPos() ) )
    {
        mHit = true;
        setState( State::DOWN );
    }
    else
    {
        setState( State::NORMAL );
    }
}

void Toggle::mouseUp( ci::app::MouseEvent &event )
{
    if( mHit && isHit( event.getPos() ) )
    {
        setValue( !getValue() );
#if defined( CINDER_COCOA_TOUCH )
        setState( State::NORMAL );
#else
        setState( State::OVER );
#endif
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
}

void Toggle::mouseWheel( ci::app::MouseEvent &event )
{
    
}

void Toggle::mouseMove( ci::app::MouseEvent &event )
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

void Toggle::mouseDrag( ci::app::MouseEvent &event )
{
    if( mHit )
    {
        setState( State::DOWN );
    }
    else
    {
        setState( State::NORMAL );
    }
}

void Toggle::keyDown( ci::app::KeyEvent &event )
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

void Toggle::keyUp( ci::app::KeyEvent &event )
{
    if( mKeyHit )
    {
        mKeyHit = false;
    }
}


#endif