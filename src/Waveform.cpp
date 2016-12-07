#include "Waveform.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Waveform::Waveform( std::string name, int bufferSize, float *values, glm::vec2 range )
	: Graph( name, bufferSize, nullptr, values, vec2( 0.0, 1.0 ), range )
{
	mXValues = new float[mBufferSize];
	for( int i = 0; i < mBufferSize; i++ ) {
		mXValues[i] = lmap<float>( (float)i, 0.0, (float)mBufferSize - 1.0f, 0.0, 1.0 );
	}
	setClosed( false );
}

Waveform::~Waveform()
{
	delete[] mXValues;
}
