#include "cinder/gl/wrapper.h"

#include "ColorPicker.h"
#include "Globals.h"
#include "Helpers.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

ColorPicker::Format::Format()
{
	fontSize( FontSize::SMALL ).label().align( Alignment::RIGHT ).size( ButtonDimension ).fullScreen( false ).sensitivity( 0.005f ).hsv().relative( true ).alpha( vec2( 0.0, 1.0 ) );
}

ColorPicker::Format::Format( const Format &copy )
{
	mFontSize = copy.mFontSize;
	mLabel = copy.mLabel;
	mAlign = copy.mAlign;
	mSize = copy.mSize;
	mFullScreen = copy.mFullScreen;
	mSensitivity = copy.mSensitivity;
	mRelative = copy.mRelative;
	mColorModel = copy.mColorModel;
	mAlphaLimits = copy.mAlphaLimits;
}

ColorPicker::ColorPicker( std::string name, const ColorA &color, const Format &format )
	: ControlWithLabel(), mColor( color ), mFormat( format ), mCallbackFn( nullptr ), mHitPoint( vec2( 0.0 ) ), mUseRef( false ), mColorRef( new ColorA( color ) )
{
	setName( name );
	setSize( vec2( mFormat.mSize, mFormat.mSize ) );
	setColor( color );
	if( mFormat.mColorModel == ColorModel::CM_RGB ) {
		mLabelNames = { "RED", "GREEN", "BLUE", "ALPHA" };
	}
	else {
		mLabelNames = { "HUE", "SAT", "VAL", "ALPHA" };
	}
}

ColorPicker::ColorPicker( std::string name, ci::ColorA *color, const Format &format )
	: ControlWithLabel(), mColor( *color ), mFormat( format ), mCallbackFn( nullptr ), mHitPoint( vec2( 0.0 ) ), mUseRef( true ), mColorRef( color )

{
	setName( name );
	setSize( vec2( mFormat.mSize, mFormat.mSize ) );
	setColor( *color );
	if( mFormat.mColorModel == ColorModel::CM_RGB ) {
		mLabelNames = { "RED", "GREEN", "BLUE", "ALPHA" };
	}
	else {
		mLabelNames = { "HUE", "SAT", "VAL", "ALPHA" };
	}
}

ColorPicker::~ColorPicker()
{
	if( !mUseRef ) {
		delete mColorRef;
	}
}

bool ColorPicker::isHit( const glm::vec2 &pt )
{
	if( mVisible ) {
		if( mLabelRef ) {
			return ( Control::isHit( pt ) || mLabelRef->isHit( pt ) );
		}
		return Control::isHit( pt );
	}
	return false;
}

void ColorPicker::setup()
{
	setSize( vec2( mFormat.mSize, mFormat.mSize ) );
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
		vec2 labelPosition;
		switch( mFormat.mAlign ) {
		default:
		case Alignment::NONE:
		case Alignment::RIGHT: {
			labelPosition = vec2( getWidth(), ( getHeight() - mLabelRef->getHeight() ) * 0.5f );
		} break;

		case Alignment::LEFT: {
			labelPosition = vec2( 0.0f, ( getHeight() - mLabelRef->getHeight() ) * 0.5f );
			mHitRect.offset( vec2( mLabelRef->getWidth(), 0.0f ) );
		} break;

		case Alignment::CENTER: {
			labelPosition = vec2( mPadding.mLeft, mPadding.mTop );
			setSize( mLabelRef->getSize() + vec2( mPadding.mLeft + mPadding.mRight, mPadding.mTop + mPadding.mBottom ) );
		} break;

		case Alignment::BOTTOM: {
			labelPosition = vec2( 0.0f, getHeight() );
		} break;

		case Alignment::TOP: {
			labelPosition = vec2( 0.0f, 0.0f );
			mHitRect.offset( vec2( 0.0f, mLabelRef->getHeight() ) );
		} break;
		}
		mLabelRef->setOrigin( labelPosition );
		addSubView( mLabelRef );
	}

	for( int i = 0; i < 4; i++ ) {
		LabelRef label = Label::create( mName + toString( i, 0 ) + "_LABEL", "", mFormat.mFontSize );
		addSubView( label );
		mColorLabels.push_back( label );
	}

	View::setup();
}

void ColorPicker::update()
{
	if( mUseRef ) {
		if( ( *mColorRef ) != mColor ) {
			setColor( *mColorRef );
		}
	}
	View::update();
}

void ColorPicker::draw()
{
	if( mHit && mFormat.mFullScreen ) {
		gl::clear( getColor() );
	}
}

void ColorPicker::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getColor() );
	}

	Control::trigger( recursive );
}

ci::JsonTree ColorPicker::save()
{
	JsonTree tree = View::save();
	ColorA color = getColor();
	tree.addChild( JsonTree( "RED", (float)color.r ) );
	tree.addChild( JsonTree( "GREEN", (float)color.g ) );
	tree.addChild( JsonTree( "BLUE", (float)color.b ) );
	tree.addChild( JsonTree( "ALPHA", (float)color.a ) );
	return tree;
}

