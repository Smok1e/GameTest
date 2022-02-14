#include "Game.h"
#include "Logging.h"
#include "Collision.h"

//------------------------

int main ()
{
	Game game;

	try
	{
		game.run ();
	}

	catch (std::exception exc)
	{
		DebugMessage ("Unhandled exception thrown: %s\n", exc);
	}

	catch (...)
	{
		DebugMessage ("Unknown exception");
	}

	return 0;
}

//------------------------