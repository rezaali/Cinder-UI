#include "Canvas.h"
#include "Globals.h"

using namespace reza::ui;
using namespace cinder;
using namespace cinder::gl;
using namespace std;

Canvas::Canvas( std::string name, const app::WindowRef& window ) : Control(), mWindowRef(window), mCanvasWidth(CanvasWidth), mSliderHeight(SliderHeight), mGraphHeight(GraphHeight), mButtonDimension(ButtonDimension), mSpacerHeight(SpacerHeight), mDirection(Direction::SOUTH), mAlignment(Alignment::LEFT), mFontsize(FontSize::MEDIUM)
{
    setName( name ); 
    setSize( vec2( mCanvasWidth, mCanvasWidth ) );
    setDrawFill( false );
    setDrawFillHighLight( false );
    setDrawOutline( false );
    setDrawOutlineHighLight( false );
    enable();
}

Canvas::~Canvas()
{
    setEnabled(false);
}

void Canvas::clear()
{
    clearPlacer();
    View::clear();
}

void Canvas::save( const ci::fs::path &path )
{
    JsonTree tree = View::save();
    tree.write( path );
}

void Canvas::load( const ci::fs::path &path )
{
    if( fs::exists( path ) ) {
        JsonTree tree( loadFile( path ) );
        View::load( tree );
        trigger();
    }
}

void Canvas::setWindow( ci::app::WindowRef window )
{
    mWindowRef = window;
    if( isEnabled() ) { enable(); } else { disable(); }
}

app::WindowRef Canvas::getWindow()
{
    return mWindowRef;
}

void Canvas::setSize( vec2 size )
{
    View::setSize( size );
    mHitRect = Rectf( 0.0f, 0.0f, size.x, size.y );
}

void Canvas::setEnabled( bool enabled )
{
    if(mEnabled != enabled)
    {
        if(enabled)
        {
            enable();
        }
        else
        {
            disable();
        }
    }
}

void Canvas::enable()
{
    mEnabled = true;
    if( mWindowRef && mWindowRef->isValid() ) {
        enableUpdateCallback();
#if defined( CINDER_COCOA_TOUCH )
        enableTouchCallbacks();
#else
        enableMouseCallbacks();
        enableKeyboardCallbacks();
#endif
    }
}

void Canvas::disable()
{
    mEnabled = false;
    if( mWindowRef && mWindowRef->isValid() ) {
        disableUpdateCallback();
#if defined( CINDER_COCOA_TOUCH )
        disableTouchCallbacks();
#else
        disableMouseCallbacks();
        disableKeyboardCallbacks();
#endif
    }
}

void Canvas::clearPlacer()
{
    mLastAddedSubViews.clear();
}

void Canvas::setPlacer( ViewRef subView )
{
    if( !subView ) {
        return;
    }
    std::remove( mLastAddedSubViews.begin(), mLastAddedSubViews.end(), subView );
    mLastAddedSubViews.push_back( subView );
}

void Canvas::setSliderHeight( float height )
{
    mSliderHeight = height;
}

void Canvas::setGraphHeight( float height )
{
    mGraphHeight = height;
}

void Canvas::setButtonDimension( float dimension )
{
    mButtonDimension = dimension;
}

void Canvas::setSpacerHeight( float height )
{
    mSpacerHeight = height;
}

float Canvas::getSliderHeight()
{
    return mSliderHeight;
}

float Canvas::getGraphHeight()
{
    return mGraphHeight;
}

float Canvas::getButtonDimension()
{
    return mButtonDimension;
}

float Canvas::getSpacerHeight()
{
    return mSpacerHeight; 
}

void Canvas::setSubViewDirection( Direction direction )
{
    mDirection = direction;
}

void Canvas::setSubViewAlignment( Alignment alignment )
{
    mAlignment = alignment;
}

void Canvas::setFontSize( FontSize fontSize )
{
    mFontsize = fontSize;
}

Alignment Canvas::getSubViewAlignment()
{
    return mAlignment;
}

Direction Canvas::getSubViewDirection()
{
    return mDirection;
}

FontSize Canvas::getSubViewFontSize()
{
    return mFontsize;
}

