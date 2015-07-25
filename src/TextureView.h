#pragma once

#include "ControlWithLabel.h"
#include "cinder/gl/Texture.h"

namespace reza { namespace ui {
    
typedef std::shared_ptr<class TextureView> TextureViewRef;
class TextureView : public ControlWithLabel
{
public:
    struct Format {
    public:
        Format() { label(false); }
        Format(const Format &copy) { mLabel = copy.mLabel; }
        Format&	label( bool label ) { mLabel = label; return *this; }
    protected:
        bool mLabel;
        friend class TextureView;
    };
    
    static TextureViewRef create( std::string name, const ci::gl::TextureRef &textureRef, const Format &format = Format() )
    {
        return TextureViewRef( new TextureView( name, textureRef ) );
    }
    
    const std::string getType() override { return "TextureView"; }
    
    void setTexture( const ci::gl::TextureRef &textureRef );
    
protected:
    TextureView( std::string name, const ci::gl::TextureRef &textureRef, const Format &format = Format() );
    void setup() override;
    void draw() override;
    
    virtual void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

    Format mFormat;
    ci::gl::TextureRef mTextureRef;
};
    
} } //namespace reza::ui