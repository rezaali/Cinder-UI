#pragma once

#include "Control.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Graph> GraphRef;
class Graph : public Control {
  public:
	static GraphRef create( std::string name, int bufferSize, float *xValues, float *yValues, glm::vec2 xRange, glm::vec2 yRange )
	{
		return GraphRef( new Graph( name, bufferSize, xValues, yValues, xRange, yRange ) );
	}

	void setBufferX( float *values );
	void setBufferY( float *values );
	void setBuffers( float *xvalues, float *yvalues );

	const std::string getType() override { return "Graph"; }
	void setClosed( bool closed );

  protected:
	Graph( std::string name, int bufferSize, float *xValues, float *yValues, glm::vec2 rangeX, glm::vec2 rangeY );

	float mLineWidth;
	int mBufferSize;
	float *mXValues;
	float *mYValues;
	glm::vec2 mXRange;
	glm::vec2 mYRange;
	bool mClosed;
	void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
	void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
};
}
} //namespace reza::ui