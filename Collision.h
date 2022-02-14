#pragma once

//------------------------

#include "Object.h"
#include "Player.h"
#include "PlayerParticle.h"
#include "Food.h"

//------------------------

void Hit (Object* a, Object* b);

void Hit_Error       (Object* a, Object* b);
void Hit_None        (Object* a, Object* b);

void Hit_Player_Food           (Object* a, Object* b);
void Hit_Player_PlayerParticle (Object* a, Object* b);

//------------------------

typedef void hit_func_t (Object* a, Object* b);
extern hit_func_t* HitTable[ObjectsTypesAmount][ObjectsTypesAmount];

//------------------------