#include "ControlWithLabel.h"

using namespace reza::ui;
using namespace ci;
using namespace std;

ControlWithLabel::ControlWithLabel() : Control() { }

ControlWithLabel::~ControlWithLabel() { }

void ControlWithLabel::changeState()
{
    Control::changeState();
    if( mLabelRef ) mLabelRef->setState( mState ); 
}

void ControlWithLabel::setLabel( const std::string &label )
{
    if( mLabelRef ) mLabelRef->setLabel( label );
}