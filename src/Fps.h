#pragma once

#include "Label.h"

namespace reza { namespace ui {
    
typedef std::shared_ptr<class Fps> FpsRef;
class Fps : public Label
{
public:
    static FpsRef create( FontSize fontSize = FontSize::MEDIUM )
    {
        return FpsRef( new Fps( fontSize ) );
    }
    
    const std::string getType() override { return "Fps"; }

protected:
    Fps( FontSize fontSize );
    void update() override;
};
    
} } //namespace reza::ui