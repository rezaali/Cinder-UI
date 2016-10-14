#pragma once

#include <map>

#include "Alignments.h"
#include "Directions.h"

#include "Controls.h"

#include "cinder/app/Window.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Canvas> CanvasRef;
class Canvas : public Control {
  public:
	static CanvasRef create( std::string name, const ci::app::WindowRef &window = ci::app::getWindow() )
	{
		CanvasRef ref = CanvasRef( new Canvas( name, window ) );
		ref->setup();
		return ref;
	}

	Canvas( std::string name, const ci::app::WindowRef &window = ci::app::getWindow() );
	~Canvas();

	const std::string getType() override { return "Canvas"; }
	void clear() override;

	//SAVE/LOAD

	virtual void save( const ci::fs::path &path );
	virtual void load( const ci::fs::path &path );

	void setWindow( ci::app::WindowRef window );
	ci::app::WindowRef getWindow();
	void setSize( glm::vec2 size ) override;

	void setEnabled( bool enabled ) override;
	void enable();
	void disable();

	void clearPlacer();
	void setPlacer( ViewRef subView );

	void setSliderHeight( float height );
	void setGraphHeight( float height );
	void setButtonDimension( float dimension );
	void setSpacerHeight( float height );

	float getSliderHeight();
	float getGraphHeight();
	float getButtonDimension();
	float getSpacerHeight();

	void setSubViewAlignment( Alignment alignment );
	void setSubViewDirection( Direction direction );
	void setFontSize( FontSize fontSize );

	Alignment getSubViewAlignment();
	Direction getSubViewDirection();
	FontSize getSubViewFontSize();

	ViewRef addSubViewPosition( ViewRef subView,
		Direction direction = Direction::SOUTH,
		Alignment alignment = Alignment::LEFT,
		bool reAdd = false );

	ViewRef addSubViewDown( ViewRef subView, Alignment alignment = Alignment::LEFT, bool reAdd = false );
	ViewRef addSubViewUp( ViewRef subView, Alignment alignment = Alignment::LEFT, bool reAdd = false );
	ViewRef addSubViewRight( ViewRef subView, Alignment alignment = Alignment::NONE, bool reAdd = false );
	ViewRef addSubViewLeft( ViewRef subView, Alignment alignment = Alignment::NONE, bool reAdd = false );

	ViewRef addSubViewSouthOf( ViewRef subView, std::string referenceName, bool reAdd = false );
	ViewRef addSubViewNorthOf( ViewRef subView, std::string referenceName, bool reAdd = false );
	ViewRef addSubViewWestOf( ViewRef subView, std::string referenceName, bool reAdd = false );
	ViewRef addSubViewEastOf( ViewRef subView, std::string referenceName, bool reAdd = false );

	//EVENTS
	virtual void draw() override;

	static CanvasRef create( std::string name )
	{
		return CanvasRef( new Canvas( name ) );
	}

	EnvelopeRef addEnvelope( const std::string name, const Envelope::Format &format = Envelope::Format() );

	QuaternionOrdererRef addQuaternionOrderer( const std::string name, const QuaternionOrderer::Format &format = QuaternionOrderer::Format() );

	BSplineEditorRef addBSplineEditor( const std::string name, ci::BSpline2f spline, BSplineEditor::Format format = BSplineEditor::Format() );

	SlideriRef addSlideri( const std::string name, int value, int min = 0, int max = 255, Slideri::Format format = Slideri::Format() );
	SlideriRef addSlideri( const std::string name, int *value, int min = 0, int max = 255, Slideri::Format format = Slideri::Format() );

	SliderfRef addSliderf( const std::string name, float value, float min = 0.0f, float max = 1.0f, Sliderf::Format format = Sliderf::Format() );
	SliderfRef addSliderf( const std::string name, float *value, float min = 0.0f, float max = 1.0f, Sliderf::Format format = Sliderf::Format() );

	SliderdRef addSliderd( const std::string name, double value, double min = 0.0, double max = 1.0, Sliderd::Format format = Sliderd::Format() );
	SliderdRef addSliderd( const std::string name, double *value, double min = 0.0, double max = 1.0, Sliderd::Format format = Sliderd::Format() );
	DialeriRef addDialeri( const std::string name, int value, int min, int max, Dialeri::Format format = Dialeri::Format() );
	DialeriRef addDialeri( const std::string name, int *value, int min, int max, Dialeri::Format format = Dialeri::Format() );

	DialerfRef addDialerf( const std::string name, float value, float min, float max, Dialerf::Format format = Dialerf::Format() );
	DialerfRef addDialerf( const std::string name, float *value, float min, float max, Dialerf::Format format = Dialerf::Format() );

	DialerdRef addDialerd( const std::string name, double value, double min, double max, Dialerd::Format format = Dialerd::Format() );
	DialerdRef addDialerd( const std::string name, double *value, double min, double max, Dialerd::Format format = Dialerd::Format() );

