#include "Colors.h"

namespace reza {
namespace ui {

ci::ColorA ColorClear = ci::ColorA( 0.0f, 0.0f, 0.0f, 0.0f );

ci::ColorA ColorBounds = ci::ColorA( 1.0f, 1.0f, 1.0f, 0.75 );
ci::ColorA ColorBoundsOutline = ci::ColorA( 1, 1, 1, 1.0 );

ci::ColorA ColorShadow = ci::ColorA( 0.0f, 0.0f, 0.0f, 0.375f );
ci::ColorA ColorBack = ci::ColorA( 0.0f, 0.0f, 0.0f, 0.55f );

ci::ColorA ColorOutline = ci::ColorA( 1.0f, 1.0f, 1.0f, 0.25f );
ci::ColorA ColorOutlineHighlight = ci::ColorA( 1.0f, 1.0f, 1.0f, 0.75f );

ci::ColorA ColorFill = ci::ColorA( 1.0f, 1.0f, 1.0f, 0.75f );
ci::ColorA ColorFillHighlight = ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
}
} // namespace reza::ui
