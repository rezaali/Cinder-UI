#pragma once

#include "Canvas.h"

namespace reza { namespace ui {
    
typedef std::shared_ptr<class SuperCanvas> SuperCanvasRef;
class SuperCanvas : public Canvas
{
public:
    static SuperCanvasRef create( std::string name, const ci::app::WindowRef& window = ci::app::getWindow() )
    {
        SuperCanvasRef ref = SuperCanvasRef( new SuperCanvas( name, window ) );
        ref->setup();
        return ref; 
    }

    SuperCanvas( std::string title, const ci::app::WindowRef& window );
    const std::string getType() override { return "SuperCanvas"; }
    
    void save( const ci::fs::path &path ) override;
    void load( const ci::fs::path &path ) override;
    
    void clear() override;
    
    LabelRef getLabel();
    void setDeltaTime( float deltaTime );
    void setMinified( bool isMinified );
    bool isMinified();
    void toggleMinified();
    bool isHitHeader( const glm::vec2 &pt );
    void addSubViewToHeader( ViewRef view );

    void setVisible( bool visible ); 

protected:
    void setup() override;
    void minify();
    void maximize();
    
    bool isSaveable() override { return true; }
    
#if defined( CINDER_COCOA_TOUCH )

#else
    virtual void mouseDown( ci::app::MouseEvent &event ) override;
    virtual void mouseUp( ci::app::MouseEvent &event ) override;
    virtual void mouseDrag( ci::app::MouseEvent &event ) override;
#endif
    
    LabelRef mLabelRef;
    std::vector<ViewRef> mHeaderSubViews;

    glm::vec2 mHitPos;
    glm::vec2 mLastPos;
    float mDeltaTime;
    float mLastHitTime;
    bool mIsMinified;
    bool mLabelHit;
};

} } //namespace reza::ui