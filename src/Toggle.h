#pragma once

#include "Button.h"

namespace reza { namespace ui {
    
typedef std::shared_ptr<class Toggle> ToggleRef;
class Toggle : public Button
{
public:
    static ToggleRef create( std::string name, bool value, const Format &format = Format() )
    {
        return ToggleRef( new Toggle( name, value, format ) );
    }
    
    static ToggleRef create( std::string name, bool *value, const Format &format = Format() )
    {
        return ToggleRef( new Toggle( name, value, format ) );
    }
    
    const std::string getType() override { return "Toggle"; }
    
protected:
    Toggle( std::string name, bool value, const Format &format = Format() );
    Toggle( std::string name, bool *value, const Format &format = Format() );
    
    bool isSaveable() override { return true; }
    ci::JsonTree save() override;
    void load( const ci::JsonTree &data ) override;

    virtual void mouseDown( ci::app::MouseEvent &event ) override;
    virtual void mouseUp( ci::app::MouseEvent &event ) override;
    virtual void mouseWheel( ci::app::MouseEvent &event ) override;
    virtual void mouseMove( ci::app::MouseEvent &event ) override;
    virtual void mouseDrag( ci::app::MouseEvent &event ) override;
    
    virtual void keyDown( ci::app::KeyEvent &event ) override;
    virtual void keyUp( ci::app::KeyEvent &event ) override;
};
    
} } //namespace reza::ui