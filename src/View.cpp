//#include "Globals.h"
#include "View.h"
#include <math.h>

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

View::View() : Responder(), mHit(false), mVisible(true), mSetNeedsDisplay(true), mBounds( 0, 0, 0, 0 ), mPadding( 2.0f ), mTouchId(-1), mEnabled(true), mEmbedded(false), mSetup(false), mUniqueIDs(0), mLoadSubViews(true), mTriggerSubViews(true) { }

View::~View() { }

void View::clear()
{
    setNeedsDisplay( this, true, true );
    mSubViewsNameMap.clear();
    mSubViews.clear();
}

bool View::isHit( const vec2 &pt )
{
    if( mVisible ) {
        Rectf r;
        r.set( 0, 0, getWidth(), getHeight() );
        r.offset( getOrigin() );
        return r.contains( pt );
    }
    return false;
}

vec2 View::getHitPercent( const vec2 &pt )
{
    vec2 hp;
    Rectf r;
    r.set( 0, 0, getWidth(), getHeight() );
    r.offset( getOrigin() );
    hp = ( pt - r.getUpperLeft() ) / r.getSize();
    return hp;
}

JsonTree View::save()
{
    JsonTree tree; 
    tree.addChild( JsonTree( "NAME", getName() ) );
    tree.addChild( JsonTree( "ID", getID() ) );
    tree.addChild( JsonTree( "TYPE", getType() ) );
    JsonTree subtree = JsonTree::makeArray( "SUBVIEWS" );
    for ( auto &it : mSubViews ) {
        if( it->isSaveable() ) {
            subtree.addChild( it->save() );
        }
    }
    if( subtree.getNumChildren() ) {
        tree.addChild( subtree );
    }
    return tree;
}

void View::load( const JsonTree &data )
{
    if( data.hasChild( "SUBVIEWS" ) && mLoadSubViews ) {
        const JsonTree& tree = data.getChild( "SUBVIEWS" );
        int numSubViews = tree.getNumChildren();
        for(int i = 0; i < numSubViews; i++) {
            const JsonTree& sub = tree[i];
            string name = sub.getValueForKey( "NAME" );
            int id = sub.getValueForKey<int>( "ID" );
            ViewRef subview = getSubView( name, id );
            if( !subview ) {
                subview = getSubView( name );
            }
            if( subview ) {
                subview->load( sub ); 
            }
        }
    }    
}

void View::setLoadSubViews( bool loadSubViews )
{
    mLoadSubViews = loadSubViews;
}

void View::setTriggerSubViews( bool triggerSubViews )
{
    mTriggerSubViews = triggerSubViews;
}

void View::trigger( bool recursive )
{
    if( recursive && mTriggerSubViews ) {
        for ( auto &it : mSubViews ) {
            it->trigger( recursive );
        }
    }
}

bool View::isEnabled()
{
    return mEnabled;
}

void View::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

bool View::isVisible()
{
    return mVisible;
}

void View::setVisible( bool visible )
{
    if( mVisible != visible ) {
        mVisible = visible;
        setNeedsDisplay();
        for ( auto &it : mSubViews ) {
            it->setVisible( visible );
        }     
    }
}

void View::setOrigin( vec2 origin )
{
    vec2 size = getSize();
    mBounds.x1 = origin.x;
    mBounds.y1 = origin.y;
    mBounds.x2 = origin.x + size.x;
    mBounds.y2 = origin.y + size.y;
    if( isVisible() )
        setNeedsDisplay( this, true, true );
}

vec2 View::getOrigin( bool recursive )
{
    vec2 origin( mBounds.x1, mBounds.y1 );
    if( recursive && !mSuperView.expired() ) {
        origin += mSuperView.lock()->getOrigin( recursive );
    }
    return origin;
}

void View::setSize( vec2 size )
{
    vec2 origin = getOrigin( false );
    mBounds.x2 = origin.x + size.x;
    mBounds.y2 = origin.y + size.y;
    setNeedsDisplay(); 
}

