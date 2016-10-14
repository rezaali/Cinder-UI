#pragma once

#include "cinder/Color.h"
#include "cinder/Json.h"
#include "cinder/Rect.h"
#include "cinder/app/App.h"

#include "Padding.h"
#include "RenderData.h"
#include "Responder.h"
#include <vector>

namespace reza {
namespace ui {

typedef std::shared_ptr<class View> ViewRef;
typedef std::weak_ptr<class View> ViewWeakRef;

class View : public Responder, public std::enable_shared_from_this<class View> {
  public:
	virtual ~View();

	const std::string getType() override { return "View"; }
	virtual void clear();

	virtual bool isHit( const glm::vec2 &pt );
	virtual glm::vec2 getHitPercent( const glm::vec2 &pt );

	//SAVE/LOAD
	virtual bool isSaveable() { return false; }
	virtual ci::JsonTree save();
	virtual void load( const ci::JsonTree &data );

	void setLoadSubViews( bool loadSubViews );
	bool getLoadSubViews();

	void setTriggerOnLoad( bool triggerOnLoad );
	bool getTriggerOnLoad();

	void setTriggerSubViews( bool triggerSubViews );
	bool getTriggerSubViews();

	virtual void trigger( bool recursive = false );

	virtual bool isDraggable() { return false; }
	bool isEnabled();
	virtual void setEnabled( bool enabled );

	bool isVisible();
	void setVisible( bool visible );

	virtual void setOrigin( glm::vec2 origin );
	glm::vec2 getOrigin( bool recursive = true );

	virtual void setSize( glm::vec2 size );
	glm::vec2 getSize();

	float getX();
	float getY();
	float getWidth();
	float getHeight();

	virtual ci::Rectf getBounds( bool bIncludeSubviews = false );
	void setPadding( const Paddingf &padding )
	{
		mPadding = padding;
		setNeedsDisplay();
	}
	const Paddingf &getPadding();

	virtual void addSubView( ViewRef subView );

	bool isSetup() { return mSetup; };
	virtual void setup();
	virtual void update();
	virtual void draw();
	virtual std::vector<RenderData> render();

	virtual size_t getRenderDataSize();
	virtual size_t getViewRenderDataSize();
	virtual std::vector<RenderData> &getViewRenderData();
	virtual std::vector<RenderData> &getRenderData();
	virtual void setNeedsDisplay( bool bSetNeedsDisplay = true );

	bool getNeedsDisplay();
	virtual void setNeedsDisplay( View *view, bool superViewNeedsDisplay = true, bool subViewsNeedsDisplay = false );
	virtual void setSuperViewNeedsDisplay();
	virtual void setSubViewsNeedsDisplay();

	virtual void autoSizeToFitSubviews();

	void addPoint( std::vector<RenderData> &data, const ci::ColorA &color, const glm::vec2 &p, float pointSize = 1.0f );
	void addCircle( std::vector<RenderData> &data, const ci::ColorA &color, const glm::vec2 &p, float radius = 1.0f, int resolution = 30 );
	void addLine( std::vector<RenderData> &data, const ci::ColorA &color, const glm::vec2 &p0, const glm::vec2 &p1, float lineWidth = 1.0f );
	void addBounds( std::vector<RenderData> &data, const ci::ColorA &color );
	void addBoundsOutline( std::vector<RenderData> &data, const ci::ColorA &color, float lineWidth = 1.0f );
	void addRect( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf &rect, float z = 0.0f );
	void addRectOutline( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf &rect, float lineWidth = 1.0f );
	void addPointGrid( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf rect, float gridInterval, float pointSize = 1.0f );
	void addCenterLines( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf rect, float lineWidth = 1.0f );
	void addTriangle( std::vector<RenderData> &data, const ci::ColorA &color, const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2 );

//EVENTS
#if defined( CINDER_COCOA_TOUCH )
	virtual void touchesBegan( ci::app::TouchEvent &event );
	virtual void touchesMoved( ci::app::TouchEvent &event );
	virtual void touchesEnded( ci::app::TouchEvent &event );
#else
	virtual void mouseDown( ci::app::MouseEvent &event );
	virtual void mouseUp( ci::app::MouseEvent &event );
	virtual void mouseWheel( ci::app::MouseEvent &event );
	virtual void mouseMove( ci::app::MouseEvent &event );
	virtual void mouseDrag( ci::app::MouseEvent &event );

	virtual void keyDown( ci::app::KeyEvent &event );
	virtual void keyUp( ci::app::KeyEvent &event );
#endif

	ViewRef getSubView( std::string subViewName, int subViewID = -1 );

    std::vector<ViewRef> getSubViews();
  protected:
	View();
	ViewWeakRef mSuperView;
	std::vector<ViewRef> mSubViews;
	std::multimap<std::string, ViewRef> mSubViewsNameMap;

	std::vector<RenderData> mViewRenderData;
	std::vector<RenderData> mRenderData;

	Paddingf mPadding;
	ci::Rectf mBounds;
	bool mSetup;
	bool mSetNeedsDisplay;
	bool mTriggerSubViews;
	bool mTriggerOnLoad;
	bool mLoadSubViews;
	bool mVisible;
	bool mHit;
	int mTouchId;
	bool mEnabled;
	bool mEmbedded;
	int mUniqueIDs;
};
}
} // namespace reza::ui