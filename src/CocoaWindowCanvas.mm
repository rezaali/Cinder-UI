#include "CocoaWindowCanvas.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/app/cocoa/AppImplMac.h"
#import "cinder/app/cocoa/CinderViewMac.h"
#include "cinder/gl/wrapper.h"
#import <Cocoa/Cocoa.h>

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

using namespace ci;
using namespace ci::app;

CocoaWindowCanvas::CocoaWindowCanvas( std::string name, std::string title )
	: Canvas( name, nullptr ), mWindowTitle( title.length() ? title : name )
{
}

void CocoaWindowCanvas::setup()
{
	if( !isValid() ) {
		setWindow( createWindow() );
	}
	View::setup();
}

cinder::app::AppCocoaView *CocoaWindowCanvas::createApp()
{
	cinder::app::AppBase::Settings settings;
	settings.setHighDensityDisplayEnabled();
	settings.setQuitOnLastWindowCloseEnabled( false );
	cinder::app::RendererGlRef renderer = cinder::app::RendererGl::create( cinder::app::RendererGl::Options().msaa( 0 ) );
	UIApp::initialize( &settings, renderer );

	mView = [[CinderViewMac alloc] initWithFrame:NSMakeRect( 0, 0, 212, 212 ) renderer:renderer sharedRenderer:nil appReceivesEvents:YES highDensityDisplay:YES enableMultiTouch:NO];
	[mView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

	mApp = new UIApp();
	mApp->setupCinderView( mView );
	mApp->launch();
	return mApp;
}

app::WindowRef CocoaWindowCanvas::createWindow()
{
	if( mApp == nullptr ) {
		createApp();
	}

	vec2 size = getSize();
	NSRect frame = NSMakeRect( 0, 0, size.x, size.y );
	NSUInteger styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
	mWindow = [[NSWindow alloc] initWithContentRect:frame styleMask:styleMask backing:NSBackingStoreBuffered defer:false];
	[mWindow setContentView:mView];
	[mWindow setAcceptsMouseMovedEvents:YES];
	[mWindow makeKeyAndOrderFront:nil];

	mWindowRef = mApp->getWindow();
	mWindowRef->setTitle( mWindowTitle );

	[[NSNotificationCenter defaultCenter] addObserverForName:NSWindowWillCloseNotification
													  object:this->mWindow
													   queue:nil
												  usingBlock:^( NSNotification *note ) {
													  mValidRef = false;
												  }];

	[[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidMoveNotification
													  object:this->mWindow
													   queue:nil
												  usingBlock:^( NSNotification *note ) {
													  if( isValid() ) {
														  CGPoint origin = [mWindow frame].origin;
														  mWindowOrigin.x = origin.x;
														  mWindowOrigin.y = origin.y;
													  }
												  }];

	mWindowRef->getSignalDraw().connect( [this] {
		gl::clear( Color::black() );
		gl::setMatricesWindow( this->getSize() );
	} );

	mValidRef = true;
	return mWindowRef;
}

void CocoaWindowCanvas::spawn()
{
	if( !isValid() ) {
		mApp->getRenderer()->makeCurrentContext();
		setWindow( createWindow() );
		setPos( mWindowOrigin );
		autoSizeToFitSubviews();
		enable();
	}
}

void CocoaWindowCanvas::close()
{
	if( isValid() ) {
		mValidRef = false;
		mWindowRef->close();
		mWindowRef = nullptr;
		[mWindow close];
		disable();
	}
}

bool CocoaWindowCanvas::isValid()
{
	return mValidRef;
}

vec2 CocoaWindowCanvas::getPos()
{
	return mWindowOrigin;
}

void CocoaWindowCanvas::setPos( vec2 pos )
{
	mWindowOrigin = pos;
	if( isValid() ) {
		[mWindow setFrameOrigin:NSMakePoint( pos.x, pos.y )];
	}
}

void CocoaWindowCanvas::autoSizeToFitSubviews()
{
	View::autoSizeToFitSubviews();
	ivec2 size = getSize();
	if( isValid() ) {
		if( mSubViews.size() ) {
			mWindowRef->setSize( size );
		}
		else {
			setSize( vec2( mCanvasWidth, 0 ) );
			mWindowRef->setSize( vec2( mCanvasWidth, 0 ) );
		}
		[mWindow setContentSize:NSMakeSize( size.x, size.y )];
	}
}

void CocoaWindowCanvas::save( const ci::fs::path &path )
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "VALID", isValid() ) );
	vec2 pos = getPos();
	tree.addChild( JsonTree( "XPOS", pos.x ) );
	tree.addChild( JsonTree( "YPOS", pos.y ) );
	tree.write( path );
}

void CocoaWindowCanvas::load( const ci::fs::path &path )
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
			cout << "COCOA WINDOW LOAD ERROR: " << exc.what() << endl;
		}
	}
}