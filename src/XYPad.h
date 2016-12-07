#pragma once

#include "ControlWithLabel.h"

namespace reza {
namespace ui {

class Canvas;

typedef std::shared_ptr<class XYPad> XYPadRef;
class XYPad : public ControlWithLabel {
  public:
	struct Format {
	  public:
		Format()
		{
			mFontSize = FontSize::SMALL;
			mLabel = true;
			mSticky = false;
			mHeight = -1;
			mMin = glm::vec2( -1.0 );
			mMax = glm::vec2( 1.0 );
			mStickyValue = 0.05f;
		}
		Format( const Format &copy )
		{
			mFontSize = copy.mFontSize;
			mLabel = copy.mLabel;
			mSticky = copy.mSticky;
			mStickyValue = copy.mStickyValue;
			mHeight = copy.mHeight;
			mMin = copy.mMin;
			mMax = copy.mMax;
		}
		Format &fontSize( FontSize fontSize )
		{
			mFontSize = fontSize;
			return *this;
		}
		Format &label( bool label )
		{
			mLabel = label;
			return *this;
		}
		Format &height( int height )
		{
			mHeight = (float)height;
			return *this;
		}
		Format &min( glm::vec2 min )
		{
			mMin = min;
			return *this;
		}
		Format &max( glm::vec2 max )
		{
			mMax = max;
			return *this;
		}
		Format &sticky( bool sticky = true )
		{
			mSticky = sticky;
			return *this;
		}
		Format &stickyValue( float value )
		{
			mStickyValue = value;
			return *this;
		}

	  protected:
		glm::vec2 mMin, mMax;
		float mHeight;
		float mStickyValue;
		bool mLabel, mSticky;
		FontSize mFontSize;
		friend class XYPad;
		friend class Canvas;
	};

	static XYPadRef create( const std::string name, glm::vec2 value, const Format &format = Format() )
	{
		return XYPadRef( new XYPad( name, value, format ) );
	}

	static XYPadRef create( const std::string name, glm::vec2 *value, const Format &format = Format() )
	{
		return XYPadRef( new XYPad( name, value, format ) );
	}

	const std::string getType() override { return "XYPad"; }
	void setValue( glm::vec2 value );
	glm::vec2 getValue();
	glm::vec2 getNormalizedValue();

	void updateValueRef();
	void updateLabel();

	virtual XYPad *setCallback( const std::function<void( glm::vec2 )> &callback );

	void setMax( glm::vec2 max, bool keepValueTheSame = false );
	glm::vec2 getMax();
	void setMin( glm::vec2 min, bool keepValueTheSame = false );
	glm::vec2 getMin();
	void setMinAndMax( glm::vec2 min, glm::vec2 max, bool keepValueTheSame = false );
	void setValueRef( glm::vec2 *value );

	bool isDraggable() override { return true; }
	XYPad( std::string name, glm::vec2 value, const Format &format = Format() );
	XYPad( std::string name, glm::vec2 *value, const Format &format = Format() );
	virtual ~XYPad();

  protected:
	void setup() override;
	void update() override;
	void trigger( bool recursive = false ) override;

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	virtual void input( const glm::vec2 &pt );

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseWheel( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	virtual void keyDown( ci::app::KeyEvent &event ) override;
	virtual void keyUp( ci::app::KeyEvent &event ) override;

	Format mFormat;
	glm::vec2 mValue;
	glm::vec2 *mValueRef;
	bool mStickyEnabled;
	bool mUseRef;
	std::function<void( glm::vec2 )> mCallbackFn;
};
}
} //namespace reza::ui