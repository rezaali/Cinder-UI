#include "MultiSlider.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

MultiSlider::MultiSlider( std::string name, const std::vector<Data> data, Format format )
	: Control(), mData( data ), mHitKey( "" ), mHoverKey( "" ), mCallbackFn( nullptr ), mFormat( format )
{
	setName( name );
	setSize( vec2( 192.0f, 16.0f ) );
	for( auto &it : mData ) {
		mDataMap[it.mKey] = &it;
	}
}

void MultiSlider::setup()
{
	float numSliders = (float)mData.size();
	vec2 size = getSize();
	size.y = mSliderHeight = std::max( size.y - mPadding.mTop - mPadding.mBottom,
		( (float)FontSize::SMALL + mPadding.mBottom + mPadding.mTop ) );
	mSliderSpacing = mPadding.mTop;
	size.y = mSliderHeight * numSliders + mPadding.mTop * 2.0f + mPadding.mBottom * 2.0f;
	if( mData.size() > 1 ) size.y += mSliderSpacing * ( numSliders - 1.0f );
	setSize( size );

	vec2 hitRectSize = mHitRect.getSize();
	float width = hitRectSize.x - ( mPadding.mLeft + mPadding.mRight );
	Rectf rect = mHitRect;
	int index = 0;
	for( auto &it : mData ) {
		rect.y1 = mHitRect.y1 + mPadding.mTop + ( mSliderSpacing + mSliderHeight ) * index;
		rect.y2 = rect.y1 + mSliderHeight;

		rect.x1 = mHitRect.x1 + mPadding.mLeft;
		rect.x2 = rect.x1 + width;

		string sliderName = it.mKey;
		LabelRef label = Label::create( sliderName + "_LABEL", sliderName, FontSize::SMALL );
		mLabelsMap[sliderName] = label;
		addSubView( label );

		float labelHeight = label->getHeight();
		float offset = ( rect.getHeight() - labelHeight ) / 2.0f;
		label->setOrigin( vec2( rect.x1, rect.y1 + offset ) );
		index++;
	}
	View::setup();
}

void MultiSlider::update()
{
	for( auto &it : mData ) {
		float scaledValue = (float)lmap<double>( it.mValue, 0.0, 1.0, it.mMin, it.mMax );
		if( ( *it.mValueRef ) != scaledValue ) {
			setValue( it.mKey, *it.mValueRef );
		}
	}
	View::update();
}

void MultiSlider::setValue( float value )
{
	for( auto &it : mDataMap ) {
		Data *data = it.second;
		setValue( it.first, (float)lmap<double>( value, 0.0, 1.0, data->mMin, data->mMax ) );
	}
}

void MultiSlider::setValue( const std::string key, float value )
{
	Data *data = mDataMap[key];
	data->mValue = lmap<double>( value, data->mMin, data->mMax, 0.0, 1.0 );
	updateValueRef( key );
	updateLabel( key );
	setNeedsDisplay();
}

float MultiSlider::getValue( const std::string key )
{
	return ( *mDataMap[key]->mValueRef );
}

float MultiSlider::getNormalizedValue( const std::string key )
{
	return std::max( std::min( (float)mDataMap[key]->mValue, 1.0f ), 0.0f );
}

void MultiSlider::updateValueRef( const std::string key )
{
	Data *data = mDataMap[key];
	*data->mValueRef = (float)lmap<double>( data->mValue, 0.0, 1.0, data->mMin, data->mMax );
}

void MultiSlider::updateLabel( const std::string key )
{
	auto it = mLabelsMap.find( key );
	if( it != mLabelsMap.end() ) it->second->setLabel( key + ":" + to_string( getValue( key ) ) );
}

MultiSlider *MultiSlider::setCallback( const std::function<void( Data )> &callback )
{
	mCallbackFn = callback;
	return this;
}

void MultiSlider::setMax( const std::string key, float max, bool keepValueTheSame )
{
	mDataMap[key]->mMax = max;
}

float MultiSlider::getMax( const std::string key )
{
	return mDataMap[key]->mMax;
}

void MultiSlider::setMin( const std::string key, float min, bool keepValueTheSame )
{
	mDataMap[key]->mMin = min;
}

float MultiSlider::getMin( const std::string key )
{
	return mDataMap[key]->mMin;
}

glm::vec2 MultiSlider::getMaxAndMin( const std::string key )
{
	return vec2( mDataMap[key]->mMin, mDataMap[key]->mMax );
}

void MultiSlider::setMinAndMax( const std::string key, float min, float max, bool keepValueTheSame )
{
	mDataMap[key]->mMin = min;
	mDataMap[key]->mMax = max;
}

void MultiSlider::trigger( bool recursive )
{
	if( mCallbackFn && mHitKey != "" ) {
		mCallbackFn( *mDataMap[mHitKey] );
	}
	Control::trigger( recursive );
}

void MultiSlider::changeState()
{
	Control::changeState();
	for( auto &it : mLabelsMap ) {
		if( it.first == mHoverKey || it.first == mHitKey ) {
			it.second->setState( mState );
		}
		else {
			it.second->setState( State::NORMAL );
		}
	}
}

