/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#ifndef WORLD_H
#define WORLD_H

// #include "MexLib.h"
// #include "MexLib_Std.h"
// #include "MexLib_Os.h"
// 
// extern funcptr *MEX_C_LIB_FUNC_NAME_STD;
// extern funcptr *MEX_C_LIB_FUNC_NAME_OS;

#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

#include "MyVector.h"
#include "MyMap.h"
#include "MathUtils.h"
#include "Arbiter.h"

struct Body;
struct Joint;

struct World
{
	World() {}
	World(Vec2 gravity, int iterations) : gravity(gravity), iterations(iterations) {}

	void Add(Body* body);
	void Add(Joint* joint);
	void Clear();

	void Step(float dt);

	void BroadPhase();

	MyVector<Body*> bodies;
	MyVector<Joint*> joints;
	MyMap<ArbiterKey, Arbiter> arbiters;
	Vec2 gravity;
	int iterations;
	static bool accumulateImpulses;
	static bool warmStarting;
	static bool positionCorrection;
};

#endif

