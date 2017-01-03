#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace reza {
namespace ui {

template <typename T>
std::string toString( T value, int precision, int width, char fill )
{
	std::stringstream out;
	out << (std::fixed) << (std::setfill( fill )) << (std::setw( width )) << (std::setprecision( precision )) << (value);
	return (out.str());
};

template <typename T>
std::string toString( T value, int precision, char fill )
{
	std::stringstream out;
	out << (std::fixed) << (std::setfill( fill )) << (std::setprecision( precision )) << (value);
	return (out.str());
};

template <typename T>
std::string toString( T value, int precision )
{
	std::stringstream out;
	out << (std::fixed) << (std::setfill( '0' )) << (std::setprecision( precision )) << (value);
	return (out.str());
};
}
} // namespace reza::ui