ci::JsonTree MultiSlider::save()
{
	JsonTree tree = View::save();
	for( auto &it : mData ) {
		tree.addChild( JsonTree( it.mKey, ( *it.mValueRef ) ) );
	}
	return tree;
}

void MultiSlider::load( const ci::JsonTree &data )
{
	for( auto &it : mData ) {
		if( data.hasChild( it.mKey ) ) {
			setValue( it.mKey, data.getValueForKey<float>( it.mKey ) );
		}
		mHitKey = it.mKey;
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	mHitKey = "";
	View::load( data );
}

void MultiSlider::_draw( std::vector<RenderData> &data, const ci::ColorA &color, const std::string key )
{
	int index = 0;

	vec2 size = mHitRect.getSize();
	float width = size.x - ( mPadding.mLeft + mPadding.mRight );
	bool drawAll = false;
	if( key == "invalid" ) {
		drawAll = true;
	}
	for( auto &it : mData ) {
		Rectf rect = mHitRect;
		rect.y1 = mHitRect.y1 + mPadding.mTop + ( mSliderSpacing + mSliderHeight ) * index;
		rect.y2 = rect.y1 + mSliderHeight;

		if( mFormat.mCrossFader ) {
			float h = mSliderHeight;
			float hh = h * 0.5;
			rect.x2 = rect.x1 + ( width - h ) * getNormalizedValue( it.mKey ) + hh;
			rect.x1 = rect.x2 - hh;
			rect.x2 = rect.x1 + h;
		}
		else {
			rect.x1 = mHitRect.x1 + mPadding.mLeft;
			rect.x2 = rect.x1 + width * getNormalizedValue( it.mKey );
		}

		if( it.mKey == key || drawAll ) {
			addRect( data, color, rect );
		}
		else {
			addRect( data, mColorClear, rect );
		}
		index++;
	}
}

void MultiSlider::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
	float numSliders = mData.size();
	float width = mHitRect.getWidth() - ( mPadding.mLeft + mPadding.mRight );
	Rectf rect = mHitRect;
	for( int i = 0; i < numSliders; i++ ) {
		rect.y1 = mHitRect.y1 + mPadding.mTop + ( mSliderSpacing + mSliderHeight ) * i;
		rect.y2 = rect.y1 + mSliderHeight;

		rect.x1 = mHitRect.x1 + mPadding.mLeft;
		rect.x2 = rect.x1 + width;
		addRect( data, color, rect );
	}
	Control::drawBack( data, color );
}

void MultiSlider::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	_draw( data, color );
}

void MultiSlider::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	_draw( data, color, mHoverKey );
	Control::drawOutlineHighlight( data, color );
}

void MultiSlider::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	_draw( data, color, mHitKey );
}

std::string MultiSlider::getKey( const glm::vec2 &pt )
{
	vec2 hp = getHitPercent( pt );
	float height = mHitRect.getHeight();
	float interval = height / (float)mData.size();
	hp.y = std::min( std::max( hp.y, 0.0f ), 1.0f );
	float offset = height * hp.y;
	int index = floor( offset / interval );
	if( index < mData.size() ) {
		return mData[index].mKey;
	}
	return "";
}

void MultiSlider::input( ci::app::MouseEvent &event )
{
	vec2 pt = event.getPos();
	vec2 hp = getHitPercent( event.getPos() );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );

	if( event.isMetaDown() ) {
		setValue( hp.x );
		return;
	}

	if( mHitKey == "" ) {
		mHitKey = getKey( pt );
		mHoverKey = "";
	}

	if( mHitKey != "" ) {
		setValue( mHitKey, lmap<double>( hp.x, 0.0, 1.0, mDataMap[mHitKey]->mMin, mDataMap[mHitKey]->mMax ) );
	}
}

void MultiSlider::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void MultiSlider::mouseUp( ci::app::MouseEvent &event )
{
	if( mHit ) {
#ifdef CINDER_COCOA_TOUCH
		setState( State::NORMAL );
#else
		if( isHit( event.getPos() ) ) {
			setState( State::OVER );
			std::string newKey = getKey( event.getPos() );
			if( newKey != mHoverKey ) {
				mHoverKey = newKey;
				changeState();
				setNeedsDisplay();
			}
		}
		else {
			setState( State::NORMAL );
		}
#endif
		if( (int)mTrigger & (int)Trigger::END ) {
			trigger();
		}
	}
	mHitKey = "";
	mHit = false;
}

void MultiSlider::mouseWheel( ci::app::MouseEvent &event )
{
}

void MultiSlider::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
		std::string newKey = getKey( event.getPos() );
		if( newKey != mHoverKey ) {
			mHoverKey = newKey;
			changeState();
			setNeedsDisplay();
		}
	}
	else {
		setState( State::NORMAL );
		mHoverKey = "";
	}
}

void MultiSlider::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void MultiSlider::keyDown( ci::app::KeyEvent &event )
{
}

void MultiSlider::keyUp( ci::app::KeyEvent &event )
{
}