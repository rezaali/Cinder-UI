#include "Colors.h"
#include "Label.h"
#include "cinder/app/App.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

Label::Label( string name, string label, FontSize fontSize )
	: Control(), mLabel( label ), mFontSize( fontSize ), mShadowOffset( vec2( 0.5, 0.5 ) ), mColorShadow( ColorShadow ), mDrawShadow( false )

{
	setName( name );
	setSize( vec2( getStringWidth( label ) + mPadding.mLeft + mPadding.mRight, static_cast<float>( mFontSize ) + mPadding.mTop + mPadding.mBottom ) );
}

void Label::changeState()
{
	switch( mState ) {
	case State::NORMAL: {
		setDrawOutline( true );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( false );
		setDrawOutlineHighLight( false );
	} break;

	case State::DOWN:
	case State::OVER: {
		setDrawOutline( true );
		setDrawFill( true );
		setDrawBack( true );
		setDrawFillHighLight( true );
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

float Label::getStringWidth( const std::string &s )
{
	if( s.length() == 0 ) return 0.0f;

	vector<RenderData> data;
	ColorA color;
	float h = static_cast<float>( mFontSize );
	vec2 sp = vec2( getSpacing(), 0 );
	vec2 of = vec2( 0, 0 );

	for( int i = 0; i < s.length(); i++ ) {
		of.x += addChar( data, color, s[i], of + sp * float( i ), h );
	}
	return of.x + sp.x * float( s.length() - 1 );
}

void Label::setLabel( const std::string &label )
{
	mLabel = label;
	setSize( vec2( getStringWidth( mLabel ) + mPadding.mLeft + mPadding.mRight, static_cast<float>( mFontSize ) + mPadding.mTop + mPadding.mBottom ) );
	setNeedsDisplay();
}

float Label::getStringHeight( const std::string &s )
{
	return static_cast<float>( mFontSize );
}

float Label::getSpacing()
{
	float h = static_cast<float>( mFontSize );
	return h * 0.25f;
}

glm::vec2 &Label::getShadowOffset()
{
	return mShadowOffset;
}

void Label::setShadowOffset( glm::vec2 shadowOffset )
{
	mShadowOffset = shadowOffset;
	setNeedsDisplay();
}

bool Label::getDrawShadow()
{
	return mDrawShadow;
}

void Label::setDrawShadow( bool drawShadow )
{
	if( mDrawShadow != drawShadow ) {
		mDrawShadow = drawShadow;
		setNeedsDisplay();
	}
}

ci::ColorA &Label::getColorShadow()
{
	return mColorShadow;
}

void Label::setColorShadow( const ci::ColorA &colorShadow )
{
	mColorShadow = colorShadow;
	setNeedsDisplay();
}

float Label::addChar( vector<RenderData> &data, const ColorA &color, const char &c, vec2 off, float height )
{
	float h = height;
	float h2 = height * 0.5;
	float h4 = h2 * 0.5;
	float h8 = h4 * 0.5;
	float h16 = h8 * 0.5;

	vec2 tl = off + vec2( 0, 0 );
	vec2 tc = off + vec2( h4, 0 );
	vec2 tr = off + vec2( h2, 0 );

	vec2 cl = off + vec2( 0, h2 );
	vec2 cc = off + vec2( h4, h2 );
	vec2 cr = off + vec2( h2, h2 );

	vec2 bl = off + vec2( 0, h );
	vec2 bc = off + vec2( h4, h );
	vec2 br = off + vec2( h2, h );

	switch( c ) {
	case 'a':
	case 'A': {
		addChar( data, color, 'H', off, height );
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		return h2;
	} break;

	case 'b':
	case 'B': {
		addChar( data, color, 'A', off, height );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 'c':
	case 'C': {
		addChar( data, color, 'L', off, height );
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		return h2;
	} break;

	case 'd':
	case 'D': {
		addChar( data, color, 'C', off, height );
		addLine( data, color, tr, br );
		return h2;
	} break;

	case 'e':
	case 'E': {
		addChar( data, color, 'C', off, height );
		addChar( data, color, '-', off, height );
		return h2;
	} break;

	case 'f':
	case 'F': {
		addLine( data, color, tl, bl );
		addChar( data, color, '-', off, height );
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		return h2;
	} break;

	case 'g':
	case 'G': {
		addChar( data, color, 'C', off, height );
		addLine( data, color, cr, br );
		addLine( data, color, cc, cr );
		return h2;
	} break;

	case 'h':
	case 'H': {
		addLine( data, color, tl, bl );
		addLine( data, color, tr, br );
		addChar( data, color, '-', off, height );
		return h2;
	} break;

	case 'i':
	case 'I': {
		addChar( data, color, 'T', off, height );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 'j':
	case 'J': {
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		addLine( data, color, tr, br );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 'k':
	case 'K': {
		addLine( data, color, tl, bl );
		addLine( data, color, cl, tr );
		addLine( data, color, cl, br );
		return h2;
	} break;

	case 'l':
	case 'L': {
		addLine( data, color, tl, bl );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 'm':
	case 'M': {
		addLine( data, color, tl, bl );
		addLine( data, color, tr, br );
		addLine( data, color, tl, tr );
		addLine( data, color, tc, bc );
		return h2;
	} break;

	case 'n':
	case 'N': {
		addLine( data, color, tl, bl );
		addLine( data, color, tr, br );
		addLine( data, color, tl, br );
		return h2;
	} break;

	case 'o':
	case 'O': {
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		addLine( data, color, tl, bl );
		addLine( data, color, tr, br );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 'p':
	case 'P': {
		addChar( data, color, 'F', off, height );
		addLine( data, color, tr, cr );
		return h2;
	} break;

	case 'q':
	case 'Q': {
		addChar( data, color, 'O', off, height );
		addLine( data, color, br - vec2( h4, h4 ), br );
		return h2;
	} break;

	case 'r':
	case 'R': {
		addChar( data, color, 'P', off, height );
		addLine( data, color, cl, br );
		return h2;
	} break;

	case 's':
	case 'S': {
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		addLine( data, color, tl, cl );
		addChar( data, color, '-', off, height );
		addLine( data, color, cr, br );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case 't':
	case 'T': {
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		addLine( data, color, tc, bc );
		return h2;
	} break;

	case 'u':
	case 'U': {
		addChar( data, color, 'L', off, height );
		addLine( data, color, tr, br );
		return h2;
	} break;

	case 'v':
	case 'V': {
		addLine( data, color, tl, bc );
		addLine( data, color, bc, tr );
		return h2;
	} break;

	case 'w':
	case 'W': {
		addLine( data, color, tl, bl );
		addLine( data, color, bl, br );
		addLine( data, color, tr, br );
		addLine( data, color, tc, bc );
		return h2;
	} break;

	case 'x':
	case 'X': {
		addLine( data, color, tl, br );
		addLine( data, color, bl, tr );
		return h2;
	} break;

	case 'y':
	case 'Y': {
		addLine( data, color, tl, cc );
		addLine( data, color, cc, tr );
		addLine( data, color, cc, bc );
		return h2;
	} break;

	case 'z':
	case 'Z': {
		addChar( data, color, '-', off + vec2( 0, -h2 ), height );
		addLine( data, color, tr, bl );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case '_': {
		addChar( data, color, '-', off + vec2( 0, h2 ), height );
		return h2;
	} break;

	case '-': {
		addLine( data, color, cl, cr );
		return h2;
	} break;

	case ' ': {
		return h2;
	} break;

	case '.': {
		addLine( data, color, bc + vec2( 0.0, -h8 ), bc );
		return h2;
	} break;

	case ':': {
		addLine( data, color, cc + vec2( 0.0, -h8 ), cc + vec2( 0.0, -h4 ) );
		addLine( data, color, cc + vec2( 0.0, h8 ), cc + vec2( 0.0, h4 ) );
		return h2;
	} break;

	case ';': {
		addLine( data, color, cc + vec2( 0.0, -h8 ), cc + vec2( 0.0, -h4 ) );
		addLine( data, color, cc + vec2( 0.0, h8 ), cc + vec2( -h16, h4 + h8 ) );
		return h2;
	} break;

	case '/': {
		addLine( data, color, bl, tr );
		return h2;
	} break;

	case '\\': {
		addLine( data, color, tl, br );
		return h2;
	} break;

	case '0': {
		addChar( data, color, 'O', off, height );
		addChar( data, color, '/', off, height );
		return h2;
	} break;

	case '1': {
		addLine( data, color, tl, tc );
		addLine( data, color, tc, bc );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case '2': {
		addLine( data, color, tl, tr );
		addChar( data, color, '-', off, height );
		addChar( data, color, '_', off, height );
		addLine( data, color, tr, cr );
		addLine( data, color, cl, bl );
		return h2;
	} break;

	case '3': {
		addLine( data, color, tl, tr );
		addChar( data, color, '-', off, height );
		addChar( data, color, '_', off, height );
		addLine( data, color, tr, br );
		return h2;
	} break;

	case '4': {
		addLine( data, color, tl, cl );
		addChar( data, color, '-', off, height );
		addLine( data, color, tr, br );
		return h2;
	} break;

	case '5': {
		addChar( data, color, 'S', off, height );
		return h2;
	} break;

	case '6': {
		addLine( data, color, tl, tr );
		addChar( data, color, '-', off, height );
		addChar( data, color, '_', off, height );
		addLine( data, color, tl, bl );
		addLine( data, color, cr, br );
		return h2;
	} break;

	case '7': {
		addLine( data, color, tl, tr );
		addChar( data, color, '/', off, height );
		return h2;
	} break;

	case '8': {
		return addChar( data, color, 'B', off, height );
	} break;

	case '9': {
		addLine( data, color, tl, tr );
		addLine( data, color, tl, cl );
		addLine( data, color, tr, br );
		addChar( data, color, '-', off, height );
		return h2;
	} break;

	case '~': {
		vec2 _l = cl + vec2( h8, -h8 );
		vec2 _r = cr - vec2( h8, -h8 );
		addLine( data, color, cl, _l );
		addLine( data, color, _l, _r );
		addLine( data, color, _r, cr );
		return h2;
	} break;

	case '!': {
		addChar( data, color, '.', off, height );
		addLine( data, color, tc, bc - vec2( 0.0, h4 ) );
		return h2;
	} break;

	case '|': {
		addLine( data, color, tc + vec2( 0.0, -h8 ), bc + vec2( 0.0, h8 ) );
		return h2;
	} break;

	case '$': {
		addChar( data, color, '5', off, height );
		addChar( data, color, '|', off, height );
		return h2;
	} break;

	case '*': {
		addChar( data, color, '+', off, height );
		addLine( data, color, cl - vec2( -h16, -h8 - h16 ), cr - vec2( h16, h8 + h16 ) );
		addLine( data, color, cl - vec2( -h16, h8 + h16 ), cr - vec2( h16, -h8 - h16 ) );
		return h2;
	} break;

	case '+': {
		addChar( data, color, '-', off, height );
		addLine( data, color, cc - vec2( 0.0, h4 ), cc + vec2( 0.0, h4 ) );
		return h2;
	} break;

	case '^': {
		addLine( data, color, tl + vec2( 0.0, h4 ), tc );
		addLine( data, color, tc, tr + vec2( 0.0, h4 ) );
		return h2;
	} break;

	case '(': {
		vec2 _o = vec2( h8, 0 );
		addLine( data, color, tc + _o, cl + _o );
		addLine( data, color, cl + _o, bc + _o );
		return h2;
	} break;

	case ')': {
		vec2 _o = vec2( -h8, 0 );
		addLine( data, color, tc + _o, cr + _o );
		addLine( data, color, cr + _o, bc + _o );
		return h2;
	} break;

	case '<': {
		addLine( data, color, cl, cr - vec2( 0.0, h4 ) );
		addLine( data, color, cl, cr + vec2( 0.0, h4 ) );
		return h2;
	} break;

	case '>': {
		addLine( data, color, cl - vec2( 0.0, h4 ), cr );
		addLine( data, color, cl + vec2( 0.0, h4 ), cr );
		return h2;
	} break;

	case '@': {
		addLine( data, color, tl, tr );
		addLine( data, color, tl, bl );
		addLine( data, color, tr, br - vec2( 0.0, h4 ) );
		addLine( data, color, br - vec2( 0.0, h4 ), bl + vec2( h8, -h4 ) );
		addLine( data, color, bl + vec2( h8, -h4 ), tl + vec2( h8, h4 ) );
		addLine( data, color, tl + vec2( h8, h4 ), tr + vec2( -h8, h4 ) );
		addLine( data, color, tr + vec2( -h8, h4 ), br - vec2( h8, h8 ) );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case '#': {
		vec2 ox = vec2( h8, 0 );
		vec2 oy = vec2( 0, h8 );
		addLine( data, color, tc - ox, bc - ox );
		addLine( data, color, tc + ox, bc + ox );
		addLine( data, color, cl - oy, cr - oy );
		addLine( data, color, cl + oy, cr + oy );
		return h2;
	} break;

	case '%': {
		vec2 of = vec2( 0.0, h4 );
		addLine( data, color, bl, tr );
		addLine( data, color, tl, tc );
		addLine( data, color, tc, cc - of );
		addLine( data, color, cc - of, tl + of );
		addLine( data, color, tl, tl + of );
		addLine( data, color, br, bc );
		addLine( data, color, bc, cc + of );
		addLine( data, color, cc + of, br - of );
		addLine( data, color, br, br - of );
		return h2;
	} break;

	case ',': {
		addLine( data, color, bc + vec2( 0.0, -h8 ), bc + vec2( -h8, h8 ) );
		return h2;
	} break;

	case '&': {
		addLine( data, color, tl, tr );
		addChar( data, color, 'X', off, height );
		addChar( data, color, '_', off, height );
		return h2;
	} break;

	case '=': {
		addLine( data, color, cl + vec2( 0.0, -h8 ), cr + vec2( 0.0, -h8 ) );
		addLine( data, color, cl + vec2( 0.0, h8 ), cr + vec2( 0.0, h8 ) );
		return h2;
	} break;

	case '{': {
		addLine( data, color, tc, tr - vec2( h8, 0 ) );
		addLine( data, color, tc, bc );
		addLine( data, color, cl + vec2( h8, 0 ), cc );
		addLine( data, color, bc, br - vec2( h8, 0 ) );
		return h2;
	} break;

	case '}': {
		addLine( data, color, tl + vec2( h8, 0 ), tc );
		addLine( data, color, tc, bc );
		addLine( data, color, cr - vec2( h8, 0 ), cc );
		addLine( data, color, bl + vec2( h8, 0 ), bc );
		return h2;
	} break;

	case ']': {
		addLine( data, color, tl + vec2( h8, 0 ), tc );
		addLine( data, color, tc, bc );
		addLine( data, color, bl + vec2( h8, 0 ), bc );
		return h2;
	} break;

	case '[': {
		addLine( data, color, tc, tr - vec2( h8, 0 ) );
		addLine( data, color, tc, bc );
		addLine( data, color, bc, br - vec2( h8, 0 ) );
		return h2;
	} break;

	case '?': {
		addLine( data, color, tl, tr );
		addLine( data, color, tl, tl + vec2( h8, h4 ) );
		addLine( data, color, tr, cc );
		addLine( data, color, cc, bc - vec2( 0.0, h4 ) );
		addChar( data, color, '.', off, height );
		return h2;
	} break;

	case '"': {
		addLine( data, color, tc - vec2( h8, 0 ), tc + vec2( -h8, h4 ) );
		addLine( data, color, tc + vec2( h8, 0 ), tc + vec2( h8, h4 ) );
		return h2;
	} break;

	case '\'': {
		addLine( data, color, tc - vec2( 0, 0 ), tc + vec2( 0, h4 ) );
		return h2;
	} break;

	default:
		break;
	}
	return 0;
}

void Label::addString( vector<RenderData> &data, const ColorA &color, const string &s, const vec2 &off, float height )
{
	float h = height;
	float p = h * .25;
	vec2 sp = vec2( p, 0 );
	vec2 of = vec2( 0, 0 );

	for( int i = 0; i < s.length(); i++ ) {
		of.x += addChar( data, color, s[i], off + of + sp * float( i ), height );
	}
}

vector<RenderData> Label::render()
{
	vector<RenderData> data;
	drawBounds( data, ( mDrawBounds && mVisible ) ? mColorBounds : mColorClear );
	drawBoundsOutline( data, ( mDrawBoundsOutline && mVisible ) ? mColorBoundsOutline : mColorClear );
	drawShadow( data, ( mDrawShadow && mVisible ) ? mColorShadow : mColorClear );
	switch( mState ) {
	case State::DISABLED:
		drawBack( data, ( mDrawBack && mVisible ) ? mColorBack : mColorClear );
		break;

	case State::NORMAL:
		drawFill( data, ( mDrawFill && mVisible ) ? mColorFill : mColorClear );
		break;

	case State::OVER:
	case State::DOWN:
		drawFillHighlight( data, ( mDrawFillHighlight && mVisible ) ? mColorFillHighlight : mColorClear );
		break;

	default:
		drawBack( data, ( mDrawBack && mVisible ) ? mColorBack : mColorClear );
		break;
	}
	drawOutline( data, ( mDrawOutline && mVisible ) ? mColorOutline : mColorClear );
	drawOutlineHighlight( data, ( mDrawOutlineHighlight && mVisible ) ? mColorOutlineHighlight : mColorClear );
	for( int i = data.size(); i < 1998; i++ ) {
		data.push_back( RenderData() );
	}
	return data;
}

void Label::drawShadow( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addString( data, color, mLabel, vec2( mPadding.mLeft + mShadowOffset.x, mPadding.mTop + mShadowOffset.y ), (float)mFontSize );
}

void Label::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addString( data, color, mLabel, vec2( mPadding.mLeft, mPadding.mTop ), (float)mFontSize );
}

void Label::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addString( data, color, mLabel, vec2( mPadding.mLeft, mPadding.mTop ), (float)mFontSize );
}

void Label::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	addString( data, color, mLabel, vec2( mPadding.mLeft, mPadding.mTop ), (float)mFontSize );
}

void Label::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void Label::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
}
