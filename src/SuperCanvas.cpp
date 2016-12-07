#include "SuperCanvas.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

SuperCanvas::SuperCanvas( std::string title, const ci::app::WindowRef &window )
	: Canvas( title, window ), mDeltaTime( 0.35f ), mLastHitTime( -1 ), mIsMinified( false ), mLabelHit( false ), mHitPos( vec2( 0, 0 ) ), mLastPos( vec2( 0, 0 ) ), mLabelRef( nullptr )
{
}

void SuperCanvas::setup()
{
	if( !mLabelRef ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, FontSize::MEDIUM );
		//        mLabelRef->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mLabelRef->getHeight() ) );
		//        mLabelRef->setDrawBounds( true );
		//        mLabelRef->setColorBounds( mLabelRef->getColorBack() );
		addSubViewDown( mLabelRef );
		addSubViewToHeader( mLabelRef );
	}
	View::setup();
}

void SuperCanvas::setDeltaTime( float deltaTime )
{
	mDeltaTime = deltaTime;
}

void SuperCanvas::setMinified( bool isMinified )
{
	if( isMinified ) {
		minify();
	}
	else {
		maximize();
	}
}

bool SuperCanvas::isMinified()
{
	return mIsMinified;
}

void SuperCanvas::toggleMinified()
{
	setMinified( !mIsMinified );
}

LabelRef SuperCanvas::getLabel()
{
	return mLabelRef;
}

void SuperCanvas::mouseUp( ci::app::MouseEvent &event )
{
	mLabelHit = false;
	Canvas::mouseUp( event );
}

bool SuperCanvas::isHitHeader( const glm::vec2 &pt )
{
	for( auto &it : mHeaderSubViews ) {
		if( it->isHit( pt ) ) {
			return true;
		}
	}
	return false;
}

void SuperCanvas::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) && isHitHeader( event.getPos() ) ) {
		mLabelHit = true;
		mHitPos = (vec2)event.getPos() - this->getOrigin();

		if( ( ci::app::getElapsedSeconds() - mLastHitTime ) < mDeltaTime ) {
			if( isMinified() ) {
				setMinified( false );
				if( (int)mTrigger & (int)Trigger::BEGIN ) {
					trigger();
				}
			}
			else {
				setMinified( true );
				if( (int)mTrigger & (int)Trigger::END ) {
					trigger();
				}
			}
			return;
		}
		mLastHitTime = (float)ci::app::getElapsedSeconds();
	}
	Canvas::mouseDown( event );
}

void SuperCanvas::mouseDrag( ci::app::MouseEvent &event )
{
	if( mLabelHit ) {
		setOrigin( (vec2)event.getPos() - mHitPos );
		return;
	}
	Canvas::mouseDrag( event );
}

void SuperCanvas::addSubViewToHeader( ViewRef view )
{
	mHeaderSubViews.push_back( view );
}

void SuperCanvas::setVisible( bool visible )
{
	View::setVisible( visible );
	setMinified( isMinified() );
}

void SuperCanvas::clear()
{
	Canvas::clear();
	mHeaderSubViews.clear();
	setup();
}

void SuperCanvas::minify()
{
	for( auto &it : mSubViews ) {
		it->setVisible( false );
	}

	if( mVisible ) {
		for( auto &it : mHeaderSubViews ) {
			it->setVisible( true );
		}
		autoSizeToFitSubviews();
	}
	mIsMinified = true;
}

void SuperCanvas::maximize()
{
	if( mVisible ) {
		for( auto &it : mSubViews ) {
			it->setVisible( true );
		}
		autoSizeToFitSubviews();
	}
	mIsMinified = false;
}

void SuperCanvas::save( const ci::fs::path &path )
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "MINIFIED", isMinified() ) );
	tree.addChild( JsonTree( "XPOS", getOrigin().x ) );
	tree.addChild( JsonTree( "YPOS", getOrigin().y ) );
	tree.write( path );
}

void SuperCanvas::load( const ci::fs::path &path )
{
	if( fs::exists( path ) ) {
		try {
			JsonTree tree( loadFile( path ) );
			View::load( tree );
			if( tree.hasChild( "MINIFIED" ) ) {
				setMinified( tree.getValueForKey<bool>( "MINIFIED" ) );
			}
			if( tree.hasChild( "XPOS" ) && tree.hasChild( "YPOS" ) ) {
				setOrigin( vec2( tree.getValueForKey<float>( "XPOS" ), tree.getValueForKey<float>( "YPOS" ) ) );
			}
			trigger();
		}
		catch( ci::Exception exc ) {
			cout << "SUPER CANVAS LOAD ERROR: " << exc.what() << endl;
		}
	}
}