ViewRef Canvas::addSubViewPosition( ViewRef subView, Direction direction, Alignment alignment, bool reAdd )
{
    if(!reAdd) addSubView( subView );
    
    ViewRef lastAdded = mLastAddedSubViews.empty() ? nullptr : mLastAddedSubViews.back();
    Rectf viewRect = subView->getBounds( true );
    if( lastAdded != NULL )
    {
        vec2 lastOrigin = lastAdded->getOrigin( false );
        Rectf lastRect = lastAdded->getBounds( true );
        
        switch( direction )
        {
            case Direction::NONE:
            case Direction::SOUTH:
            {
                subView->setOrigin( lastOrigin + vec2( 0.0f, lastRect.getHeight() + mPadding.mBottom ) );
            }
                break;
            case Direction::NORTH:
            {
                subView->setOrigin( lastOrigin + vec2( 0.0f, -viewRect.getHeight() - mPadding.mBottom ) );
                break;
            }
            case Direction::EAST:
            {
                subView->setOrigin( lastOrigin + vec2( lastRect.getWidth() + mPadding.mRight, 0.0f ) );
                break;
            }
            case Direction::WEST:
            {
                subView->setOrigin( lastOrigin + vec2( - viewRect.getWidth() - mPadding.mBottom, 0.0f ) );
            }
                break;
        }
    }
    else
    {
        subView->setOrigin( vec2( mPadding.mTop, mPadding.mLeft ) );
    }

    vec2 viewOrigin = subView->getOrigin( false );
    switch(alignment) {
        case Alignment::LEFT:
            subView->setOrigin( vec2( mPadding.mLeft, viewOrigin.y ) );
            break;
        case Alignment::NONE:
            break;
        case Alignment::CENTER:
            subView->setOrigin( vec2( (getWidth() - viewRect.getWidth())*0.5f, viewOrigin.y ) );
            break;
        case Alignment::RIGHT:
            subView->setOrigin( vec2( ( getWidth() - viewRect.getWidth() - mPadding.mRight ), viewOrigin.y ) );
            break;
        case Alignment::TOP:
            subView->setOrigin( vec2( viewOrigin.x, mPadding.mTop ) );
            break;
        case Alignment::BOTTOM:
            subView->setOrigin( vec2( viewOrigin.x, getHeight() - viewRect.getHeight() - mPadding.mBottom ) );
            break;
    }
    
    if( viewRect.getHeight() != 0 || viewRect.getWidth() != 0 )
    {
        mLastAddedSubViews.push_back( subView );
    }
    return subView;
}

ViewRef Canvas::addSubViewDown( ViewRef subView, Alignment alignment, bool reAdd )
{
    return addSubViewPosition( subView, Direction::SOUTH, alignment, reAdd );
}

ViewRef Canvas::addSubViewUp( ViewRef subView, Alignment alignment, bool reAdd )
{
    return addSubViewPosition( subView, Direction::NORTH, alignment, reAdd );
}

ViewRef Canvas::addSubViewRight( ViewRef subView, Alignment alignment, bool reAdd )
{
    return addSubViewPosition( subView, Direction::EAST, alignment, reAdd );
}

ViewRef Canvas::addSubViewLeft( ViewRef subView, Alignment alignment, bool reAdd )
{
    return addSubViewPosition( subView, Direction::WEST, alignment, reAdd );
}

ViewRef Canvas::addSubViewSouthOf( ViewRef subView, std::string referenceName, bool reAdd )
{
    if( !reAdd ) addSubView( subView );
    
    ViewRef referenceSubView = getSubView( referenceName );
    
    if( referenceSubView )
    {
        Rectf referenceBounds = referenceSubView->getBounds( true );
        subView->setOrigin( referenceSubView->getOrigin( false ) + vec2( 0.0f, referenceBounds.getHeight() + mPadding.mBottom ) );
    }
    else
    {
        subView->setOrigin( vec2( mPadding.mLeft, mPadding.mTop ) );
    }
    
    mLastAddedSubViews.push_back( subView );
    return subView;
}

ViewRef Canvas::addSubViewNorthOf( ViewRef subView, std::string referenceName, bool reAdd )
{
    if( !reAdd ) addSubView( subView );
    
    ViewRef referenceSubView = getSubView( referenceName );
    
    if( referenceSubView )
    {
        Rectf viewBounds = subView->getBounds( true );
        subView->setOrigin( referenceSubView->getOrigin( false ) + vec2( 0.0f, -viewBounds.getHeight() - mPadding.mTop ) );
    }
    else
    {
        subView->setOrigin( vec2( mPadding.mLeft, mPadding.mTop ) );
    }
    
    mLastAddedSubViews.push_back( subView );
    return subView;
}

