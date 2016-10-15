#pragma once

#include "ControlWithLabel.h"
#include <deque>

namespace reza {
namespace ui {

typedef std::shared_ptr<class Plotter> PlotterRef;
class Plotter : public ControlWithLabel {
  public:
	static PlotterRef create( std::string name, float *value, glm::vec2 range, int bufferSize )
	{
		return PlotterRef( new Plotter( name, value, range, bufferSize ) );
	}

	const std::string getType() override { return "Plotter"; }
	void setUpdateAlways( bool updateAlways );

  protected:
	Plotter( std::string name, float *value, glm::vec2 range, int bufferSize );
	void setup() override;
	void update() override;

	bool mUpdateAlways;
	std::deque<float> mValues;
	float mLineWidth;
	int mBufferSize;
	float *mValue;
	glm::vec2 mRange;
	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
};
}
} //namespace reza::ui