vec2 View::getSize()
{
    return vec2( mBounds.getWidth(), mBounds.getHeight() );
}

float View::getX()
{
    return mBounds.getX1();
}

float View::getY()
{
    return mBounds.getY1();
}

float View::getWidth()
{
    return mBounds.getWidth();
}

float View::getHeight()
{
    return mBounds.getHeight();
}

Rectf View::getBounds( bool bIncludeSubviews )
{
    Rectf bounds = Rectf( 0.0f, 0.0f, getWidth(), getHeight() );
    if( bIncludeSubviews ) {
        for ( auto &it : mSubViews ) {
            bounds.include( it->getBounds( bIncludeSubviews ) );
        }
        bounds.offset( getOrigin( false ) );
    } else {
        bounds.offset( getOrigin() );
    }
    return bounds;
}

const Paddingf& View::getPadding()
{
    return mPadding;
}

void View::addSubView( ViewRef subView )
{
    subView->setup(); 
    subView->setID( mUniqueIDs );
    mUniqueIDs++;
    mSubViewsNameMap.insert( { subView->getName(), subView } );
    mSubViews.push_back( subView );
    subView->mSuperView = shared_from_this();
    setNeedsDisplay( this );
}

void View::setup()
{
    for ( auto &it : mSubViews ) {
        if( !it->isSetup() ) {
            it->setup();
        }
    }
    mSetup = true;
}

void View::update()
{
    for ( auto &it : mSubViews ) {
        it->update();
    }
}

void View::draw()
{
    if( mVisible ) {
        for ( auto &it : mSubViews ) {
            if( it->isVisible() ) {
                it->draw();
            }
        }
    }
}

vector<RenderData> View::render()
{
    vector<RenderData> data;
    return data;
}

size_t View::getRenderDataSize()
{
    return mRenderData.size();
}

size_t View::getViewRenderDataSize()
{
    return mViewRenderData.size();
}

vector<RenderData>& View::getViewRenderData()
{
    return mViewRenderData;
}

vector<RenderData>& View::getRenderData()
{
    size_t index = 0;
    if( mSetNeedsDisplay ) {
        mViewRenderData = render();
        if( mRenderData.size() < mViewRenderData.size() ) {
            mRenderData.insert( mRenderData.begin(), mViewRenderData.begin(), mViewRenderData.end() );
            index = mViewRenderData.size();
        } else {
            for ( auto &it : mViewRenderData ) {
                mRenderData[index] = it;
                index++;
            }
        }
        mSetNeedsDisplay = false;
    } else {
        index = getViewRenderDataSize();
    }
    
    for ( auto &it : mSubViews ) {
        if( it->getNeedsDisplay() ) {
            vector<RenderData> &subViewData = it->getRenderData();
            if( ( index+subViewData.size() ) > mRenderData.size() ) {
                mRenderData.insert( mRenderData.end(), subViewData.begin(), subViewData.end() );
                index += subViewData.size();
            } else {
                for ( auto &sit : subViewData ) {
                    mRenderData[index] = sit;
                    index++;
                }
            }
        } else {
            index += it->getRenderDataSize();
        }
    }
    return mRenderData;
}

void View::setNeedsDisplay( bool bSetNeedsDisplay )
{
    if( bSetNeedsDisplay ) {
        setNeedsDisplay(this);
    } else {
        mSetNeedsDisplay = bSetNeedsDisplay;
    }
}

bool View::getNeedsDisplay()
{
    return mSetNeedsDisplay;
}

void View::setNeedsDisplay( View *view, bool superViewNeedsDisplay, bool subViewsNeedsDisplay )
{
    if( view == this ) {
        mSetNeedsDisplay = true;
    }
    
    //Super View
    if( superViewNeedsDisplay ) {
        setSuperViewNeedsDisplay();
    }
    
    //Sub Views
    if( subViewsNeedsDisplay ) {
        setSubViewsNeedsDisplay();
    }
}

void View::setSuperViewNeedsDisplay()
{
    if( !mSuperView.expired() ) {
        mSuperView.lock()->setNeedsDisplay();
    }
}

