#include "TextInput.h"
#include "cinder/Clipboard.h"

using namespace reza::ui;
using namespace cinder;
using namespace cinder::app;
using namespace std;

TextInput::Format::Format()
{
	fontSize( FontSize::MEDIUM ).trigger( Trigger::ALL ).autoClear( false ).numeric( false );
}

TextInput::Format::Format( const Format &copy )
{
	mFontSize = copy.mFontSize;
	mTrigger = copy.mTrigger;
	mAutoClear = copy.mAutoClear;
	mNumeric = copy.mNumeric;
}

TextInput::TextInput( std::string name, const Format &format )
	: ControlWithLabel(), mFormat( format ), mCallbackFn( nullptr ), mClicked( false ), mCursorPosition( 0 ), mStartIndex( 0 )
{
	setName( name );
	setTrigger( mFormat.mTrigger );
}

void TextInput::setup()
{
	if( !mLabelRef ) {
		mLabelRef = Label::create( mName + "_LABEL", " ", mFormat.mFontSize );
		mLabelRef->setSize( vec2( getWidth() - mPadding.mLeft - mPadding.mRight, mLabelRef->getSize().y ) );
		addSubView( mLabelRef );
		mLabelRef->setOrigin( vec2( mLabelRef->getSpacing(), mPadding.mTop ) );
		mCharWidth = mLabelRef->getStringWidth( " " ) + mLabelRef->getSpacing();
		mMaxDisplayLength = (int)floor( ( getWidth() - mPadding.mLeft - mPadding.mRight ) / mCharWidth );
		setSize( vec2( getWidth(), mLabelRef->getHeight() + mPadding.mTop + mPadding.mBottom ) );
		setValue( mName );
	}
}

void TextInput::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValue() );
	}

	Control::trigger( recursive );
}

JsonTree TextInput::save()
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "VALUE", getValue() ) );
	return tree;
}

void TextInput::load( const ci::JsonTree &data )
{
	if( data.hasChild( "VALUE" ) ) {
		setValue( data.getValueForKey<string>( "VALUE" ) );
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

std::string TextInput::getValue()
{
	return mValue;
}

void TextInput::setValue( std::string value )
{
	if( mLabelRef ) {
		mValue = value;
		int length = (int)mValue.length();
		if( length > mMaxDisplayLength ) {
			mStartIndex = (int)mValue.length() - mMaxDisplayLength;
			mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
			mCursorPosition = mMaxDisplayLength;
		}
		else {
			mDisplayValue = mValue;
			mStartIndex = 0;
			mCursorPosition = (int)mDisplayValue.length();
		}
		mLabelRef->setLabel( mDisplayValue );
	}
}

void TextInput::setCallback( const std::function<void( std::string )> &callback )
{
	mCallbackFn = callback;
}

void TextInput::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	if( mClicked ) {
		Rectf rect = mHitRect;
		rect.offset( vec2( mLabelRef->getStringWidth( mDisplayValue.substr( 0, mCursorPosition ) ) + mPadding.mLeft, mPadding.mTop ) );

		if( mCursorPosition > 0 ) {
			rect.offset( vec2( mLabelRef->getSpacing(), 0.0f ) );
		}

		rect.y1 -= mPadding.mTop * 0.5f;
		rect.x2 = rect.x1 + 1.0f;
		rect.y2 = rect.y1 + (int)mFormat.mFontSize + mPadding.mBottom;

		addRect( data, color, rect );
	}
	else {
		addRect( data, mColorClear, mHitRect );
	}
}

void TextInput::calculateCursorPosition( const glm::vec2 &pt )
{
	vec2 ht = mLabelRef->getHitPercent( pt );
	int charIndex =(int)round( ( ht.x * mLabelRef->getWidth() ) / mCharWidth );
	if( charIndex > (int)mDisplayValue.length() ) {
		charIndex = (int)mDisplayValue.length();
	}
	mCursorPosition = charIndex;
}

void TextInput::insertCharacter( const std::string &s )
{
	if( mCursorPosition == mMaxDisplayLength ) {
		mValue.insert( mStartIndex + mCursorPosition, s );
		mStartIndex++;
		mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
	}
	else {
		mValue.insert( mStartIndex + mCursorPosition, s );
		mCursorPosition++;
		if( (int)mValue.length() > mMaxDisplayLength ) {
			mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
		}
		else {
			mDisplayValue = mValue;
		}
	}
	if( (int)mTrigger & (int)Trigger::CHANGE ) {
		trigger();
	}
	mLabelRef->setLabel( mDisplayValue );
}

void TextInput::deleteCharacter()
{
	int length = (int)mValue.length();
	if( length ) {
		int charIndex = mCursorPosition - 1;
		int index = mStartIndex + std::max( charIndex, 0 );
		if( length > mMaxDisplayLength ) {
			mValue.replace( index, 1, "" );
			mStartIndex = mStartIndex > 0 ? mStartIndex - 1 : mStartIndex;
			mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
			mLabelRef->setLabel( mDisplayValue );
			if( (int)mTrigger & (int)Trigger::CHANGE ) {
				trigger();
			}
		}
		else if( charIndex >= 0 ) {
			mValue.replace( index, 1, "" );
			mCursorPosition = mCursorPosition > 0 ? mCursorPosition - 1 : mCursorPosition;
			mDisplayValue = mValue;
			mLabelRef->setLabel( mDisplayValue );
			if( (int)mTrigger & (int)Trigger::CHANGE ) {
				trigger();
			}
		}
	}
}

void TextInput::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		mClicked = true;
		setState( State::DOWN );
		calculateCursorPosition( event.getPos() );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

void TextInput::mouseUp( ci::app::MouseEvent &event )
{
	if( mHit ) {
#ifdef CINDER_COCOA_TOUCH
		setState( State::NORMAL );
#else
		if( isHit( event.getPos() ) ) {
			setState( State::OVER );
		}
		else {
			setState( State::NORMAL );
		}
#endif
	}
	mHit = false;
}

void TextInput::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		mClicked = false;
		setState( State::NORMAL );
	}
}

