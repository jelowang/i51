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

#include "Arbiter.h"
#include "Body.h"
#include "World.h"
#include "i51KitAde.h"

//#include "glut.h"

void Contact::Contact1()
{
	 Pn = 0.0f;
	 Pt = 0.0f;
	 Pnb = 0.0f; 
}

ArbiterKey::ArbiterKey()
{
	body1 = NULL;
	body2 = NULL;
}

void ArbiterKey::ArbiterKey1(Body* b1, Body* b2)
{
	if (b1 < b2)
	{
		body1 = b1; body2 = b2;
	}
	else
	{
		body1 = b2; body2 = b1;
	}
}

 bool operator < (const ArbiterKey& a1, const ArbiterKey& a2)
{
//	i51AdeOsLog(0,"Start operator <");
	if ((iU32)(a1.body1) < (iU32)(a2.body1))
	{
//		i51AdeOsLog(0,"(iU32)(a1.body1) < (iU32)(a2.body1)");
		return true;
	}

	if ((iU32)(a1.body1) == (iU32)(a2.body1) && (iU32)(a1.body2) < (iU32)(a2.body2))
	{
//		i51AdeOsLog(0,"iU32)(a1.body1) == (iU32)(a2.body1) && (iU32)(a1.body2) < (iU32)(a2.body2)");
		return true;
	}

//	i51AdeOsLog(0,"return false");
	return false;
}

Arbiter::Arbiter()
{
	body1 = NULL;
	body2 = NULL;
}

void Arbiter::Arbiter1(Body* b1, Body* b2)
{
//i51AdeOsLog ( 0 , "Start Arbiter");
	if (b1 < b2)
	{
		body1 = b1;
		body2 = b2;
	}
	else
	{
		body1 = b2;
		body2 = b1;
	}

//i51AdeOsLog ( 0 , "Start Collide");
	numContacts = Collide(contacts, body1, body2);
//i51AdeOsLog ( 0 , "Start sqrtf");
	friction = sqrtf(body1->friction * body2->friction);
//i51AdeOsLog ( 0 , "End Arbiter");

	//glPointSize(4.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glBegin(GL_POINTS);
	//for (int i = 0; i < numContacts; ++i)
	//{
	//	glVertex2f(contacts[i].position.x, contacts[i].position.y);
	//}
	//glEnd();
	//glPointSize(1.0f);
/*
	for (int i = 0; i < numContacts; ++i)
	{
		i51KitG2DrawPoint((iS32)contacts[i].position.x, (iS32)contacts[i].position.y, 0XAA00);
	}
*/
}

Contact mergedContacts[2];
void Arbiter::Update(Contact* newContacts, int numNewContacts)
{
	mergedContacts[0].Contact1();
	mergedContacts[1].Contact1();

	for (int i = 0; i < numNewContacts; ++i)
	{
		Contact* cNew = newContacts + i;
		int k = -1;
		for (int j = 0; j < numContacts; ++j)
		{
			Contact* cOld = contacts + j;
			if (cNew->feature.value == cOld->feature.value)
			{
				k = j;
				break;
			}
		}

		if (k > -1)
		{
			Contact* c = mergedContacts + i;
			Contact* cOld = contacts + k;
			*c = *cNew;
			if (World::warmStarting)
			{
				c->Pn = cOld->Pn;
				c->Pt = cOld->Pt;
				c->Pnb = cOld->Pnb;
			}
			else
			{
				c->Pn = 0.0f;
				c->Pt = 0.0f;
				c->Pnb = 0.0f;
			}
		}
		else
		{
			mergedContacts[i] = newContacts[i];
		}
	}

	for (int i = 0; i < numNewContacts; ++i)
		contacts[i] = mergedContacts[i];

	numContacts = numNewContacts;
}


const float k_allowedPenetration = 0.01f;
float k_biasFactor ;
Contact* c ;
Vec2 r1;
Vec2 r2;