void View::setSubViewsNeedsDisplay()
{
    for ( auto it : mSubViews ) {
        if( it->isVisible() ) {
            it->setNeedsDisplay( it.get(), false, true );
        }
    }
}

void View::autoSizeToFitSubviews()
{
    float minX = 100000.0f;
    float maxX = -100000.0f;
    float minY = 100000.0f;
    float maxY = -100000.0f;
    
    for ( auto &it : mSubViews ) {
        if( it->isVisible() ) {
            Rectf b = it->getBounds( true );
            
            if( b.x1 < minX ) minX = b.x1;
            else if( b.x1 > maxX ) maxX = b.x1;
            if( b.x2 < minX ) minX = b.x2;
            else if( b.x2 > maxX ) maxX = b.x2;
            
            if( b.y1 < minY ) minY = b.y1;
            else if( b.y1 > maxY ) maxY = b.y1;
            if( b.y2 < minY ) minY = b.y2;
            else if( b.y2 > maxY ) maxY = b.y2;
        }
    }
    
    vec2 tl = vec2( minX, minY );
    for( auto &it : mSubViews ) {
        it->setOrigin( it->getOrigin( false ) - tl + vec2( mPadding.mRight, mPadding.mBottom ) );
    }
    
    Rectf bounds( minX, minY, maxX, maxY );
    setSize( bounds.getSize() + vec2( mPadding.mLeft + mPadding.mRight, mPadding.mTop + mPadding.mBottom ) );
}

void View::addPoint( vector<RenderData> &data, const ColorA &color, const vec2 &p, float pointSize )
{
    float off = pointSize*0.5;
    addRect( data, color, Rectf( p.x - off, p.y - off, p.x + off, p.y + off ) );
}

void View::addCircle( vector<RenderData> &data, const ColorA &color, const vec2 &p, float radius, int resolution )
{
    vec2 off = vec2(mBounds.x1, mBounds.y1);
    vec3 pt = vec3( p + off, 0.0f );
    
    if( !mSuperView.expired() ) {
        vec3 offset = vec3( mSuperView.lock()->getOrigin(), 0.0f );
        pt += offset;
    }
    
    float incAngle = 2.0f*M_PI/(float)resolution;
    for( int i = 0; i < resolution; i++ ) {
        float theta = i*incAngle;
        float thetaNext = (i+1)*incAngle;
        
        RenderData c = RenderData();
        RenderData pt0 = RenderData();
        RenderData pt1 = RenderData();
        
        c.pos = pt;
        pt0.pos = pt + radius*vec3( sinf( theta ), cosf( theta ), 0.0f );
        pt1.pos = pt + radius*vec3( sinf( thetaNext ), cosf( thetaNext ), 0.0f );
        
        c.color = vec4( color.r, color.g, color.b, color.a );
        pt0.color = vec4( color.r, color.g, color.b, color.a );
        pt1.color = vec4( color.r, color.g, color.b, color.a );
        
        c.uv = vec2( 0, 0 );
        pt0.uv = vec2( 1, 1 );
        pt1.uv = vec2( 1, 1 );
        
        data.push_back( c );
        data.push_back( pt0 );
        data.push_back( pt1 );
    }
}