ViewRef Canvas::addSubViewWestOf( ViewRef subView, std::string referenceName, bool reAdd )
{
    if( !reAdd ) addSubView( subView );
    
    ViewRef referenceSubView = getSubView( referenceName );
    
    if( referenceSubView )
    {
        Rectf viewBounds = subView->getBounds( true );
        subView->setOrigin( referenceSubView->getOrigin( false ) + vec2( -viewBounds.getWidth() - mPadding.mLeft, 0.0f ) );
    }
    else
    {
        subView->setOrigin( vec2( mPadding.mLeft, mPadding.mTop ) );
    }
    
    mLastAddedSubViews.push_back( subView );
    return subView;
}

ViewRef Canvas::addSubViewEastOf( ViewRef subView, std::string referenceName, bool reAdd )
{
    if( !reAdd ) addSubView( subView );
    
    ViewRef referenceSubView = getSubView( referenceName );
    
    if( referenceSubView )
    {
        Rectf referenceBounds = referenceSubView->getBounds( true );
        subView->setOrigin( referenceSubView->getOrigin( false ) + vec2( referenceBounds.getWidth() + mPadding.mRight, 0.0f ) );
    }
    else
    {
        subView->setOrigin( vec2( mPadding.mLeft, mPadding.mTop ) );
    }
    
    mLastAddedSubViews.push_back( subView );
    return subView;
}

void Canvas::enableUpdateCallback()
{
    mPostDrawCb = mWindowRef->getSignalPostDraw().connect( [ this ] () {
        update();
        draw();
    } );
}

void Canvas::draw()
{
    if( !mSetup ) { setup(); }
    
    getRenderData();
    
    if( !mGlslProgRef ) {
        setupShader();
    } else {
        
        gl::ScopedDepthTest scpDrd( false );
        gl::ScopedDepthWrite scpDwt( false );
        gl::ScopedBlendAlpha scpAlp;
        gl::pushMatrices();
        gl::setMatricesWindow( app::getWindowWidth(), app::getWindowHeight() );
        
        gl::ScopedVao           scopedVao( mVaoRef );
        gl::ScopedGlslProg      scopedShader( mGlslProgRef );
            
        mGlslProgRef->uniform( "uModelViewProjection", gl::getModelViewProjection() );
        
        gl::enableAlphaBlending();
        gl::disableDepthRead();
        
        glDisable( GL_CULL_FACE );
        glDrawArrays( GL_TRIANGLES, 0, mRenderData.size() );
        
        View::draw();
        
        gl::popMatrices();
    }
}

void Canvas::disableUpdateCallback()
{
    mPostDrawCb.disconnect();
}

#if defined( CINDER_COCOA_TOUCH )

void Canvas::enableTouchCallbacks()
{
    mTouchesBeganCb = mWindowRef->getSignalTouchesBegan().connect( [this]( ci::app::TouchEvent event ) {
        touchesBegan( event );
    } );
    mTouchesMovedCb = mWindowRef->getSignalTouchesMoved().connect( [this]( ci::app::TouchEvent event ) {);
        touchesMoved( event );
    } );
    mTouchesEndedCb = mWindowRef->getSignalTouchesEnded().connect( [this]( ci::app::TouchEvent event ) {
        touchesEnded( event );
    } );
}

void Canvas::disableTouchCallbacks()
{
    mCbTouchesBegan.disconnect();
    mCbTouchesMoved.disconnect();
    mCbTouchesEnded.disconnect();
}

#else

void Canvas::enableMouseCallbacks()
{
    mMouseDownCb = mWindowRef->getSignalMouseDown().connect( [this]( ci::app::MouseEvent event ) {
        mouseDown( event );
    } );
    mMouseUpCb = mWindowRef->getSignalMouseUp().connect( [this]( ci::app::MouseEvent event ) {
        mouseUp( event );
    } );
    mMouseMoveCb = mWindowRef->getSignalMouseMove().connect( [this]( ci::app::MouseEvent event ) {
        mouseMove( event );
    } );
    mMouseWheelCb = mWindowRef->getSignalMouseWheel().connect( [this]( ci::app::MouseEvent event ) {
        mouseWheel( event );
    } );
    mMouseDragCb = mWindowRef->getSignalMouseDrag().connect( [this]( ci::app::MouseEvent event ) {
        mouseDrag( event );
    } );
}

