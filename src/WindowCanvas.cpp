#include "WindowCanvas.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

WindowCanvas::WindowCanvas( std::string title ) : Canvas( title, nullptr ) { }

void WindowCanvas::setup()
{
    setWindow( createWindow() );
    View::setup();
}

app::WindowRef WindowCanvas::createWindow()
{
    auto window = app::App::get()->createWindow( app::Window::Format().renderer( app::RendererGl::create( app::RendererGl::Options().msaa( 0 ) ) ).size( getSize() ) );
    std::string name = getName();
    std::transform( name.begin(), name.end(), name.begin(), ::toupper );
    window->setTitle( name );
    window->getSignalClose().connect( [ this ] { mValidRef = false; } );
    window->getSignalMove().connect( [ this ] { if( isValid() ) { mWindowOrigin = mWindowRef->getPos(); } } );
    window->getSignalDraw().connect( [ this ] {
        gl::clear( Color::black() );
        gl::setMatricesWindow( this->getSize() );
    } );
    mValidRef = true;
    return window;
}

void WindowCanvas::spawn()
{
    if( !isValid() ) {
        setWindow( createWindow() );
        setPos( mWindowOrigin );
        autoSizeToFitSubviews();
    }
}

void WindowCanvas::close()
{
    if( isValid() ) {
        mWindowRef->close();
        mWindowRef = nullptr;
    }
}

bool WindowCanvas::isValid()
{
    return ( mWindowRef && mWindowRef->isValid() );
}

bool& WindowCanvas::getValidRef()
{
    return mValidRef;
}

vec2 WindowCanvas::getPos()
{
    return ( isValid() == true ? vec2( mWindowRef->getPos() ) : mWindowOrigin );
}

void WindowCanvas::setPos( vec2 pos )
{
    if( isValid() ) {
        mWindowOrigin = pos;
        mWindowRef->setPos( mWindowOrigin );
    }
}

void WindowCanvas::autoSizeToFitSubviews()
{
    View::autoSizeToFitSubviews();
    if( isValid() ) {
        mWindowRef->setSize( getSize() );
    }
}

void WindowCanvas::save( const ci::fs::path &path )
{
    JsonTree tree = View::save();
    tree.addChild( JsonTree( "VALID", isValid() ) );
    vec2 pos = getPos();
    tree.addChild( JsonTree( "XPOS", pos.x ) );
    tree.addChild( JsonTree( "YPOS", pos.y ) );
    tree.write( path );
}

void WindowCanvas::load( const ci::fs::path &path )
{
    if( fs::exists( path ) ) {
        JsonTree tree( loadFile( path ) );
        View::load( tree );
        if( tree.hasChild( "VALID" ) ) {
            if( tree.hasChild( "XPOS" ) && tree.hasChild( "YPOS" ) ) {
                setPos( vec2( tree.getValueForKey<float>( "XPOS" ), tree.getValueForKey<float>( "YPOS" ) ) );
            }
            if( !tree.getValueForKey<bool>( "VALID" ) ) {
                close();
            }
        }
        trigger();
    }
}