void ColorPicker::load( const ci::JsonTree &data )
{
	if( data.hasChild( "RED" ) && data.hasChild( "GREEN" ) && data.hasChild( "BLUE" ) && data.hasChild( "ALPHA" ) ) {
		setColor( ColorA( data.getValueForKey<float>( "RED" ), data.getValueForKey<float>( "GREEN" ), data.getValueForKey<float>( "BLUE" ), data.getValueForKey<float>( "ALPHA" ) ) );
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

void ColorPicker::updateLabel()
{
	if( mHit ) {
		vec2 pos = mHitPoint - getOrigin();
		vec2 off = vec2( 0.0f, (float)mFormat.mFontSize + mPadding.mTop + mPadding.mBottom );
		int index = 0;
		vec3 color = mColor.get( mFormat.mColorModel );
		for( auto &it : mColorLabels ) {
			LabelRef ref = it;
			ref->setLabel( mLabelNames[index] + ": " + toString( (float)index < 3 ? color[index] : mColor.a, 3 ) );
			ref->setOrigin( pos + off * (float)index );
			ref->setDrawBounds( true );
			ref->setColorBounds( ColorA( mColor, abs( mColor.a ) ) );
			index++;
		}
	}
	else {
		vec2 zero( 0.0 );
		for( auto &it : mColorLabels ) {
			LabelRef ref = it;
			ref->setLabel( "" );
			ref->setOrigin( zero );
			ref->setDrawBounds( false );
			ref->setColorBounds( mColor );
		}
	}
	if( mLabelRef ) mLabelRef->setLabel( mName + ":[A:" + toString( mColor.a, 3.0 ) + "]" );
}

void ColorPicker::updateColorRef()
{
	setColorBack( mColor );
	*mColorRef = getColor();
}

void ColorPicker::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addRect( data, color, mHitRect );
}

void ColorPicker::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void ColorPicker::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

ColorA ColorPicker::getColor()
{
	if( mFormat.mColorModel == ColorModel::CM_RGB ) {
		return mColor;
	}
	else {
		vec3 color = mColor.get( ColorModel::CM_RGB );
		ColorA result( ColorModel::CM_RGB, color.x, color.y, color.z, mColor.a );
		return result;
	}
}

void ColorPicker::setColor( const ColorA &color )
{
	mColor = color;
	updateColorRef();
	setNeedsDisplay();
}

ColorPicker *ColorPicker::setCallback( const std::function<void( ColorA )> &callback )
{
	mCallbackFn = callback;
	return this;
}

void ColorPicker::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		mHitPoint = event.getPos();
		updateLabel();
		setState( State::DOWN );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}

	View::mouseDown( event );
}

void ColorPicker::mouseUp( ci::app::MouseEvent &event )
{
	if( mHit ) {
#if defined( CINDER_COCOA_TOUCH )
		setState( State::NORMAL );
#else
		if( isHit( event.getPos() ) ) {
			setState( State::OVER );
		}
		else {
			setState( State::NORMAL );
		}
#endif
		mHit = false;
		updateLabel();
		if( (int)mTrigger & (int)Trigger::END ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}

	View::mouseUp( event );
}

void ColorPicker::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}

	View::mouseMove( event );
}

void ColorPicker::mouseWheel( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) && !mHit ) {
		mColor.a -= event.getWheelIncrement() * mFormat.mSensitivity;
		mColor.a = std::max( std::min( mColor.a, mFormat.mAlphaLimits.y ), mFormat.mAlphaLimits.x );
		updateColorRef();
		updateLabel();
		setNeedsDisplay();
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else if( mHit ) {
		vec3 color = mColor.get( mFormat.mColorModel );
		color.z -= event.getWheelIncrement() * mFormat.mSensitivity;
		color.z = std::max( std::min( color.z, 1.0f ), 0.0f );
		mColor.set( mFormat.mColorModel, vec4( color.x, color.y, color.z, mColor.a ) );
		updateColorRef();
		updateLabel();
		setNeedsDisplay();
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
}

void ColorPicker::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		vec2 pt = event.getPos();
		vec3 color = mColor.get( mFormat.mColorModel );

		if( mFormat.mRelative ) {
			//COMMON
			vec2 delta = pt - mHitPoint;
			delta *= mFormat.mSensitivity;
			color.x += delta.x;
			//SPECIFIC
			if( mFormat.mColorModel == ColorModel::CM_HSV ) {
				color.x = color.x > 1.0 ? 0.0 : color.x < 0.0 ? 1.0 : color.x;
				color.y -= delta.y;
			}
			else {
				color.x = std::max( std::min( color.x, 1.0f ), 0.0f );
				color.y += delta.y;
			}
		}
		else {
			//COMMON
			color.x = (float)event.getX() / (float)app::getWindow()->getWidth();
			//SPECIFIC
			if( mFormat.mColorModel == ColorModel::CM_HSV ) {
				color.x = color.x > 1.0 ? 0.0 : color.x < 0.0 ? 1.0 : color.x;
				color.y = 1.0 - ( (float)event.getY() / (float)app::getWindow()->getHeight() );
			}
			else {
				color.x = std::max( std::min( color.x, 1.0f ), 0.0f );
				color.y = (float)event.getY() / (float)app::getWindow()->getHeight();
			}
		}

		color.y = std::max( std::min( color.y, 1.0f ), 0.0f );
		mColor.set( mFormat.mColorModel, vec4( color.x, color.y, color.z, mColor.a ) );
		mHitPoint = pt;
		updateColorRef();
		updateLabel();
		setNeedsDisplay();
		setState( State::DOWN );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}

	View::mouseDrag( event );
}