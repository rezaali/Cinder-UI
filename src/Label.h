#pragma once

#include "Control.h"
#include "FontSizes.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Label> LabelRef;
class Label : public Control {
  public:
	static LabelRef create( std::string name, std::string label, FontSize fontSize )
	{
		return LabelRef( new Label( name, label, fontSize ) );
	}

	const std::string getType() override { return "Label"; }
	void setLabel( const std::string &label );
	const std::string getLabel() { return mLabel; };
	float getStringWidth( const std::string &s );
	float getStringHeight( const std::string &s );
	float getSpacing();

	glm::vec2 &getShadowOffset();
	void setShadowOffset( glm::vec2 shadowOffset );
	bool getDrawShadow();
	void setDrawShadow( bool drawShadow );
	ci::ColorA &getColorShadow();
	void setColorShadow( const ci::ColorA &colorShadow );

  protected:
	Label( std::string name, std::string label, FontSize fontSize );
	std::vector<RenderData> render() override;
	void changeState() override;
	virtual void drawShadow( std::vector<RenderData> &data, const ci::ColorA &color );
	void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	float addChar( std::vector<RenderData> &data, const ci::ColorA &color, const char &c, glm::vec2 off, float height );
	void addString( std::vector<RenderData> &data, const ci::ColorA &color, const std::string &s, const glm::vec2 &off, float height );

	std::string mLabel;
	FontSize mFontSize;

	glm::vec2 mShadowOffset;
	ci::ColorA mColorShadow;
	bool mDrawShadow;
};
}
} //namespace reza::ui