void View::addLine( vector<RenderData> &data, const ColorA &color, const vec2 &p0, const vec2 &p1, float lineWidth )
{
    //Insert Render Data Needed
    int offset = data.size();
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    
    vec2 off = vec2( mBounds.x1, mBounds.y1 );
    vec3 zAxis( 0.0f, 0.0f, 1.0f );
    vec3 pt0 = vec3( p0 + off, 0.0f );
    vec3 pt1 = vec3( p1 + off, 0.0f );
    vec3 dir = normalize( pt1 - pt0 );
    pt0 -= dir*lineWidth*0.5f;
    pt1 += dir*lineWidth*0.5f;
    dir = cross(dir, zAxis);
    dir *= lineWidth*0.5f;
    
    if( !mSuperView.expired() ) {
        vec3 offset = vec3( mSuperView.lock()->getOrigin(), 0.0f );
        pt0 += offset;
        pt1 += offset;
    }
    
    data[offset+0].pos = pt0 - dir;
    data[offset+1].pos = pt1 - dir;
    data[offset+2].pos = pt1 + dir;
    
    data[offset+3].pos = data[offset+2].pos;
    data[offset+4].pos = pt0 + dir;
    data[offset+5].pos = data[offset+0].pos;
    
    data[offset+0].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+1].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+2].color = vec4( color.r, color.g, color.b, color.a );
    
    data[offset+3].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+4].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+5].color = vec4( color.r, color.g, color.b, color.a );
    
    data[offset+0].uv = vec2( 0, 0 );
    data[offset+1].uv = vec2( 1, 0 );
    data[offset+2].uv = vec2( 1, 1 );
    
    data[offset+3].uv = vec2( 1, 1 );
    data[offset+4].uv = vec2( 0, 1 );
    data[offset+5].uv = vec2( 0, 0 );
}

void View::addBounds( vector<RenderData> &data, const ColorA &color )
{
    Rectf rect = getBounds( true );
    rect.offset(-getOrigin(false));
    addRect( data, color, rect );
}

void View::addBoundsOutline( vector<RenderData> &data, const ColorA &color, float lineWidth )
{
    Rectf rect = getBounds( true );
    rect.offset(-getOrigin(false)); 
    addRectOutline( data, color, rect, lineWidth );
}

void View::addRect( vector<RenderData> &data, const ColorA &color, const Rectf &rect, float z )
{
    int offset = data.size();
    
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    data.push_back( RenderData() );
    
    float x = mBounds.x1 + rect.x1;
    float y = mBounds.y1 + rect.y1;
    
    if( !mSuperView.expired() ) {
        vec2 origin = mSuperView.lock()->getOrigin();
        x += origin.x;
        y += origin.y;
    }
    
    float w = rect.getWidth();
    float h = rect.getHeight();
    
    data[offset+0].pos = vec3( x, y, z );
    data[offset+1].pos = vec3( x+w, y, z );
    data[offset+2].pos = vec3( x+w, y+h, z );
    
    data[offset+3].pos = vec3( x+w, y+h, z );
    data[offset+4].pos = vec3( x, y+h, z );
    data[offset+5].pos = vec3( x, y, z );
    
    data[offset+0].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+1].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+2].color = vec4( color.r, color.g, color.b, color.a );
    
    data[offset+3].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+4].color = vec4( color.r, color.g, color.b, color.a );
    data[offset+5].color = vec4( color.r, color.g, color.b, color.a );
    
    data[offset+0].uv = vec2( 0, 0 );
    data[offset+1].uv = vec2( 1, 0 );
    data[offset+2].uv = vec2( 1, 1 );
    
    data[offset+3].uv = vec2( 1, 1 );
    data[offset+4].uv = vec2( 0, 1 );
    data[offset+5].uv = vec2( 0, 0 );
}

void View::addRectOutline( vector<RenderData> &data, const ColorA &color, const Rectf &rect, float lineWidth )
{
    vec2 tl = vec2( rect.x1, rect.y1 );
    vec2 tr = vec2( rect.x2, rect.y1 );
    vec2 bl = vec2( rect.x1, rect.y2 );
    vec2 br = vec2( rect.x2, rect.y2 );
    
    addLine( data, color, tl, tr, lineWidth );
    addLine( data, color, tr, br, lineWidth );
    addLine( data, color, br, bl, lineWidth );
    addLine( data, color, bl, tl, lineWidth );
}

