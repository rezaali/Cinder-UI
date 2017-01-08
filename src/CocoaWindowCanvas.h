#pragma once

#include "Canvas.h"
#include "cinder/app/cocoa/AppCocoaView.h"
#include "cinder/app/cocoa/AppImplMac.h"
#import <Cocoa/Cocoa.h>

namespace reza {
namespace ui {

typedef std::shared_ptr<class CocoaWindowCanvas> CocoaWindowCanvasRef;
class CocoaWindowCanvas : public Canvas {
  public:
	static CocoaWindowCanvasRef create( std::string name, std::string title )
	{
		CocoaWindowCanvasRef ref = CocoaWindowCanvasRef( new CocoaWindowCanvas( name, title ) );
		ref->setup();
		return ref;
	}

	static CocoaWindowCanvasRef create( std::string name )
	{
		CocoaWindowCanvasRef ref = CocoaWindowCanvasRef( new CocoaWindowCanvas( name, name ) );
		ref->setup();
		return ref;
	}

	void spawn();
	void close();
	bool isValid();

	glm::vec2 getPos();
	void setPos( glm::vec2 pos );

	void autoSizeToFitSubviews() override;

	void save( const ci::fs::path &path ) override;
	void load( const ci::fs::path &path ) override;

  protected:
	class UIApp : public cinder::app::AppCocoaView {
	};

	CocoaWindowCanvas( std::string name, std::string title );
	void setup() override;

	const std::string getType() override { return "WindowCanvas"; }
	bool isSaveable() override { return true; }
	cinder::app::AppCocoaView *createApp();
	ci::app::WindowRef createWindow();

	std::string mWindowTitle;
	NSWindow *mWindow = nullptr;
	CinderViewMac *mView = nullptr;
	UIApp *mApp = nullptr;
	bool mValidRef = false;
	glm::vec2 mWindowOrigin = glm::vec2( 0.0 );
};
}
}
