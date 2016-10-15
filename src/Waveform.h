#pragma once

#include "Graph.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Waveform> WaveformRef;
class Waveform : public Graph {
  public:
	static WaveformRef create( std::string name, int bufferSize, float *values, glm::vec2 range )
	{
		return WaveformRef( new Waveform( name, bufferSize, values, range ) );
	}

	const std::string getType() override { return "Waveform"; }
	virtual ~Waveform();

  protected:
	Waveform( std::string name, int bufferSize, float *values, glm::vec2 range );
};
}
} //namespace reza::ui