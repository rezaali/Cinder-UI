#pragma once

namespace reza { namespace ui {
    
template<typename T>
std::string toString( T value, int precision, int width, char fill )
{
    std::ostringstream out;
    out << std::fixed << std::setfill( fill ) << std::setw( width ) << std::setprecision( precision ) << value;
    return out.str();
}
    
template<typename T>
std::string toString( T value, int precision, char fill )
{
    std::ostringstream out;
    out << std::fixed << std::setfill( fill ) << std::setprecision( precision ) << value;
    return out.str();
}
    
    
template<typename T>
std::string toString( T value, int precision )
{
    std::ostringstream out;
    out << std::fixed << std::setfill( '0' ) << std::setprecision( precision ) << value;
    return out.str();
}
    
} } // namespace reza::ui