//Mouse Callbacks
void Canvas::disableMouseCallbacks()
{
    mMouseDownCb.disconnect();
    mMouseUpCb.disconnect();
    mMouseMoveCb.disconnect();
    mMouseWheelCb.disconnect();
    mMouseDragCb.disconnect();
}

//KeyBoard Callbacks
void Canvas::enableKeyboardCallbacks()
{
    mKeyDownCb = mWindowRef->getSignalKeyDown().connect( [this]( ci::app::KeyEvent event ) {
        keyDown( event );
    } );
    mKeyUpCb = mWindowRef->getSignalKeyUp().connect( [this]( ci::app::KeyEvent event ) {
        keyUp( event );
    } );
}

//KeyBoard Callbacks
void Canvas::disableKeyboardCallbacks()
{
    mKeyDownCb.disconnect();
    mKeyUpCb.disconnect();
}

#endif

void Canvas::setupShader()
{
    try {
        mGlslProgRef = gl::GlslProg::create( gl::GlslProg::Format()
                                            .vertex( CI_GLSL( 330,
                                                             uniform mat4 uModelViewProjection;
                                                             
                                                             in vec3 iPosition;
                                                             in vec2 iUv;
                                                             in vec4 iColor;
                                                             
                                                             out vec2 vUv;
                                                             out vec4 vColor;
                                                             
                                                             void main() {
                                                                 vColor       = iColor;
                                                                 vUv          = iUv;
                                                                 gl_Position  = uModelViewProjection * vec4( iPosition, 1.0 );
                                                             } ) )
                                            .fragment( CI_GLSL( 330,
                                                               in vec2 vUv;
                                                               in vec4 vColor;
                                                               
                                                               out vec4 oColor;
                                                               
                                                               void main()
                                                               {
                                                                   oColor = vColor;
                                                               } ) )
                                            .attribLocation( "iPosition", 0 )
                                            .attribLocation( "iUv", 1 )
                                            .attribLocation( "iColor", 2 )
                                            );
    }
    catch( gl::GlslProgCompileExc exc ){
        cout << "Problem Compiling UI Shader" << endl << exc.what() << endl;
    }
}

void Canvas::setupBuffers()
{
    mVboRef = gl::Vbo::create( GL_ARRAY_BUFFER,
                              mRenderData.size() * sizeof(RenderData),
                              mRenderData.data(),
                              GL_DYNAMIC_DRAW );
    mVaoRef = gl::Vao::create();
    
    gl::ScopedVao scopedVao( mVaoRef );
    gl::ScopedBuffer scopedBuffer( mVboRef );
    gl::enableVertexAttribArray( 0 );
    gl::enableVertexAttribArray( 1 );
    gl::enableVertexAttribArray( 2 );
    gl::vertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( RenderData ), (const GLvoid*) offsetof( RenderData, pos ) );
    gl::vertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( RenderData ), (const GLvoid*) offsetof( RenderData, uv ) );
    gl::vertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( RenderData ), (const GLvoid*) offsetof( RenderData, color ) );
}

void Canvas::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{
    addBoundsOutline( data, color );
}

