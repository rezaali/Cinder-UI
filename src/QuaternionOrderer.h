#pragma once

#include "ControlWithLabel.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class QuaternionOrderer> QuaternionOrdererRef;
class QuaternionOrderer : public ControlWithLabel {
  public:
	struct Format {
	  public:
		Format()
		{
			mFontSize = FontSize::SMALL;
			mLabel = true;
			mHeight = -1;
			mStickyValue = 0.01f;
			mThreshold = 0.01f;

			mShowTime = false;
			mTimeRef = nullptr;
			mTime = 0.0f;
			mUseTimeRef = false;
		}
		Format( const Format &copy )
		{
			mFontSize = copy.mFontSize;
			mLabel = copy.mLabel;
			mHeight = copy.mHeight;
			mStickyValue = copy.mStickyValue;
			mThreshold = copy.mThreshold;

			mShowTime = copy.mShowTime;
			mTimeRef = copy.mTimeRef;
			mTime = copy.mTime;
			mUseTimeRef = copy.mUseTimeRef;
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
		Format &stickyValue( float value )
		{
			mStickyValue = value;
			return *this;
		}
		Format &threshold( float threshold )
		{
			mThreshold = threshold;
			return *this;
		}
		Format &showTime( bool showTime )
		{
			mShowTime = showTime;
			return *this;
		}
		Format &setTimeRef( float *TimeRef )
		{
			mUseTimeRef = true;
			mShowTime = true;
			mTimeRef = TimeRef;
			mTime = *TimeRef;
			return *this;
		}
		float getHeight() const { return mHeight; }
	  protected:
		FontSize mFontSize;
		bool mLabel;
		float mHeight;
		float mStickyValue;
		float mThreshold;

		float *mTimeRef;
		float mTime;
		bool mShowTime;
		bool mUseTimeRef;

		friend class QuaternionOrderer;
		friend class Canvas;
	};

	static QuaternionOrdererRef create( std::string name, const Format &format = Format() )
	{
		return QuaternionOrdererRef( new QuaternionOrderer( name, format ) );
	}

	const std::string getType() override { return "QuaternionOrderer"; }
	void setShowTime( bool showTime = true );
	void setTime( float Time );
	void setTimeRef( float *TimeRef );

	void setEasingFn( std::function<float( float )> easingFn );

	void addQuat( float time, glm::quat quat );
	glm::quat getQuat( float time );

	void reset();

	virtual ~QuaternionOrderer();

  protected:
	QuaternionOrderer( std::string name, const Format &format = Format() );

	void setup() override;
	void update() override;
	std::vector<RenderData> render() override;

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;

	virtual void input( const ci::app::MouseEvent &event );

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseWheel( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	Format mFormat;
	int mHitIndex = -1;
	std::vector<float> mTimes;
	std::vector<glm::quat> mQuats;
	std::function<void( glm::quat )> mCallbackFn;
	std::function<float( float )> mEasingFn;
};
}
} //namespace reza::ui