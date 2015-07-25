#pragma once

#include "Control.h"

namespace reza { namespace ui {
    
typedef std::shared_ptr<class Spacer> SpacerRef;
class Spacer : public Control
{
public:
    static SpacerRef create()
    {
        return SpacerRef( new Spacer() );
    }
    
    const std::string getType() override { return "Spacer"; }
    
protected:
    Spacer();
};

} } //namespace reza::ui