#pragma once

#include "Button.h"
#include "ControlWithLabel.h"
#include "Directions.h"
#include "Toggle.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Radio> RadioRef;
class Radio : public ControlWithLabel {
  public:
	struct Format {
	  public:
		Format() { label( false ).fontSize( FontSize::SMALL ).active( "" ).buttonFormat( Button::Format() ).direction( Direction::SOUTH ); }
		Format( const Format &copy )
		{
			mLabel = copy.mLabel;
			mFontSize = copy.mFontSize;
			mActive = copy.mActive;
			mButtonFormat = copy.mButtonFormat;
			mDirection = copy.mDirection;
		}
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
		Format &active( std::string active )
		{
			mActive = active;
			return *this;
		}
		Format &buttonFormat( const Button::Format &buttonFormat )
		{
			mButtonFormat = buttonFormat;
			return *this;
		}
		Format &direction( Direction direcion = Direction::EAST )
		{
			mDirection = direcion;
			;
			return *this;
		}

	  protected:
		Button::Format mButtonFormat;
		FontSize mFontSize;
		bool mLabel;
		std::string mActive;
		Direction mDirection;
		friend class Radio;
	};

	static RadioRef create( const std::string name, const std::vector<std::string> options, const Format &format = Format() )
	{
		return RadioRef( new Radio( name, options, format ) );
	}

	const std::string getType() override { return "Radio"; }
	void activate( std::string name );
	virtual void setCallback( const std::function<void( std::string, bool )> &callback );

	~Radio();

  protected:
	Radio( std::string name, const std::vector<std::string> options, const Format &format = Format() );

	void setup() override;
	void trigger( bool recursive = false ) override;

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	virtual void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	virtual void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	Format mFormat;
	std::vector<std::string> mOptions;
	std::map<std::string, ToggleRef> mToggleMap;
	ToggleRef mActive;
	std::function<void( std::string, bool )> mCallbackFn;
};
}
} //namespace reza::ui