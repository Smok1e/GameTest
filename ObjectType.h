#pragma once

//------------------------

enum class ObjectType
{
	Base,
	Player,
	PlayerParticle,
	Food,

	Amount
};

//------------------------

const int ObjectsTypesAmount = static_cast <int> (ObjectType::Amount);

//------------------------

const char* StrObjectType (ObjectType type);

//------------------------