#pragma once

#include "Alignments.h"
#include "ControlWithLabel.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class ColorPicker> ColorPickerRef;
class ColorPicker : public ControlWithLabel {
  public:
	struct Format {
	  public:
		Format();
		Format( const Format &copy );
		Format &fontSize( FontSize fontSize )
		{
			mFontSize = fontSize;
			return *this;
		}
		Format &label( bool label = true )
		{
			mLabel = label;
			return *this;
		}
		Format &align( Alignment align )
		{
			mAlign = align;
			return *this;
		}
		Format &size( int size )
		{
			mSize = size;
			return *this;
		}
		Format &fullScreen( bool fullScreen = true )
		{
			mFullScreen = fullScreen;
			return *this;
		}
		Format &sensitivity( float sensitivity )
		{
			mSensitivity = sensitivity;
			return *this;
		}
		Format &relative( bool relative = true )
		{
			mRelative = relative;
			return *this;
		}
		Format &rgb()
		{
			mColorModel = ci::ColorModel::CM_RGB;
			return *this;
		}
		Format &hsv()
		{
			mColorModel = ci::ColorModel::CM_HSV;
			return *this;
		}
		Format &alpha( const glm::vec2 &limits = glm::vec2( -1.0, 1.0 ) )
		{
			mAlphaLimits = limits;
			return *this;
		}

	  protected:
		FontSize mFontSize;
		bool mLabel;
		Alignment mAlign;
		int mSize;
		bool mFullScreen;
		float mSensitivity;
		bool mRelative;
		glm::vec2 mAlphaLimits;
		ci::ColorModel mColorModel;
		friend class ColorPicker;
	};

	static ColorPickerRef create( std::string name, const ci::ColorA &color, const Format &format = Format() )
	{
		return ColorPickerRef( new ColorPicker( name, color, format ) );
	}

	static ColorPickerRef create( std::string name, ci::ColorA *color, const Format &format = Format() )
	{
		return ColorPickerRef( new ColorPicker( name, color, format ) );
	}

	const std::string getType() override { return "ColorPicker"; }
	ci::ColorA getColor();
	virtual void setColor( const ci::ColorA &color );
	virtual ColorPicker *setCallback( const std::function<void( ci::ColorA )> &callback );

	ColorPicker( std::string name, const ci::ColorA &color, const Format &format = Format() );
	ColorPicker( std::string name, ci::ColorA *color, const Format &format = Format() );

	~ColorPicker();

  protected:
	bool isHit( const glm::vec2 &pt ) override;

	void setup() override;
	void update() override;
	void draw() override;
	void trigger( bool recursive = false ) override;

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	void updateLabel();
	void updateColorRef();

	virtual void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseWheel( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	bool mUseRef;
	ci::ColorA *mColorRef;
	ci::ColorA mColor;
	glm::vec2 mHitPoint;
	Format mFormat;

	std::vector<LabelRef> mColorLabels;
	std::vector<std::string> mLabelNames;

	std::function<void( ci::ColorA )> mCallbackFn;
};
}
} //namespace reza::ui