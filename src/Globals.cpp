#include "cinder/app/Platform.h"
#include "Globals.h"

namespace reza {
namespace ui {

float Padding = 2.0f;

#if defined( CINDER_COCOA )
float CanvasWidth = 212.0f;
#elif defined( CINDER_LINUX )
float CanvasWidth = 212.0f;
#elif defined( CINDER_MSW )
float CanvasWidth = 242.0f;
#endif

float SliderHeight = 16.0f;
float GraphHeight = 64.0f;
float ButtonDimension = 16.0f;
float SpacerHeight = 1.0f;
}
} // namespace reza::ui
