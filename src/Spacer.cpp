#include "Spacer.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Spacer::Spacer() : Control()
{
    setName( "Spacer" );    
    setPadding( Paddingf( 0.0f, 1.5f, 0.0f, 1.5f ) );
    setDrawBack( false );
    setDrawFill( true );
    setDrawOutline( false );
}