void TextInput::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
	}
	else {
		setState( State::NORMAL );
	}
}

void TextInput::keyDown( ci::app::KeyEvent &event )
{
	if( mClicked ) {
		if( event.getCode() == KeyEvent::KEY_v && event.isMetaDown() ) {
			if( Clipboard::hasString() ) {
				setValue( Clipboard::getString() );
				if( (int)mTrigger & (int)Trigger::CHANGE ) {
					trigger();
				}
			}
		}
		else {
			switch( event.getCode() ) {
			case KeyEvent::KEY_RIGHT: {
				if( mCursorPosition == mMaxDisplayLength && (int)mValue.substr( mStartIndex ).length() > mMaxDisplayLength ) {
					mStartIndex++;
					mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
					mLabelRef->setLabel( mDisplayValue );
				}
				else if( mCursorPosition < (int)mDisplayValue.length() ) {
					mCursorPosition = mCursorPosition + 1;
				}
				setNeedsDisplay();
			} break;

			case KeyEvent::KEY_LEFT: {
				if( mCursorPosition == 0 && mStartIndex > 0 ) {
					mStartIndex--;
					mDisplayValue = mValue.substr( mStartIndex, mMaxDisplayLength );
					mLabelRef->setLabel( mDisplayValue );
				}
				else {
					mCursorPosition = mCursorPosition > 0 ? mCursorPosition - 1 : mCursorPosition;
				}
				setNeedsDisplay();
			} break;

			case KeyEvent::KEY_DELETE:
			case KeyEvent::KEY_BACKSPACE: {
				deleteCharacter();
			} break;

			case KeyEvent::KEY_UNKNOWN:
			case KeyEvent::KEY_TAB:
			case KeyEvent::KEY_CLEAR:
			case KeyEvent::KEY_KP0:
			case KeyEvent::KEY_KP1:
			case KeyEvent::KEY_KP2:
			case KeyEvent::KEY_KP3:
			case KeyEvent::KEY_KP4:
			case KeyEvent::KEY_KP5:
			case KeyEvent::KEY_KP6:
			case KeyEvent::KEY_KP7:
			case KeyEvent::KEY_KP8:
			case KeyEvent::KEY_KP9:
			case KeyEvent::KEY_KP_DIVIDE:
			case KeyEvent::KEY_KP_MULTIPLY:
			case KeyEvent::KEY_KP_PLUS:
			case KeyEvent::KEY_KP_EQUALS:
			case KeyEvent::KEY_UP:
			case KeyEvent::KEY_DOWN:
			case KeyEvent::KEY_INSERT:
			case KeyEvent::KEY_HOME:
			case KeyEvent::KEY_END:
			case KeyEvent::KEY_PAGEUP:
			case KeyEvent::KEY_PAGEDOWN:
			case KeyEvent::KEY_F1:
			case KeyEvent::KEY_F2:
			case KeyEvent::KEY_F3:
			case KeyEvent::KEY_F4:
			case KeyEvent::KEY_F5:
			case KeyEvent::KEY_F6:
			case KeyEvent::KEY_F7:
			case KeyEvent::KEY_F8:
			case KeyEvent::KEY_F9:
			case KeyEvent::KEY_F10:
			case KeyEvent::KEY_F11:
			case KeyEvent::KEY_F12:
			case KeyEvent::KEY_F13:
			case KeyEvent::KEY_F14:
			case KeyEvent::KEY_F15:
			case KeyEvent::KEY_NUMLOCK:
			case KeyEvent::KEY_CAPSLOCK:
			case KeyEvent::KEY_SCROLLOCK:
			case KeyEvent::KEY_RSHIFT:
			case KeyEvent::KEY_LSHIFT:
			case KeyEvent::KEY_RCTRL:
			case KeyEvent::KEY_LCTRL:
			case KeyEvent::KEY_RALT:
			case KeyEvent::KEY_LALT:
			case KeyEvent::KEY_RMETA:
			case KeyEvent::KEY_LMETA:
			case KeyEvent::KEY_LSUPER:
			case KeyEvent::KEY_RSUPER:
			case KeyEvent::KEY_MODE:
			case KeyEvent::KEY_COMPOSE:
			case KeyEvent::KEY_HELP:
			case KeyEvent::KEY_PRINT:
			case KeyEvent::KEY_SYSREQ:
			case KeyEvent::KEY_BREAK:
			case KeyEvent::KEY_MENU:
			case KeyEvent::KEY_POWER:
			case KeyEvent::KEY_EURO:
			case KeyEvent::KEY_UNDO:
				break;

			case KeyEvent::KEY_KP_ENTER:
			case KeyEvent::KEY_RETURN: {
				mClicked = false;
				if( (int)mTrigger & (int)Trigger::END ) {
					trigger();
					setNeedsDisplay();
				}
				if( mFormat.mAutoClear ) {
					setValue( "" );
				}
			} break;

			default: {
				if( mFormat.mNumeric ) {
					if( event.getChar() == '-' && mValue.length() == 0 ) {
						insertCharacter( string( 1, event.getChar() ) );
					}
					else if( event.getChar() == '.' && mValue.find( "." ) == string::npos ) {
						insertCharacter( string( 1, event.getChar() ) );
					}
					else if( isdigit( event.getChar() ) ) {
						insertCharacter( string( 1, event.getChar() ) );
					}
				}
				else {
					insertCharacter( string( 1, event.getChar() ) );
				}
			} break;
			}
		}
	}
}