	MultiSliderRef addMultiSlider( const std::string name, const std::vector<MultiSlider::Data> data, MultiSlider::Format format = MultiSlider::Format() );

	RangeiRef addRangei( const std::string name, int valueLow, int valueHigh, int min = 0, int max = 255, Rangei::Format format = Rangei::Format() );
	RangefRef addRangef( const std::string name, float valueLow, float valueHigh, float min = 0.0f, float max = 1.0f, Rangef::Format format = Rangef::Format() );
	RangedRef addRanged( const std::string name, double valueLow, double valueHigh, double min = 0.0, double max = 1.0, Ranged::Format format = Ranged::Format() );

	RangeiRef addRangei( const std::string name, int *valueLow, int *valueHigh, int min = 0, int max = 255, Rangei::Format format = Rangei::Format() );
	RangefRef addRangef( const std::string name, float *valueLow, float *valueHigh, float min = 0.0f, float max = 1.0f, Rangef::Format format = Rangef::Format() );
	RangedRef addRanged( const std::string name, double *valueLow, double *valueHigh, double min = 0.0, double max = 1.0, Ranged::Format format = Ranged::Format() );

	ButtonRef addButton( const std::string name, bool value, const Button::Format &format = Button::Format() );
	ButtonRef addButton( const std::string name, bool *value, const Button::Format &format = Button::Format() );

	ToggleRef addToggle( const std::string name, bool value, const Toggle::Format &format = Toggle::Format() );
	ToggleRef addToggle( const std::string name, bool *value, const Toggle::Format &format = Toggle::Format() );

	LabelRef addLabel( const std::string label, FontSize fontSize = FontSize::MEDIUM );
	FpsRef addFps( const std::string prefix = ":", FontSize fontSize = FontSize::MEDIUM );

	SpacerRef addSpacer();

	GraphRef addGraph( const std::string name, int bufferSize, float *xValues, float *yValues, glm::vec2 xRange = glm::vec2( -1.0f, 1.0f ), glm::vec2 yRange = glm::vec2( -1.0f, 1.0f ), float height = -1.0 );

	PlotterRef addPlotter( const std::string name, float *value, glm::vec2 range = glm::vec2( -1.0f, 1.0f ), int bufferSize = 64, float height = -1.0 );

	WaveformRef addWaveform( const std::string name, int bufferSize, float *values, glm::vec2 range = glm::vec2( 1.0f, -1.0f ), float height = -1.0 );

	SpectrumRef addSpectrum( const std::string name, int bufferSize, float *values, glm::vec2 range = glm::vec2( 1.0f, -1.0f ), float height = -1.0 );

	XYPadRef addXYPad( const std::string name, glm::vec2 value, const XYPad::Format &format = XYPad::Format() );
	XYPadRef addXYPad( const std::string name, glm::vec2 *value, const XYPad::Format &format = XYPad::Format() );

	TextureViewRef addTexture( std::string name, const ci::gl::TextureRef &textureRef, const TextureView::Format &format = TextureView::Format() );

	RadioRef addRadio( std::string name, const std::vector<std::string> options, const Radio::Format &format = Radio::Format() );

	TextInputRef addTextInput( std::string name, const TextInput::Format &format = TextInput::Format() );

	ColorPickerRef addColorPicker( std::string name, const ci::ColorA &color, const ColorPicker::Format &format = ColorPicker::Format() );

	ColorPickerRef addColorPicker( std::string name, ci::ColorA *color, const ColorPicker::Format &format = ColorPicker::Format() );

	void right();
	void down();

  protected:
	void enableUpdateCallback();
	void disableUpdateCallback();

#if defined( CINDER_COCOA_TOUCH )
	void enableTouchCallbacks();
	void disableTouchCallbacks();
#else
	void enableMouseCallbacks();
	void disableMouseCallbacks();
	void enableKeyboardCallbacks();
	void disableKeyboardCallbacks();
#endif

	void setupShader();
	void setupBuffers();
	std::vector<RenderData> &getRenderData() override;
	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	ci::gl::VaoRef mVaoRef;
	ci::gl::VboRef mVboRef;
	ci::gl::GlslProgRef mGlslProgRef;

	ci::signals::ScopedConnection mPostDrawCb;
#if defined( CINDER_COCOA_TOUCH )
	ci::signals::scoped_connection mTouchesBeganCb, mTouchesMovedCb, mTouchesEndedCb;
#else
	ci::signals::ScopedConnection mMouseDownCb, mMouseDragCb, mMouseUpCb, mMouseMoveCb, mMouseWheelCb;
	ci::signals::ScopedConnection mKeyDownCb, mKeyUpCb;
#endif

	std::vector<ViewRef> mLastAddedSubViews;

	float mCanvasWidth;
	float mSliderHeight;
	float mGraphHeight;
	float mButtonDimension;
	float mSpacerHeight;
	Direction mDirection;
	Alignment mAlignment;
	FontSize mFontsize;
	ci::app::WindowRef mWindowRef;
};
}
} //namespace reza::ui