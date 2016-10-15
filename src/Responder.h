#include <string>

#pragma once

namespace reza { namespace ui {
    
class Responder
{
public:
    Responder();
    virtual ~Responder();
    
    const std::string getName() { return mName; }
    void setName( std::string name ) { mName = name; };
    
    void setID( int ID ) { mID = ID; }
    const int getID() { return mID; }
    
    virtual const std::string getType() { return "Responder"; }
    
protected:
    std::string mName;
    int mID;
};

} } // namespace reza::ui