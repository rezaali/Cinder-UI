#pragma once

#include "Control.h"
#include "Label.h"

namespace reza { namespace ui {
    
class ControlWithLabel : public Control
{
public:
    ControlWithLabel();
    
    const std::string getType() override { return "ControlWithLabel"; }
    
    virtual ~ControlWithLabel();
    virtual void changeState() override;
    
protected:
    
    void setLabel( const std::string &label ); 
    LabelRef mLabelRef;
};

} } //namespace reza::ui