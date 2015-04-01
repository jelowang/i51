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
// #include "MexLib.h"
// #include "MexLib_Std.h"
// #include "MexLib_Os.h"
// 
// extern funcptr *MEX_C_LIB_FUNC_NAME_STD;
// extern funcptr *MEX_C_LIB_FUNC_NAME_OS;

#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"
#include "World.h"
#include "Body.h"
#include "Joint.h"

typedef MyMap<ArbiterKey, Arbiter>::iterator ArbIter;
typedef MyPair<ArbiterKey, Arbiter> ArbPair;

bool World::accumulateImpulses = true;
bool World::warmStarting = true;
bool World::positionCorrection = true;

void World::Add(Body* body)
{
//iLog("start Add boby");
	bodies.push_back(body);
//iLog("end Add boby");
}

void World::Add(Joint* joint)
{
//iLog("start Add joint");
	joints.push_back(joint);
//iLog("end Add joint");
}

void World::Clear()
{
	bodies.clear();
	joints.clear();
	arbiters.clear();
}

Arbiter newArb;
ArbiterKey key;
MyPair<ArbiterKey,Arbiter> TempPair;

void World::BroadPhase()
{
//i51AdeOsLog ( 0 , "Start BroadPhase" ) ;
	// O(n^2) broad-phase
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* bi = bodies[i];

		for (int j = i + 1; j < (int)bodies.size(); ++j)
		{
			Body* bj = bodies[j];

			if (bi->invMass == 0.0f && bj->invMass == 0.0f)
				continue;
//			break;
//i51AdeOsLog ( 0 , "BroadPhase Arbiter1" ) ;
			newArb.Arbiter1(bi, bj);
//i51AdeOsLog ( 0 , "BroadPhase ArbiterKey1" ) ;
			key.ArbiterKey1(bi, bj);
			if (newArb.numContacts > 0)
			{
//i51AdeOsLog ( 0 , "Start arbiters.find(key)" ) ;
				ArbIter iter = arbiters.find(key);
//i51AdeOsLog ( 0 , "End arbiters.find(key)" ) ;
				if (iter == arbiters.end())
				{
//i51AdeOsLog ( 0 , "iter == arbiters.end()" ) ;
//TempPair.first = key;
//TempPair.second = newArb;
//i51AdeOsLog ( 0 , "Start insert" ) ;
					arbiters.insert(ArbPair(key, newArb));
//i51AdeOsLog ( 0 , "End insert" ) ;
				}
				else
				{
//i51AdeOsLog ( 0 , "Start Updata" ) ;
					iter->second.Update(newArb.contacts, newArb.numContacts);
//i51AdeOsLog ( 0 , "End insert" ) ;
				}
//i51AdeOsLog ( 0 , "BroadPhase 7" ) ;
			}
			else
			{
//i51AdeOsLog ( 0 , "Start erase" ) ;
				arbiters.erase(key);
//i51AdeOsLog ( 0 , "End erase" ) ;
			}
		}
	}
//i51AdeOsLog ( 0 , "End BroadPhase" ) ;
}

void World::Step(float dt)
{
	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;
//i51AdeOsLog ( 0 , "Start Set") ;

	// Determine overlapping bodies and update contact points.
	BroadPhase();

	// Integrate forces.
//i51AdeOsLog ( 0 , "(int)bodies.size()=%d",(int)bodies.size() ) ;
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		if (b->invMass == 0.0f)
			continue;

		b->velocity += (dt * (gravity + (b->invMass * b->force)));
		b->angularVelocity += dt * b->invI * b->torque;
	}

	// Perform pre-steps.
//i51AdeOsLog ( 0 , "arb->second.PreStep(inv_dt)") ;
	for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
	{
		arb->second.PreStep(inv_dt);
	}

//i51AdeOsLog ( 0 , "(int)joints.size()=%d",(int)joints.size()) ;
	for (int i = 0; i < (int)joints.size(); ++i)
	{
		joints[i]->PreStep(inv_dt);	
	}

	// Perform iterations
//i51AdeOsLog ( 0 , "iterations=%d",iterations) ;
	for (int i = 0; i < iterations; ++i)
	{
		for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
		{
			arb->second.ApplyImpulse();
		}

		for (int j = 0; j < (int)joints.size(); ++j)
		{
			joints[j]->ApplyImpulse();
		}
	}

	// Integrate Velocities
//i51AdeOsLog ( 0 , "(int)bodies.size()=%d",(int)bodies.size()) ;
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		b->position += dt * b->velocity;
		b->rotation += dt * b->angularVelocity;

		b->force.Set(0.0f, 0.0f);
		b->torque = 0.0f;
	}
//i51AdeOsLog ( 0 , "End Set") ;
}

