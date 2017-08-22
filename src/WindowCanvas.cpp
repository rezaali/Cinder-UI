#include "WindowCanvas.h"

#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/wrapper.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

WindowCanvas::WindowCanvas( std::string title )
	: Canvas( title, nullptr )
{
}

void WindowCanvas::setup()
{
	if( mWindowRef == nullptr ) {
		setWindow( createWindow() );
	}
	setDrawBack( false );
	View::setup();
}

void WindowCanvas::update()
{
	if( !isValid() && mWindowRef && mWindowRef->isValid() ) {
		mWindowRef->close();
	}
}

app::WindowRef WindowCanvas::createWindow()
{
	app::WindowRef window = app::App::get()->createWindow( app::Window::Format().renderer( app::RendererGl::create( app::RendererGl::Options().msaa( 0 ) ) ).size( getSize() ) );
	std::string name = getName();
	std::transform( name.begin(), name.end(), name.begin(), ::toupper );
	window->setTitle( name );
	window->getSignalMove().connect( [this] { if( isValid() ) { mWindowOrigin = mWindowRef->getPos(); } } );
	window->getSignalClose().connect( [this] {
		mValidRef = false;
		mWindowRef = nullptr;
	} );
	window->getSignalDraw().connect( [this, window] {
		gl::setMatricesWindow( window->getSize() );
		gl::clear( ColorA::black() );
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
		mRenderData.clear();
		setNeedsDisplay( this, true, true );
		mSetup = false;
		mGlslProgRef = nullptr;
		setupBuffers();
	}
}

void WindowCanvas::close()
{
	if( isValid() ) {
#if defined( CINDER_COCOA ) || defined( CINDER_LINUX )
		mValidRef = false;
#else //  defined( CINDER_COCOA ) || defined( CINDER_LINUX )
		mValidRef = false;
		mWindowRef->close();
		mWindowRef = nullptr;
#endif
	}
}

bool WindowCanvas::isValid()
{
	return mValidRef;
}

bool &WindowCanvas::getValidRef()
{
	return mValidRef;
}

vec2 WindowCanvas::getPos()
{
	if( mValidRef ) {
		return vec2( mWindowRef->getPos() );
	}
	else {
		return mWindowOrigin;
	}
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
		if( mSubViews.size() ) {
			mWindowRef->setSize( getSize() );
		}
		else {
			setSize( vec2( mCanvasWidth, 0 ) );
			mWindowRef->setSize( vec2( mCanvasWidth, 0 ) );
		}
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
		try {
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
		catch( ci::Exception exc ) {
			std::cout << "WINDOW CANVAS LOAD ERROR: " << exc.what() << std::endl;
		}
	}
}
