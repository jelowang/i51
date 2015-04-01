#if 1
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

//Body bodies[100];
//Joint joints[100];

Body *bodies;
Joint *joints;

Body* bomb = NULL;

#ifdef MEX_PLATFORM_MODIS
float timeStep = 1.0f / 26.0f;
int iterations = 3;
#else
float timeStep = 1.0f / 26.0f;
int iterations = 3;
#endif

int numBodies = 0;
int numJoints = 0;

int demoIndex = 0;

World world;


void LaunchBomb(void)
{
	if (!bomb)
	{
		bomb = bodies + numBodies;
		bomb->Set(Vec2(1.0f, 1.0f), 50.0f);
		bomb->friction = 0.2f;
		world.Add(bomb);
		++numBodies;
	}

	bomb->position.Set(Random(-15.0f, 15.0f), 15.0f);
	bomb->rotation = Random(-1.5f, 1.5f);
	bomb->velocity = -1.5f * bomb->position;
	bomb->angularVelocity = Random(-20.0f, 20.0f);
}

// Single box
void Demo1(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(1.0f, 1.0f), 200.0f);
	b->position.Set(0.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;
}

// A simple pendulum
void Demo2(Body* b, Joint* j)
{
	Body* b1 = b + 0;
	b1->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b1->friction = 0.2f;
	b1->position.Set(0.0f, -0.5f * b1->width.y);
	b1->rotation = 0.0f;
	world.Add(b1);

	Body* b2 = b + 1;
	b2->Set(Vec2(1.0f, 1.0f), 100.0f);
	b2->friction = 0.2f;
	b2->position.Set(9.0f, 11.0f);
	b2->rotation = 0.0f;
	world.Add(b2);

	numBodies += 2;

	j->Set(b1, b2, Vec2(0.0f, 11.0f));
	world.Add(j);

	numJoints += 1;
}

