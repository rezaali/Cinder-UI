#include "Colors.h"
#include "Control.h"

using namespace reza::ui;
using namespace ci;
using namespace std;

Control::Control()
	: View(), mState( State::NORMAL ), mTrigger( Trigger::ALL ), mKeyHit( false ), mKeyModifier( -1 ), mDrawBounds( false ), mDrawBoundsOutline( false ), mDrawBack( true ), mDrawFill( true ), mDrawFillHighlight( false ), mDrawOutline( true ), mDrawOutlineHighlight( false ), mColorClear( ColorClear ), mColorBounds( ColorBounds ), mColorBoundsOutline( ColorBoundsOutline ), mColorBack( ColorBack ), mColorOutline( ColorOutline ), mColorOutlineHighlight( ColorOutlineHighlight ), mColorFill( ColorFill ), mColorFillHighlight( ColorFillHighlight )
{
}

Control::~Control()
{
}

bool Control::isHit( const glm::vec2 &pt )
{
	if( mVisible ) {
		Rectf r = mHitRect;
		r.offset( getOrigin() );
		return r.contains( pt );
	}
	return false;
}

vec2 Control::getHitPercent( const vec2 &pt )
{
	vec2 hp;
	Rectf r = mHitRect;
	r.offset( getOrigin() );
	hp = ( pt - r.getUpperLeft() ) / r.getSize();
	return hp;
}

Rectf Control::getBounds( bool bIncludeSubviews )
{
	Rectf bounds = Rectf( 0.0f, 0.0f, getWidth(), getHeight() );
	Rectf hitRect = mHitRect;
	hitRect.x1 -= mPadding.mLeft;
	hitRect.y1 -= mPadding.mTop;
	hitRect.x2 += mPadding.mRight;
	hitRect.y2 += mPadding.mBottom;
	bounds.include( hitRect );
	if( bIncludeSubviews ) {
		for( auto &it : mSubViews ) {
			bounds.include( it->getBounds( bIncludeSubviews ) );
		}
		bounds.offset( getOrigin( false ) );
	}
	else {
		bounds.offset( getOrigin() );
	}
	return bounds;
}

void Control::setSize( vec2 size )
{
	View::setSize( size );
	mHitRect.set( mPadding.mLeft,
		mPadding.mTop,
		mBounds.getWidth() - mPadding.mRight,
		mBounds.getHeight() - mPadding.mBottom );
}

//PROPERTIES
void Control::setState( State state )
{
	if( mState != state ) {
		mState = state;
		changeState();
	}
}

State Control::getState()
{
	return mState;
}

void Control::changeState()
{
	switch( mState ) {
	case State::NORMAL: {
		setDrawOutline( true );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( false );
		setDrawOutlineHighLight( false );
	} break;

	case State::DOWN: {
		setDrawOutline( true );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( true );
		setDrawOutlineHighLight( true );
	} break;

	case State::OVER: {
		setDrawOutline( true );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( false );
		setDrawOutlineHighLight( true );
	} break;

	case State::DISABLED: {
		setDrawOutline( false );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( false );
		setDrawOutlineHighLight( false );
	} break;

	default:
		break;
	}
}

void Control::setTrigger( Trigger trigger )
{
	mTrigger = trigger;
}

Trigger Control::getTrigger()
{
	return mTrigger;
}

vector<RenderData> Control::render()
{
	vector<RenderData> data;
	drawBounds( data, ( mDrawBounds && mVisible ) ? mColorBounds : mColorClear );
	drawBoundsOutline( data, ( mDrawBoundsOutline && mVisible ) ? mColorBoundsOutline : mColorClear );
	drawBack( data, ( mDrawBack && mVisible ) ? mColorBack : mColorClear );
	drawFill( data, ( mDrawFill && mVisible ) ? mColorFill : mColorClear );
	drawFillHighlight( data, ( mDrawFillHighlight && mVisible ) ? mColorFillHighlight : mColorClear );
	drawOutline( data, ( mDrawOutline && mVisible ) ? mColorOutline : mColorClear );
	drawOutlineHighlight( data, ( mDrawOutlineHighlight && mVisible ) ? mColorOutlineHighlight : mColorClear );
	return data;
}

void Control::drawBounds( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addBounds( data, color );
}

void Control::drawBoundsOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addBoundsOutline( data, color );
}

void Control::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addRect( data, color, mHitRect );
}

void Control::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawBack( data, color );
}

void Control::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawFill( data, color );
}

void Control::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addRectOutline( data, color, mHitRect );
}

void Control::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawOutline( data, color );
}

bool Control::getDrawBounds()
{
	return mDrawBounds;
}

bool Control::getDrawBoundsOutline()
{
	return mDrawBoundsOutline;
}

bool Control::getDrawBack()
{
	return mDrawBack;
}

bool Control::getDrawFill()
{
	return mDrawFill;
}

bool Control::getDrawFillHighLight()
{
	return mDrawFillHighlight;
}

bool Control::getDrawOutline()
{
	return mDrawOutline;
}

bool Control::getDrawOutlineHighLight()
{
	return mDrawOutlineHighlight;
}

