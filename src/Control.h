#pragma once

#include "States.h"
#include "Triggers.h"
#include "View.h"
#include "cinder/app/Event.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Control> ControlRef;
class Control : public View {
  public:
	static ControlRef create()
	{
		return ControlRef( new Control() );
	}

	Control();
	virtual ~Control();

	const std::string getType() override { return "Control"; }
	//HIT TESTS
	virtual bool isHit( const glm::vec2 &pt ) override;
	virtual glm::vec2 getHitPercent( const glm::vec2 &pt ) override;

	virtual ci::Rectf getBounds( bool bIncludeSubviews = false ) override;

	//DIMENSIONS
	virtual void setSize( glm::vec2 size ) override;

	//PROPERTIES
	virtual void setState( State state );
	virtual State getState();
	virtual void changeState();

	//TRIGGERS
	virtual void setTrigger( Trigger trigger );
	virtual Trigger getTrigger();

	//RENDERING
	virtual std::vector<RenderData> render() override;
	virtual void drawBounds( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawBoundsOutline( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawBack( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawFill( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color );
	virtual void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color );

	bool getDrawBounds();
	bool getDrawBoundsOutline();
	bool getDrawBack();
	bool getDrawFill();
	bool getDrawFillHighLight();
	bool getDrawOutline();
	bool getDrawOutlineHighLight();

	void setDrawBounds( bool drawBounds );
	void setDrawBoundsOutline( bool drawBoundsOutline );
	void setDrawBack( bool drawBack );
	void setDrawFill( bool drawFill );
	void setDrawFillHighLight( bool drawFillHighlight );
	void setDrawOutline( bool drawOutline );
	void setDrawOutlineHighLight( bool drawOutlineHighlight );

	ci::ColorA &getColorBounds();
	ci::ColorA &getColorBoundsOutline();
	ci::ColorA &getColorBack();
	ci::ColorA &getColorOutline();
	ci::ColorA &getColorOutlineHighlight();
	ci::ColorA &getColorFill();
	ci::ColorA &getColorFillHighlight();

	void setColorBounds( const ci::ColorA &colorBounds );
	void setColorBoundsOutline( const ci::ColorA &colorBoundsHighlight );
	void setColorBack( const ci::ColorA &colorBack );
	void setColorOutline( const ci::ColorA &colorOutline );
	void setColorOutlineHighlight( const ci::ColorA &colorOutlineHighlight );
	void setColorFill( const ci::ColorA &colorFill );
	void setColorFillHighlight( const ci::ColorA &colorFillHighlight );

	bool getIsBindedToKey( int key );
	bool isBindedToKeyEvent( ci::app::KeyEvent &event );
	bool getIsModifierDown( ci::app::KeyEvent &event );
	bool getIsBindedToModifier( ci::app::KeyEvent &event );
	void bindToKey( int key, int modifier = -1 );
	void unbindToKey( int key );
	void unbindAllKeys();

  protected:
	ci::Rectf mHitRect;
	State mState;
	Trigger mTrigger;
	bool mKeyHit;
	std::map<int, bool> mKeyBindings;
	int mKeyModifier;

	bool mDrawBounds;
	bool mDrawBoundsOutline;
	bool mDrawBack;
	bool mDrawOutline;
	bool mDrawOutlineHighlight;
	bool mDrawFill;
	bool mDrawFillHighlight;

	ci::ColorA mColorClear;
	ci::ColorA mColorBounds;
	ci::ColorA mColorBoundsOutline;
	ci::ColorA mColorBack;
	ci::ColorA mColorOutline;
	ci::ColorA mColorOutlineHighlight;
	ci::ColorA mColorFill;
	ci::ColorA mColorFillHighlight;
};
}
} //namespace reza::ui