#include "Fps.h"
#include "Helpers.h"
#include "cinder/app/App.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

Fps::Fps( const string prefix, FontSize fontSize )
	: Label( prefix, prefix + ":60.00", fontSize ), mPrefix( prefix )
{
}

void Fps::update()
{
	if( isVisible() ) {
		setLabel( mPrefix + toString( app::getWindow()->getApp()->getAverageFps(), 2 ) );
	}
}