// Varying friction coefficients
void Demo3(Body* b, Joint* j)
{
//iLog("Start demo3");
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 11.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(5.25f, 9.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(2.0f, 7.0f);
	b->rotation = 0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(-5.25f, 5.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 3.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	float friction[5] = {0.75f, 0.5f, 0.35f, 0.1f, 0.0f};
	for (int i = 0; i < 5; ++i)
	{
		b->Set(Vec2(0.5f, 0.5f), 25.0f);
		b->friction = friction[i];
		b->position.Set(-7.5f + 2.0f * i, 14.0f);
		world.Add(b);
		++b; ++numBodies;
	}
//iLog("End demo3");
}

// A vertical stack
void Demo4(Body* b, Joint* j)
{
//iLog("Start demo4");
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
//iLog("world.Add(b)");
	++b; ++numBodies;

	for (int i = 0; i < 10; ++i)
	{
//iLog("i=%d",i);
		b->Set(Vec2(1.0f, 1.0f), 1.0f);
//iLog("b->Set");
		b->friction = 0.2f;
//iLog("b->friction");
		float x = Random(-0.1f, 0.1f);
//iLog("b->Random,x=%f",x);
		b->position.Set(x, 0.51f + 1.05f * i);
//iLog("b->position.Set");
		world.Add(b);
//iLog("world.Add(b)");
		++b; ++numBodies;
	}
//iLog("End demo4");
}

// A pyramid
void Demo5(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	Vec2 x(-6.0f, 0.75f);
	Vec2 y;

	for (int i = 0; i < 4; ++i)
	{
		y = x;

		for (int j = i; j < 4; ++j)
		{
			b->Set(Vec2(1.0f, 1.0f), 10.0f);
			b->friction = 0.2f;
			b->position = y;
			world.Add(b);
			++b; ++numBodies;

			y += Vec2(1.125f, 0.0f);
		}

		//x += Vec2(0.5625f, 1.125f);
		x += Vec2(0.5625f, 2.0f);
	}
}

// A teeter
void Demo6(Body* b, Joint* j)
{
	Body* b1 = b + 0;
	b1->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b1->position.Set(0.0f, -0.5f * b1->width.y);
	world.Add(b1);

	Body* b2 = b + 1;
	b2->Set(Vec2(12.0f, 0.25f), 100.0f);
	b2->position.Set(0.0f, 1.0f);
	world.Add(b2);

	Body* b3 = b + 2;
	b3->Set(Vec2(0.5f, 0.5f), 25.0f);
	b3->position.Set(-5.0f, 2.0f);
	world.Add(b3);

	Body* b4 = b + 3;
	b4->Set(Vec2(0.5f, 0.5f), 25.0f);
	b4->position.Set(-5.5f, 2.0f);
	world.Add(b4);

	Body* b5 = b + 4;
	b5->Set(Vec2(1.0f, 1.0f), 100.0f);
	b5->position.Set(5.5f, 15.0f);
	world.Add(b5);

	numBodies += 5;

	j->Set(b1, b2, Vec2(0.0f, 1.0f));
	world.Add(j);

	numJoints += 1;
}

// A suspension bridge
void Demo7(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	const int numPlanks = 15;
	float mass = 50.0f;

	for (int i = 0; i < numPlanks; ++i)
	{
		b->Set(Vec2(1.0f, 0.25f), mass);
		b->friction = 0.2f;
		b->position.Set(-8.5f + 1.25f * i, 5.0f);
		world.Add(b);
		++b; ++numBodies;
	}

	// Tuning
	float frequencyHz = 2.0f;
	float dampingRatio = 0.7f;

	// frequency in radians
	float omega = 2.0f * k_pi * frequencyHz;

	// damping coefficient
	float d = 2.0f * mass * dampingRatio * omega;

	// spring stifness
	float k = mass * omega * omega;

	// magic formulas
	float softness = 1.0f / (d + timeStep * k);
	float biasFactor = timeStep * k / (d + timeStep * k);

	for (int i = 0; i < numPlanks; ++i)
	{
		j->Set(bodies+i, bodies+i+1, Vec2(-9.125f + 1.25f * i, 5.0f));
		j->softness = softness;
		j->biasFactor = biasFactor;

		world.Add(j);
		++j; ++numJoints;
	}

	j->Set(bodies + numPlanks, bodies, Vec2(-9.125f + 1.25f * numPlanks, 5.0f));
	j->softness = softness;
	j->biasFactor = biasFactor;
	world.Add(j);
	++j; ++numJoints;
}

// Dominos
void Demo8(Body* b, Joint* j)
{
	Body* b1 = b;
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(12.0f, 0.5f), FLT_MAX);
	b->position.Set(-1.5f, 10.0f);
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(0.2f, 2.0f), 10.0f);
		b->position.Set(-6.0f + 1.0f * i, 11.125f);
		b->friction = 0.1f;
		world.Add(b);
		++b; ++numBodies;
	}

	b->Set(Vec2(14.0f, 0.5f), FLT_MAX);
	b->position.Set(1.0f, 6.0f);
	b->rotation = 0.3f;
	world.Add(b);
	++b; ++numBodies;

	Body* b2 = b;
	b->Set(Vec2(0.5f, 3.0f), FLT_MAX);
	b->position.Set(-7.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;

	Body* b3 = b;
	b->Set(Vec2(12.0f, 0.25f), 20.0f);
	b->position.Set(-0.9f, 1.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b3, Vec2(-2.0f, 1.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b4 = b;
	b->Set(Vec2(0.5f, 0.5f), 10.0f);
	b->position.Set(-10.0f, 15.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b2, b4, Vec2(-7.0f, 15.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b5 = b;
	b->Set(Vec2(2.0f, 2.0f), 20.0f);
	b->position.Set(6.0f, 2.5f);
	b->friction = 0.1f;
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b5, Vec2(6.0f, 2.6f));
	world.Add(j);
	++j; ++numJoints;

	Body* b6 = b;
	b->Set(Vec2(2.0f, 0.2f), 10.0f);
	b->position.Set(6.0f, 3.6f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b5, b6, Vec2(7.0f, 3.5f));
	world.Add(j);
	++j; ++numJoints;
}

// A multi-pendulum
void Demo9(Body* b, Joint* j)
{

////i51AdeOsLog(0, "InitDemo(1111)");
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
////i51AdeOsLog(0, "InitDemo(22222)");
	b->position.Set(0.0f, -0.5f * b->width.y);
////i51AdeOsLog(0, "InitDemo(3333333)");
	b->rotation = 0.0f;
	world.Add(b);
////i51AdeOsLog(0, "InitDemo(4444444)");

	Body * b1 = b;
	++b;
	++numBodies;

	float mass = 10.0f;

	// Tuning
	float frequencyHz = 4.0f;
	float dampingRatio = 0.7f;

	// frequency in radians
	float omega = 2.0f * k_pi * frequencyHz;

	// damping coefficient
	float d = 2.0f * mass * dampingRatio * omega;

	// spring stiffness
	float k = mass * omega * omega;

	// magic formulas
	float softness = 1.0f / (d + timeStep * k);
	float biasFactor = timeStep * k / (d + timeStep * k);

	const float y = 12.0f;

////i51AdeOsLog(0, "InitDemo(555555)");
	for (int i = 0; i < 15; ++i)
	{
		Vec2 x(0.5f + i, y);
////i51AdeOsLog(0, "InitDemo(11 i %d)",i);
		b->Set(Vec2(0.75f, 0.25f), mass);
		b->friction = 0.2f;
		b->position = x;
		b->rotation = 0.0f;
		world.Add(b);
////i51AdeOsLog(0, "InitDemo(22 i %d)",i);
		j->Set(b1, b, Vec2((float)i, y));
////i51AdeOsLog(0, "InitDemo(33 i %d)",i);
		j->softness = softness;
		j->biasFactor = biasFactor;
		world.Add(j);
////i51AdeOsLog(0, "InitDemo(44 i %d)",i);
		b1 = b;
		++b;
		++numBodies;
		++j;
		++numJoints;
	}
////i51AdeOsLog(0, "InitDemo(66666)");
}


char demoStrings[9][40] = 
{
	"Demo 1: A Single Box",
	"Demo 2: Simple Pendulum",
	"Demo 3: Varying Friction Coefficients",
	"Demo 4: Randomized Stacking",
	"Demo 5: Pyramid Stacking",
	"Demo 6: A Teeter",
	"Demo 7: A Suspension Bridge",
	"Demo 8: Dominos",
	"Demo 9: Multi-pendulum"
};

void InitDemo(int index)
{
// 	if (index > 0)
// 		log(MEX_LOG_APP0, "InitDemo: %s", demoStrings[index - 1]);
// 	else
// 		log(MEX_LOG_APP0, "ReleaseDemo");

//i51AdeOsLog(0, "start InitDemo:%d",index);
	world.Clear();
//i51AdeOsLog(0, "world.Clear()");
	numBodies = 0;
	numJoints = 0;
	bomb = NULL;

	demoIndex = index;
	
	switch (index)
	{
	case 0:
		break;
	case 1:
		Demo1(bodies, joints);
		break;
	case 2:
		Demo2(bodies, joints);
		break;
	case 3:
		Demo3(bodies, joints);
		break;
	case 4:
		Demo4(bodies, joints);
		break;
	case 5:
		Demo5(bodies, joints);
		break;
	case 6:
		Demo6(bodies, joints);
		break;
	case 7:
		Demo7(bodies, joints);
		break;
	case 8:
		Demo8(bodies, joints);
		break;
	case 9:
////i51AdeOsLog(0, "InitDemo(13)");
		Demo9(bodies, joints);
////i51AdeOsLog(0, "InitDemo(14)");
		break;
	}
i51AdeOsLog(0, "End InitDemo:%d",index);	
}
#endif