// Precompute normal mass, tangent mass, and bias.
float rn1;
float rn2;
float kNormal;
Vec2 tangent;
float rt1;
float rt2;
float kTangent;
Vec2 P;
void Arbiter::PreStep(float inv_dt)
{
//	const float k_allowedPenetration = 0.01f;
	k_biasFactor = World::positionCorrection ? 0.2f : 0.0f;

	for (int i = 0; i < numContacts; ++i)
	{
		 c = contacts + i;

		r1 = c->position - body1->position;
		r2 = c->position - body2->position;

		// Precompute normal mass, tangent mass, and bias.
		rn1 = Dot(r1, c->normal);
		rn2 = Dot(r2, c->normal);
		kNormal = body1->invMass + body2->invMass;
		kNormal += body1->invI * (Dot(r1, r1) - rn1 * rn1) + body2->invI * (Dot(r2, r2) - rn2 * rn2);
		c->massNormal = 1.0f / kNormal;

		tangent = Cross(c->normal, 1.0f);
		rt1 = Dot(r1, tangent);
		rt2 = Dot(r2, tangent);
		kTangent = body1->invMass + body2->invMass;
		kTangent += body1->invI * (Dot(r1, r1) - rt1 * rt1) + body2->invI * (Dot(r2, r2) - rt2 * rt2);
		c->massTangent = 1.0f /  kTangent;

		c->bias = -k_biasFactor * inv_dt * Min(0.0f, c->separation + k_allowedPenetration);

		if (World::accumulateImpulses)
		{
			// Apply normal + friction impulse
			P = c->Pn * c->normal + c->Pt * tangent;

			body1->velocity -= body1->invMass * P;
			body1->angularVelocity -= body1->invI * Cross(r1, P);

			body2->velocity += body2->invMass * P;
			body2->angularVelocity += body2->invI * Cross(r2, P);
		}
	}
}

Body* b1;// = body1;
Body* b2;// = body2;

//Contact* c;// = contacts + i;
Vec2 dv;// = b2->velocity + Cross(b2->angularVelocity, c->r2) - b1->velocity - Cross(b1->angularVelocity, c->r1);

// Compute normal impulse
float vn;// = Dot(dv, c->normal);

float dPn;// = c->massNormal * (-vn + c->bias);

float Pn0;// = c->Pn;
Vec2 Pn;// = dPn * c->normal;

//Vec2 tangent;// = Cross(c->normal, 1.0f);
static float vt;// = Dot(dv, tangent);
float dPt;// = c->massTangent * (-vt);

// Compute friction impulse
float maxPt;// = friction * c->Pn;

// Clamp friction
float oldTangentImpulse;// = c->Pt;
//float maxPt;// = friction * dPn;

// Apply contact impulse
Vec2 Pt;// = dPt * tangent;
void Arbiter::ApplyImpulse()
{
	 b1 = body1;
	 b2 = body2;

	for (int i = 0; i < numContacts; ++i)
	{
		c = contacts + i;
		c->r1 = c->position - b1->position;
		c->r2 = c->position - b2->position;

		// Relative velocity at contact
		dv = b2->velocity + Cross(b2->angularVelocity, c->r2) - b1->velocity - Cross(b1->angularVelocity, c->r1);

		// Compute normal impulse
		vn = Dot(dv, c->normal);

		dPn = c->massNormal * (-vn + c->bias);

		if (World::accumulateImpulses)
		{
			// Clamp the accumulated impulse
			Pn0 = c->Pn;
			c->Pn = Max(Pn0 + dPn, 0.0f);
			dPn = c->Pn - Pn0;
		}
		else
		{
			dPn = Max(dPn, 0.0f);
		}

		// Apply contact impulse
		Pn = dPn * c->normal;

		b1->velocity -= b1->invMass * Pn;
		b1->angularVelocity -= b1->invI * Cross(c->r1, Pn);

		b2->velocity += b2->invMass * Pn;
		b2->angularVelocity += b2->invI * Cross(c->r2, Pn);

		// Relative velocity at contact
		dv = b2->velocity + Cross(b2->angularVelocity, c->r2) - b1->velocity - Cross(b1->angularVelocity, c->r1);

		tangent = Cross(c->normal, 1.0f);
		vt = Dot(dv, tangent);
		dPt = c->massTangent * (-vt);

		if (World::accumulateImpulses)
		{
			// Compute friction impulse
			maxPt = friction * c->Pn;

			// Clamp friction
			oldTangentImpulse = c->Pt;
			c->Pt = Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
			dPt = c->Pt - oldTangentImpulse;
		}
		else
		{
			maxPt = friction * dPn;
			dPt = Clamp(dPt, -maxPt, maxPt);
		}

		// Apply contact impulse
		Pt = dPt * tangent;

		b1->velocity -= b1->invMass * Pt;
		b1->angularVelocity -= b1->invI * Cross(c->r1, Pt);

		b2->velocity += b2->invMass * Pt;
		b2->angularVelocity += b2->invI * Cross(c->r2, Pt);
	}
}