void Control::setDrawBounds( bool drawBounds )
{
	if( mDrawBounds != drawBounds ) {
		mDrawBounds = drawBounds;
		setNeedsDisplay();
	}
}

void Control::setDrawBoundsOutline( bool drawBoundsOutline )
{
	if( mDrawBoundsOutline != drawBoundsOutline ) {
		mDrawBoundsOutline = drawBoundsOutline;
		setNeedsDisplay();
	}
}

void Control::setDrawBack( bool drawBack )
{
	if( mDrawBack != drawBack ) {
		mDrawBack = drawBack;
		setNeedsDisplay();
	}
}

void Control::setDrawFill( bool drawFill )
{
	if( mDrawFill != drawFill ) {
		mDrawFill = drawFill;
		setNeedsDisplay();
	}
}

void Control::setDrawFillHighLight( bool drawFillHighlight )
{
	if( mDrawFillHighlight != drawFillHighlight ) {
		mDrawFillHighlight = drawFillHighlight;
		setNeedsDisplay();
	}
}

void Control::setDrawOutline( bool drawOutline )
{
	if( mDrawOutline != drawOutline ) {
		mDrawOutline = drawOutline;
		setNeedsDisplay();
	}
}

void Control::setDrawOutlineHighLight( bool drawOutlineHighlight )
{
	if( mDrawOutlineHighlight != drawOutlineHighlight ) {
		mDrawOutlineHighlight = drawOutlineHighlight;
		setNeedsDisplay();
	}
}

ci::ColorA &Control::getColorBounds()
{
	return mColorBounds;
}

ci::ColorA &Control::getColorBoundsOutline()
{
	return mColorBoundsOutline;
}

ci::ColorA &Control::getColorBack()
{
	return mColorBack;
}

ci::ColorA &Control::getColorOutline()
{
	return mColorOutline;
}

ci::ColorA &Control::getColorOutlineHighlight()
{
	return mColorOutlineHighlight;
}

ci::ColorA &Control::getColorFill()
{
	return mColorFill;
}

ci::ColorA &Control::getColorFillHighlight()
{
	return mColorFillHighlight;
}

void Control::setColorBounds( const ci::ColorA &colorBounds )
{
	mColorBounds = colorBounds;
	setNeedsDisplay();
}

void Control::setColorBoundsOutline( const ci::ColorA &colorBoundsOutline )
{
	mColorBoundsOutline = colorBoundsOutline;
	setNeedsDisplay();
}

void Control::setColorBack( const ci::ColorA &colorBack )
{
	mColorBack = colorBack;
	setNeedsDisplay();
}

void Control::setColorOutline( const ci::ColorA &colorOutline )
{
	mColorOutline = colorOutline;
	setNeedsDisplay();
}

void Control::setColorOutlineHighlight( const ci::ColorA &colorOutlineHighlight )
{
	mColorOutlineHighlight = colorOutlineHighlight;
	setNeedsDisplay();
}

void Control::setColorFill( const ci::ColorA &colorFill )
{
	mColorFill = colorFill;
	setNeedsDisplay();
}

void Control::setColorFillHighlight( const ci::ColorA &colorFillHighlight )
{
	mColorFillHighlight = colorFillHighlight;
	setNeedsDisplay();
}

bool Control::getIsBindedToKey( int key )
{
	auto it = mKeyBindings.find( key );
	if( it != mKeyBindings.end() ) {
		return true;
	}
	return false;
}

bool Control::isBindedToKeyEvent( ci::app::KeyEvent &event )
{
	bool useModifier = ( mKeyModifier != -1 ) ? true : false;
	bool modDown = getIsModifierDown( event );
	bool isBindedToMod = getIsBindedToModifier( event );
	bool isBindedToKey = getIsBindedToKey( event.getCode() );

	if( isBindedToKey ) {
		if( isBindedToMod ) {
			return true;
		}
		else if( !modDown && !useModifier ) {
			return true;
		}
	}
	return false;
}

bool Control::getIsModifierDown( ci::app::KeyEvent &event )
{
	if( event.isMetaDown() ) {
		return true;
	}
	else if( event.isAltDown() ) {
		return true;
	}
	else if( event.isControlDown() ) {
		return true;
	}
	else if( event.isShiftDown() ) {
		return true;
	}
	else if( event.isAccelDown() ) {
		return true;
	}
	return false;
}

bool Control::getIsBindedToModifier( ci::app::KeyEvent &event )
{
	if( event.isMetaDown() && mKeyModifier == app::KeyEvent::META_DOWN ) {
		return true;
	}
	else if( event.isAltDown() && mKeyModifier == app::KeyEvent::ALT_DOWN ) {
		return true;
	}
	else if( event.isControlDown() && mKeyModifier == app::KeyEvent::CTRL_DOWN ) {
		return true;
	}
	return false;
}

void Control::bindToKey( int key, int modifier )
{
	mKeyBindings[key] = false;
	mKeyModifier = modifier;
}

void Control::unbindToKey( int key )
{
	mKeyBindings[key] = false;
}

void Control::unbindAllKeys()
{
	mKeyBindings.clear();
}