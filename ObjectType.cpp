#include "ObjectType.h"

//------------------------

const char* StrObjectType (ObjectType type)
{
	#define STR_OBJECT_TYPE_(type) case ObjectType::##type: { return #type; break; }

	switch (type)
	{
		STR_OBJECT_TYPE_ (Base          );
		STR_OBJECT_TYPE_ (Player        );
		STR_OBJECT_TYPE_ (PlayerParticle);
		STR_OBJECT_TYPE_ (Food          );

		default: return "Unknown object";
	}

	#undef STR_OBJECT_TYPE_
}

//------------------------