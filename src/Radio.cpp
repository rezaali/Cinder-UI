#include "Radio.h"
#include "Toggle.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

Radio::Radio( std::string name, const std::vector<std::string> options, const Radio::Format &format ) : ControlWithLabel(), mOptions(options), mActive(nullptr), mFormat(format), mCallbackFn(nullptr)
{
    setName( name );
    setPadding( Paddingf( 0.0 ) );
}

Radio::~Radio() { }

void Radio::setup()
{
    float yOffset = 0;
    if( !mLabelRef && mFormat.mLabel )
    {
        mLabelRef = Label::create( mName + "_LABEL", mName, mFormat.mFontSize );
        addSubView( mLabelRef );
        mLabelRef->setOrigin( vec2( 0.0f, 0.0f ) );
        yOffset += mLabelRef->getSize().y;
    }
    
    int index = 0;
    ToggleRef last = nullptr;
    for( auto &it : mOptions )
    {
        ToggleRef toggle = Toggle::create( it, false, mFormat.mButtonFormat );
        vec2 offset = vec2( 0.0f, yOffset );
        if( last != nullptr )
        {
            offset = vec2( 0.0f, last->getOrigin().y + last->getBounds( true ).getHeight() + mPadding.mBottom );
        }        
        toggle->setOrigin( offset );
        toggle->setCallback([this, toggle](bool value){ if( value ) { this->activate( toggle->getName() ); } } );
        mToggleMap[it] = toggle;
        addSubView( toggle ); 
        index++;
        last = toggle;
    }
    activate( mFormat.mActive );
    autoSizeToFitSubviews(); 
    View::setup();
}

void Radio::trigger( bool recursive )
{
    if( mCallbackFn && mActive )
    {
        mCallbackFn( mActive->getName(), mActive->getValue() );
    }
    Control::trigger( recursive );
}

JsonTree Radio::save()
{
    JsonTree tree = View::save();
    if( mActive ) tree.addChild( JsonTree( "ACTIVE", mActive->getName() ) );
    return tree;
}

void Radio::load( const ci::JsonTree &data )
{
    if( data.hasChild( "ACTIVE" ) )
    {
        activate( data.getValueForKey<string>("ACTIVE") );
    }
    View::load( data );
}

void Radio::activate( std::string name )
{
    for( auto &it : mToggleMap )
    {
        if( it.first == name )
        {
            it.second->setValue( true );
            mActive = it.second;
        }
        else
        {
            it.second->setValue( false );
        }
    }
    trigger(); 
}

void Radio::setCallback( const std::function<void(std::string, bool)> &callback )
{
   mCallbackFn = callback;
}

void Radio::drawBack( std::vector<RenderData> &data, const ci::ColorA &color )
{
    
}

void Radio::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
    
}

void Radio::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
    
}

void Radio::drawOutline( std::vector<RenderData> &data, const ci::ColorA &color )
{

}

void Radio::drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
    
}