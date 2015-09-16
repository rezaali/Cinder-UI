#pragma once

#include "cinder/BSpline.h"
#include "ControlWithLabel.h"

namespace reza { namespace ui {
    
class Canvas;

typedef std::shared_ptr<class BSplineEditor> BSplineEditorRef;
class BSplineEditor : public ControlWithLabel
{
public:
    struct Format {
    public:
        Format() {
            mFontSize = FontSize::SMALL;
            mLabel = true;
            mOptions = true;
            mSticky = false;
            mHeight = -1;
            mMin = glm::vec2( -1.0 );
            mMax = glm::vec2( 1.0 );
            mStickyValue = 0.05;
            mResolution = 512;
            mThreshold = 0.1;
        }
        Format( const Format &copy ) {
            mFontSize = copy.mFontSize;
            mLabel = copy.mLabel;
            mOptions = copy.mOptions;
            mSticky = copy.mSticky;
            mStickyValue = copy.mStickyValue;
            mHeight = copy.mHeight;
            mMin = copy.mMin;
            mMax = copy.mMax;
            mResolution = copy.mResolution;
            mThreshold = copy.mThreshold;
        }
        Format&	fontSize( FontSize fontSize ) { mFontSize = fontSize; return *this; }
        Format&	label( bool label ) { mLabel = label; return *this; }
        Format&	options( bool options ) { mOptions = options; return *this; }
        Format& height( int height ) { mHeight = height; return *this; }
        Format& min( glm::vec2 min ) { mMin = min; return *this; }
        Format& max( glm::vec2 max ) { mMax = max; return *this; }
        Format& sticky( bool sticky = true ) { mSticky = sticky; return *this; }
        Format& stickyValue( float value ) { mStickyValue = value; return *this; }
        Format& threshold( float threshold ) { mThreshold = threshold; return *this; }
        Format& resolution( int resolution ) { mResolution = resolution; return *this; }
    protected:
        glm::vec2 mMin, mMax;
        int mResolution;
        float mHeight;
        float mThreshold;
        float mStickyValue;
        bool mLabel, mSticky, mOptions;
        FontSize mFontSize;
        friend class BSplineEditor;
        friend class Canvas;
    };
    
    static BSplineEditorRef create( const std::string name, ci::BSpline2f spline, const Format &format = Format() )
    {
        return BSplineEditorRef( new BSplineEditor( name, spline, format ) );
    }
        
    const std::string getType() override { return "BSplineEditor"; }
    
    void setSpline( ci::BSpline2f spline );
    ci::BSpline2f getSpline();
    
    int getDegree();
    bool isLoop();
    bool isOpen();
    bool isUniform(); 
    
    void setProperties( int degree, bool loop, bool open );
    void setDegree( int degree );
    void setLoop( bool loop );
    void setOpen( bool open );
    
    void updateSplineRef( bool force = false );
    
    virtual void setCallback( const std::function<void(ci::BSpline2f)> &callback );
    
    void setMax( glm::vec2 max, bool keepValueTheSame = false );
    glm::vec2 getMax();
    void setMin( glm::vec2 min, bool keepValueTheSame = false );
    glm::vec2 getMin();
    void setMinAndMax( glm::vec2 min, glm::vec2 max, bool keepValueTheSame = false );
    void setSplineRef( ci::BSpline2f *spline );
    
    bool isDraggable() override { return true; }
    
    BSplineEditor( std::string name, ci::BSpline2f spline, const Format &format = Format() );
    BSplineEditor( std::string name, ci::BSpline2f *spline, const Format &format = Format() );
    virtual ~BSplineEditor();
    
protected:
    void setup() override;
    void trigger( bool recursive = false ) override;
    
    bool isSaveable() override { return true; }
    ci::JsonTree save() override;
    void load( const ci::JsonTree &data ) override;

    std::vector<RenderData> render() override;
    void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    
    virtual void input( const ci::app::MouseEvent &event ); 
    
    virtual void mouseDown( ci::app::MouseEvent &event ) override;
    virtual void mouseUp( ci::app::MouseEvent &event ) override;
    virtual void mouseWheel( ci::app::MouseEvent &event ) override;
    virtual void mouseMove( ci::app::MouseEvent &event ) override;
    virtual void mouseDrag( ci::app::MouseEvent &event ) override;
        
    ci::vec2 map( const ci::vec2& pt ); 
    ci::vec2 norm( const ci::vec2& pt );
    ci::vec2 expand( const ci::vec2& pt );
    
    Format mFormat;
    int mDegree;
    bool mLoop;
    bool mOpen;    
    ci::BSpline2f *mSplineRef;
    int mHitIndex = -1; 
    std::vector<ci::vec2> mControlPoints;
    bool mUseRef;
    std::function<void(ci::BSpline2f)> mCallbackFn;
};
    
} } //namespace reza::ui