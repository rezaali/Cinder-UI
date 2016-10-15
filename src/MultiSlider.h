#pragma once

#include "ControlWithLabel.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class MultiSlider> MultiSliderRef;
class MultiSlider : public Control {
  public:
	struct Data {
		Data( std::string key, float *value, float min = 0.0f, float max = 1.0f )
			: mKey( key ), mValueRef( value ), mMin( min ), mMax( max ) { mValue = ci::lmap<double>( ( *mValueRef ), mMin, mMax, 0.0, 1.0 ); }
		float value() { return *mValueRef; }
		std::string mKey;
		double mValue;
		float *mValueRef;
		float mMin;
		float mMax;
	};

	struct Format {
	  public:
		Format() { crossFader( false ); }
		Format( const Format &copy )
		{
			mCrossFader = copy.mCrossFader;
		}
		Format &crossFader( bool crossFader = true )
		{
			mCrossFader = crossFader;
			return *this;
		}

	  protected:
		bool mCrossFader;
		friend class MultiSlider;
	};

	static MultiSliderRef create( const std::string name, const std::vector<Data> data, Format format = Format() )
	{
		return MultiSliderRef( new MultiSlider( name, data, format ) );
	}

	const std::string getType() override { return "MultiSlider"; }
	void setValue( float value );
	void setValue( const std::string key, float value );
	float getValue( const std::string key );
	float getNormalizedValue( const std::string key );

	void updateValueRef( const std::string key );
	void updateLabel( const std::string key );

	virtual MultiSlider *setCallback( const std::function<void( Data )> &callback );

	void setMax( const std::string key, float max, bool keepValueTheSame = false );
	float getMax( const std::string key );

	void setMin( const std::string key, float min, bool keepValueTheSame = false );
	float getMin( const std::string key );

	glm::vec2 getMaxAndMin( const std::string key );
	void setMinAndMax( const std::string key, float min, float max, bool keepValueTheSame = false );

	bool isDraggable() override { return true; }
	MultiSlider( std::string name, const std::vector<Data> data, Format format = Format() );

  protected:
	void setup() override;
	void update() override;
	void trigger( bool recursive = false ) override;
	void changeState() override;

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	virtual void _draw( std::vector<RenderData> &data, const ci::ColorA &color, const std::string key = "invalid" );
	void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	virtual std::string getKey( const glm::vec2 &pt );
	virtual void input( ci::app::MouseEvent &event );

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseWheel( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	virtual void keyDown( ci::app::KeyEvent &event ) override;
	virtual void keyUp( ci::app::KeyEvent &event ) override;

	Format mFormat;
	float mSliderHeight;
	float mSliderSpacing;
	std::vector<Data> mData;
	std::map<std::string, Data *> mDataMap;
	std::string mHitKey;
	std::string mHoverKey;
	std::map<std::string, LabelRef> mLabelsMap;
	std::function<void( Data )> mCallbackFn;
};
}
} //namespace reza::ui