#pragma once

#include "ControlWithLabel.h"

namespace reza {
namespace ui {

template <typename T>
class DialerT : public ControlWithLabel {
  public:
	typedef std::shared_ptr<class DialerT<T>> DialerRef;
	typedef std::shared_ptr<class DialerT<int>> DialeriRef;
	typedef std::shared_ptr<class DialerT<float>> DialerfRef;
	typedef std::shared_ptr<class DialerT<double>> DialerdRef;

	struct Format {
	  public:
		Format() { fontSize( FontSize::SMALL ).precision( 3 ).label( true ).sign( true ); }
		Format( const Format &copy )
		{
			mFontSize = copy.mFontSize;
			mPrecision = copy.mPrecision;
			mLabel = copy.mLabel;
			mSign = copy.mSign;
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
		Format &precision( int pre )
		{
			mPrecision = pre;
			return *this;
		}
		Format &sign( bool sign )
		{
			mSign = sign;
			return *this;
		}

	  protected:
		FontSize mFontSize;
		int mPrecision;
		bool mLabel, mSign;
		friend class DialerT;
	};

	static DialerRef create( const std::string name, T val, T min, T max, Format format = Format() )
	{
		return DialerRef( new DialerT( name, val, min, max, format ) );
	}

	static DialerRef create( const std::string name, T *val, T min, T max, Format format = Format() )
	{
		return DialerRef( new DialerT( name, val, min, max, format ) );
	}

	DialerT( std::string name, T value, T min, T max, Format format );
	DialerT( std::string name, T *value, T min, T max, Format format );
	virtual ~DialerT();

	const std::string getType() override { return "Dialer"; }
	void setValue( T value );
	T getValue();
	double getNormalizedValue();

	virtual DialerT *setCallback( const std::function<void( T )> &callback );

	void setMax( T max, bool keepValueTheSame = false );
	T getMax();
	void setMin( T min, bool keepValueTheSame = false );
	T getMin();
	glm::vec2 getMaxAndMin();
	void setMinAndMax( T min, T max, bool keepValueTheSame = false );
	void setValueRef( T *value );

	bool isDraggable() override { return true; }
  protected:
	virtual void setup() override;
	void update() override;
	void trigger( bool recursive = false ) override;
	void updateValueRef();
	void updateLabel();

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

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

	glm::vec2 getHitPercent( const glm::vec2 &pt ) override;
	void calculatePrecisionZone();
	ci::Rectf getHitRect();

	Format mFormat;
	T mMin;
	T mMax;
	double mValue;
	T *mValueRef;
	bool mUseRef;
	int mSetWidth;

	double mZoneMultiplier;
	glm::vec2 mHitPoint;
	int mNumOfPrecisionZones;
	int mCurrentPrecisionZone;
	int mCurrentIndex;
	int mDotIndex;
	float mDigitWidth;
	float mLabelWidth;

	std::function<void( T )> mCallbackFn;
};

typedef DialerT<int> Dialeri;
typedef DialerT<float> Dialerf;
typedef DialerT<double> Dialerd;

typedef std::shared_ptr<class DialerT<int>> DialeriRef;
typedef std::shared_ptr<class DialerT<float>> DialerfRef;
typedef std::shared_ptr<class DialerT<double>> DialerdRef;
}
} //namespace reza::ui