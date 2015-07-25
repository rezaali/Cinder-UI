#include "Fps.h"
#include "Helpers.h"
#include "cinder/app/App.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;


Fps::Fps( FontSize fontSize ) : Label( "FPS", "FPS:120.000000", fontSize ) { }

void Fps::update()
{
    if( isVisible() )
    {
        setLabel( "FPS:" + toString( app::getWindow()->getApp()->getAverageFps(), 2 ) );
    }
}
