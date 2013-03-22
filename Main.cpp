#include "common.h"
#include "KickItUp.h"
#include <SDL/SDL.h>

int main( int argc, char * argv[] )
{
	KickItUp	kickItUp;
	
	if( !kickItUp.Initialize() )
		return 1;
	kickItUp.Run();
	kickItUp.Destroy();

	return 0;
}
