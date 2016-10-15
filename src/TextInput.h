#pragma once

#include "ControlWithLabel.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class TextInput> TextInputRef;
class TextInput : public ControlWithLabel {
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
		Format &trigger( Trigger trigger )
		{
			mTrigger = trigger;
			return *this;
		}
		Format &autoClear( bool autoClear = true )
		{
			mAutoClear = autoClear;
			return *this;
		}
		Format &numeric( bool numeric = true )
		{
			mNumeric = numeric;
			return *this;
		}

	  protected:
		bool mNumeric;
		bool mAutoClear;
		Trigger mTrigger;
		FontSize mFontSize;
		friend class TextInput;
	};

	static TextInputRef create( std::string name, const Format &format = Format() )
	{
		return TextInputRef( new TextInput( name, format ) );
	}

	std::string getValue();
	void setValue( std::string value );
	virtual void setCallback( const std::function<void( std::string )> &callback );

  protected:
	TextInput( std::string name, const Format &format = Format() );
	void setup() override;
	void trigger( bool recursive = false ) override;

	void calculateCursorPosition( const glm::vec2 &pt );
	void insertCharacter( const std::string &s );
	void deleteCharacter();

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	virtual void keyDown( ci::app::KeyEvent &event ) override;

	Format mFormat;
	bool mClicked;
	std::string mDisplayValue;
	std::string mValue;
	std::function<void( std::string )> mCallbackFn;
	int mMaxDisplayLength;
	float mCharWidth;
	int mCursorPosition;
	int mStartIndex;
};
}
} //namespace reza::ui