void View::addPointGrid( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf rect, float gridInterval, float pointSize )
{
    vec2 offset = rect.getUpperLeft();
    
    float w = rect.getWidth();
    float h = rect.getHeight();
    
    float hw = w*0.5;
    float hh = h*0.5;
    
    for( int x = 0; x <= hw; x+=gridInterval ) {
        for( int y = 0; y <= hh; y+=gridInterval ) {
            addPoint( data, color, offset + vec2(hw+x, hh+y), pointSize );
            addPoint( data, color, offset + vec2(hw-x, hh+y), pointSize );
            addPoint( data, color, offset + vec2(hw-x, hh-y), pointSize );
            addPoint( data, color, offset + vec2(hw+x, hh-y), pointSize );
        }
    }
}

void View::addCenterLines( std::vector<RenderData> &data, const ci::ColorA &color, const ci::Rectf rect, float lineWidth )
{
    vec2 offset = rect.getUpperLeft();
    
    float w = rect.getWidth();
    float h = rect.getHeight();
    
    float hw = w*0.5;
    float hh = h*0.5;
    
    addLine( data, color, offset + vec2(0, hh), offset + vec2(w, hh), lineWidth );
    addLine( data, color, offset + vec2(hw, 0), offset + vec2(hw, h), lineWidth );
}

void View::addTriangle( std::vector<RenderData> &data, const ci::ColorA &color, const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2)
{
    vec2 off = vec2( mBounds.x1, mBounds.y1 );
    
    if( !mSuperView.expired() ) {
        vec2 offset = mSuperView.lock()->getOrigin();
        off += offset;
    }
 
    vec3 pt0 = vec3( p0 + off, 0.0f );
    vec3 pt1 = vec3( p1 + off, 0.0f );
    vec3 pt2 = vec3( p2 + off, 0.0f );
    
    RenderData rd0 = RenderData();
    RenderData rd1 = RenderData();
    RenderData rd2 = RenderData();
        
    rd0.pos = pt0;
    rd1.pos = pt1;
    rd2.pos = pt2;
    
    rd0.color = vec4( color.r, color.g, color.b, color.a );
    rd1.color = vec4( color.r, color.g, color.b, color.a );
    rd2.color = vec4( color.r, color.g, color.b, color.a );
        
    rd0.uv = vec2( 0, 0 );
    rd1.uv = vec2( 1, 1 );
    rd2.uv = vec2( 1, 1 );
        
    data.push_back( rd0 );
    data.push_back( rd1 );
    data.push_back( rd2 );
}

ViewRef View::getSubView( std::string subViewName, int subViewID )
{
    auto findings = mSubViewsNameMap.equal_range( subViewName );
    for( auto it = findings.first; it != findings.second; ++it ) {
        if( subViewID == -1 ) {
            return it->second;
        } else if( it->second->getID() == subViewID ) {
            return it->second;
        }
    }
    return nullptr;
}

#if defined( CINDER_COCOA_TOUCH )

void View::touchesBegan( ci::app::TouchEvent &event )
{
    
}

void View::touchesMoved( ci::app::TouchEvent &event )
{
    
}

void View::touchesEnded( ci::app::TouchEvent &event )
{
    
}

#else

void View::mouseDown( ci::app::MouseEvent &event )
{
    if( isHit( event.getPos() ) ) {
        for ( auto &it : mSubViews ) {
            if( it->isVisible() && it->isEnabled() ) {
                it->mouseDown( event );
            }
        }
    }
}

void View::mouseUp( ci::app::MouseEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isVisible() && it->isEnabled() ) {
            it->mouseUp( event );
        }
    }
}

void View::mouseWheel( ci::app::MouseEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isVisible() && it->isEnabled() ) {
            it->mouseWheel( event );
        }
    }
}

void View::mouseMove( ci::app::MouseEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isVisible() && it->isEnabled() ) {
            it->mouseMove( event );
        }
    }
}

void View::mouseDrag( ci::app::MouseEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isVisible() && it->isEnabled() ) {
            it->mouseDrag( event );
        }
    }
}

void View::keyDown( ci::app::KeyEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isEnabled() ) {
            it->keyDown( event );
        }
    }
}

void View::keyUp( ci::app::KeyEvent &event )
{
    for ( auto &it : mSubViews ) {
        if( it->isEnabled() ) {
            it->keyUp( event );
        }
    }
}

#endif