#pragma once

#include "Graph.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Spectrum> SpectrumRef;
class Spectrum : public Graph {
  public:
	static SpectrumRef create( std::string name, int bufferSize, float *values, glm::vec2 range )
	{
		return SpectrumRef( new Spectrum( name, bufferSize, values, range ) );
	}

	const std::string getType() override { return "Spectrum"; }
	virtual ~Spectrum();

  protected:
	Spectrum( std::string name, int bufferSize, float *values, glm::vec2 range );
	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
};
}
} //namespace reza::ui