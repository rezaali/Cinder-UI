#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "UI.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

class basicApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
    void cleanup() override;
    fs::path getSaveLoadPath();
    
    SuperCanvasRef mUi;
    float mRed = 0.0;
    float mGreen = 0.0;
    float mBlue = 0.0;
};

void basicApp::setup()
{
    mUi = SuperCanvas::create( "basic" );
    mUi->addSpacer();
    mUi->addLabel( "WE CAN DO BETTER THAN PARAMS! ^_^", FontSize::SMALL );
    mUi->addLabel( "DOUBLE CLICK BASIC TO MINIMIZE", FontSize::SMALL );
    mUi->addLabel( "CLICK BASIC AND DRAG TO MOVE", FontSize::SMALL );
    mUi->addSpacer();

    mUi->addLabel( "BASIC SLIDER:", FontSize::SMALL );
    mUi->addSliderf( "RED", &mRed );

    mUi->addSpacer();
    mUi->addLabel( "SLIDER W/ OPTION: CROSSFADER", FontSize::SMALL );
    mUi->addSliderf( "GREEN", &mGreen, 0.0, 1.0, Sliderf::Format().crossFader() );

    mUi->addSpacer();
    mUi->addLabel( "SLIDER W/ OPTION: NO LABEL", FontSize::SMALL );
    mUi->addSliderf( "BLUE", &mBlue, 0.0, 1.0, Sliderf::Format().label( false ) );

    mUi->addMultiSlider( "BG COLOR", {
        MultiSlider::Data( "RED", &mRed ),
        MultiSlider::Data( "GREEN", &mGreen ),
        MultiSlider::Data( "BLUE", &mBlue )
    } );
    mUi->autoSizeToFitSubviews();
    
    mUi->load( getSaveLoadPath() );
}

void basicApp::update()
{
}

void basicApp::draw()
{
	gl::clear( Color( mRed, mGreen, mBlue ) );
}

void basicApp::cleanup()
{
    mUi->save( getSaveLoadPath() );
}

fs::path basicApp::getSaveLoadPath()
{
    fs::path path = getAssetPath( "" );
    path += "/" + mUi->getName() + ".json";
    return path;
}

CINDER_APP( basicApp, RendererGl )
