#include "cinder/gl/draw.h"

#include "TextureView.h"

using namespace reza::ui;
using namespace cinder;
using namespace cinder::gl;
using namespace std;

TextureView::TextureView( std::string name, const ci::gl::TextureRef &textureRef, const Format &format )
	: ControlWithLabel(), mTextureRef( textureRef ), mFormat( format )
{
	setName( name );
}

void TextureView::setup()
{
	if( !mLabelRef && mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, FontSize::SMALL );
		addSubView( mLabelRef );
		mLabelRef->setOrigin( vec2( 0.0f, getHeight() ) );
	}
	View::setup();
}

void TextureView::draw()
{
	Rectf rect = mHitRect;
	rect += getOrigin();
	if( mFormat.mHeight > 0 ) {
		rect.y2 = rect.y1 + mFormat.mHeight;
	}
	if( mTextureRef ) {
		gl::draw( mTextureRef, rect );
	}
	Control::draw();
}

void TextureView::setTexture( const ci::gl::TextureRef &textureRef )
{
	mTextureRef = textureRef;
}

void TextureView::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void TextureView::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void TextureView::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void TextureView::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
}

void TextureView::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
}
