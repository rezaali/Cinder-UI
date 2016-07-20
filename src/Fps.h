#pragma once

#include "Label.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Fps> FpsRef;
class Fps : public Label {
  public:
	static FpsRef create( const std::string prefix, FontSize fontSize = FontSize::MEDIUM )
	{
		return FpsRef( new Fps( prefix, fontSize ) );
	}

	const std::string getType() override { return "Fps"; }
  protected:
	Fps( const std::string prefix, FontSize fontSize );
	void update() override;

	std::string mPrefix = ":";
};
}
} //namespace reza::ui