EnvelopeRef Canvas::addEnvelope( const std::string name, const Envelope::Format& format )
{
    EnvelopeRef ref = Envelope::create( name, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, format.mHeight < 0.0f ? w : format.mHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

QuaternionOrdererRef Canvas::addQuaternionOrderer( const std::string name, const QuaternionOrderer::Format& format )
{
    QuaternionOrdererRef ref = QuaternionOrderer::create( name, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, format.mHeight < 0.0f ? w : format.mHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

BSplineEditorRef Canvas::addBSplineEditor( const std::string name, BSpline2f spline, BSplineEditor::Format format )
{
    BSplineEditorRef ref = BSplineEditor::create( name, spline, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, format.mHeight < 0.0f ? w : format.mHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SlideriRef Canvas::addSlideri( const std::string name, int value, int min, int max, Slideri::Format format )
{
    SlideriRef ref = SliderT<int>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SliderfRef Canvas::addSliderf( const std::string name, float value, float min, float max, Sliderf::Format format )
{
    SliderfRef ref = SliderT<float>::create( name, value, min, max, format  );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SliderdRef Canvas::addSliderd( const std::string name, double value, double min, double max, Sliderd::Format format )
{
    SliderdRef ref = SliderT<double>::create( name, value, min, max, format  );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SlideriRef Canvas::addSlideri( const std::string name, int *value, int min, int max, Slideri::Format format)
{
    SlideriRef ref = SliderT<int>::create( name, value, min, max, format  );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SliderfRef Canvas::addSliderf( const std::string name, float *value, float min, float max, Sliderf::Format format )
{
    SliderfRef ref = SliderT<float>::create( name, value, min, max, format  );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SliderdRef Canvas::addSliderd( const std::string name, double *value, double min, double max, Sliderd::Format format )
{
    SliderdRef ref = SliderT<double>::create( name, value, min, max, format  );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialeriRef Canvas::addDialeri( const std::string name, int value, int min, int max, Dialeri::Format format )
{
    DialeriRef ref = DialerT<int>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialeriRef Canvas::addDialeri( const std::string name, int *value, int min, int max, Dialeri::Format format )
{
    DialeriRef ref = DialerT<int>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialerfRef Canvas::addDialerf( const std::string name, float value, float min, float max, Dialerf::Format format )
{
    DialerfRef ref = DialerT<float>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialerfRef Canvas::addDialerf( const std::string name, float *value, float min, float max, Dialerf::Format format )
{
    DialerfRef ref = DialerT<float>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialerdRef Canvas::addDialerd( const std::string name, double value, double min, double max, Dialerd::Format format )
{
    DialerdRef ref = DialerT<double>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

DialerdRef Canvas::addDialerd( const std::string name, double *value, double min, double max, Dialerd::Format format )
{
    DialerdRef ref = DialerT<double>::create( name, value, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

MultiSliderRef Canvas::addMultiSlider( const std::string name, const std::vector<MultiSlider::Data> data,  MultiSlider::Format format )
{
    MultiSliderRef ref = MultiSlider::create( name, data, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangeiRef Canvas::addRangei( const std::string name, int valueLow, int valueHigh, int min, int max, Rangei::Format format )
{
    RangeiRef ref = RangeT<int>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangefRef Canvas::addRangef( const std::string name, float valueLow, float valueHigh, float min, float max, Rangef::Format format )
{
    RangefRef ref = RangeT<float>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangedRef Canvas::addRanged( const std::string name, double valueLow, double valueHigh, double min, double max, Ranged::Format format )
{
    RangedRef ref = RangeT<double>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangeiRef Canvas::addRangei( const std::string name, int *valueLow, int *valueHigh, int min, int max, Rangei::Format format )
{
    RangeiRef ref = RangeT<int>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangefRef Canvas::addRangef( const std::string name, float *valueLow, float *valueHigh, float min, float max, Rangef::Format format )
{
    RangefRef ref = RangeT<float>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RangedRef Canvas::addRanged( const std::string name, double *valueLow, double *valueHigh, double min, double max, Ranged::Format format )
{
    RangedRef ref = RangeT<double>::create( name, valueLow, valueHigh, min, max, format );
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSliderHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ButtonRef Canvas::addButton( const std::string name, bool value, const Button::Format &format )
{
    ButtonRef ref = Button::create( name, value, format );
    ref->setSize( vec2( mButtonDimension, mButtonDimension ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ButtonRef Canvas::addButton( const std::string name, bool *value, const Button::Format &format )
{
    ButtonRef ref = Button::create( name, value, format );
    ref->setSize( vec2( mButtonDimension, mButtonDimension ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ToggleRef Canvas::addToggle( const std::string name, bool value, const Toggle::Format &format )
{
    ToggleRef ref = Toggle::create( name, value, format );
    ref->setSize( vec2( mButtonDimension, mButtonDimension ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ToggleRef Canvas::addToggle( const std::string name, bool *value, const Toggle::Format &format )
{
    ToggleRef ref = Toggle::create( name, value, format );
    ref->setSize( vec2( mButtonDimension, mButtonDimension ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

LabelRef Canvas::addLabel( const string label, FontSize fontSize )
{
    LabelRef ref = Label::create( label+"_LABEL", label, fontSize );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

FpsRef Canvas::addFps( const std::string prefix, FontSize fontSize )
{
    FpsRef ref = Fps::create( prefix, fontSize );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SpacerRef Canvas::addSpacer()
{
    SpacerRef ref = Spacer::create();
    ref->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mSpacerHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

GraphRef Canvas::addGraph( const std::string name, int bufferSize, float *xValues, float *yValues, glm::vec2 rangeX, glm::vec2 rangeY, float height )
{
    GraphRef ref = Graph::create( name, bufferSize, xValues, yValues, rangeX, rangeY );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, height == -1.0 ? w : height ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

PlotterRef Canvas::addPlotter( const std::string name, float *value, glm::vec2 range, int bufferSize, float height )
{
    PlotterRef ref = Plotter::create( name, value, range, bufferSize );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, height == -1.0 ? mGraphHeight : height ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

WaveformRef Canvas::addWaveform( const std::string name, int bufferSize, float *values, glm::vec2 range, float height )
{
    WaveformRef ref = Waveform::create( name, bufferSize, values, range );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, height == -1.0 ? mGraphHeight : height ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

SpectrumRef Canvas::addSpectrum( const std::string name, int bufferSize, float *values, glm::vec2 range, float height )
{
    SpectrumRef ref = Spectrum::create( name, bufferSize, values, range );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, height == -1.0 ? mGraphHeight : height ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

XYPadRef Canvas::addXYPad( const std::string name, vec2 value, const XYPad::Format &format )
{
    XYPadRef ref = XYPad::create( name, value, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, format.mHeight < 0.0f ? w : format.mHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

XYPadRef Canvas::addXYPad( const std::string name, vec2 *value, const XYPad::Format &format )
{
    XYPadRef ref = XYPad::create( name, value, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, format.mHeight < 0.0f ? w : format.mHeight ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

TextureViewRef Canvas::addTexture( std::string name, const ci::gl::TextureRef &textureRef, const TextureView::Format &format )
{
    TextureViewRef ref = TextureView::create( name, textureRef, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    float h = w;
    if( format.getHeight() ) {
        h = format.getHeight();
    } else if( textureRef != nullptr ) {
        h = ( w / textureRef->getAspectRatio() ) ;
    }
    ref->setSize( vec2( w, h ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

RadioRef Canvas::addRadio( std::string name, const std::vector<std::string> options, const Radio::Format &format )
{
    RadioRef ref = Radio::create( name, options, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, 0.0f ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

TextInputRef Canvas::addTextInput( std::string name, const TextInput::Format &format )
{
    TextInputRef ref = TextInput::create( name, format );
    float w = getWidth() - mPadding.mLeft - mPadding.mRight;
    ref->setSize( vec2( w, 0.0f ) );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ColorPickerRef Canvas::addColorPicker( std::string name, const ci::ColorA &color, const ColorPicker::Format &format )
{
    ColorPickerRef ref = ColorPicker::create( name, color, format );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

ColorPickerRef Canvas::addColorPicker( std::string name, ci::ColorA *color, const ColorPicker::Format &format )
{
    ColorPickerRef ref = ColorPicker::create( name, color, format );
    addSubViewPosition( ref, mDirection, mAlignment );
    return ref;
}

vector<RenderData>& Canvas::getRenderData()
{
    size_t index = 0;
    if( mSetNeedsDisplay )
    {
        mViewRenderData = render();
        if( mRenderData.size() < mViewRenderData.size() )
        {
            mRenderData.insert( mRenderData.begin(), mViewRenderData.begin(), mViewRenderData.end() );
            setupBuffers();
            index = mViewRenderData.size();
        }
        else
        {
            mVboRef->bufferSubData(index, mViewRenderData.size()*sizeof(RenderData), mViewRenderData.data());
            for ( auto &it : mViewRenderData )
            {
                mRenderData[index] = it;
                index++;
            }
        }
        mSetNeedsDisplay = false;
    }
    else
    {
        index = getViewRenderDataSize();
    }
    
    for ( auto &it : mSubViews )
    {
        if( it->getNeedsDisplay() )
        {
            vector<RenderData> &subViewData = it->getRenderData();
            
            if( ( index + subViewData.size() ) > mRenderData.size() )
            {
                mRenderData.insert( mRenderData.end(), subViewData.begin(), subViewData.end() );
                index += subViewData.size();
                setupBuffers();
            }
            else
            {
//                cout << "REBUFFERING: " << it->getName() << endl; 
                mVboRef->bufferSubData( index*sizeof( RenderData ), subViewData.size()*sizeof( RenderData ), subViewData.data() );
                for ( auto &sit : subViewData )
                {
                    mRenderData[index] = sit;
                    index++;
                }
            }
        }
        else
        {
            index += it->getRenderDataSize();
        }
    }
    return mRenderData;
}