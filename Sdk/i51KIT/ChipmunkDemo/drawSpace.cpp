/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include <limits.h>
#include <string.h>

#include "chipmunk_private.h"
#include "drawSpace.h"

/*
	IMPORTANT - READ ME!
	
	This file sets up a simple interface that the individual demos can use to get
	a Chipmunk space running and draw what's in it. In order to keep the Chipmunk
	examples clean and simple, they contain no graphics code. All drawing is done
	by accessing the Chipmunk structures at a very low level. It is NOT
	recommended to write a game or application this way as it does not scale
	beyond simple shape drawing and is very dependent on implementation details
	about Chipmunk which may change with little to no warning.
*/

static void
drawCircleShape(cpBody *body, cpCircleShape *circle, cpSpace *space)
{
	iS32 X = (iS32)(circle->c.x);
	iS32 Y = (iS32)(circle->c.y);
	iU32 R = (iU32)(circle->r);
	i51KitG2FillCircle( X, Y, R, 0XFF0);
	i51KitG2DrawCircle( X, Y, R, 0XFF0);

}

static void
drawSegmentShape(cpBody *body, cpSegmentShape *seg, cpSpace *space)
{
	cpVect a = seg->ta;
	cpVect b = seg->tb;
	iS32 Point[2][2] = {0};// = {(iS32)a.x,(iS32)a.y,(iS32)b.x,(iS32)b.y};
	 Point[0][0]= (iS32)(a.x);
	 Point[0][1] = (iS32)(a.y);
	 Point[1][0]= (iS32)(b.x);
	 Point[1][1]= (iS32)(b.y);
	i51KitG2DrawLine( Point, 0XFF00);
}

iS32 PolygonPoint[10][2]= {0};
static void
drawPolyShape(cpBody *body, cpPolyShape *poly, cpSpace *space)
{
	int count = poly->numVerts;
	int i = 0;
	cpVect * verts = poly->verts;
	
	for( i = 0; i< count; i++ )
	{
		 PolygonPoint[i][0]= (iS32)(verts[i].x);
		 PolygonPoint[i][1] = (iS32)(verts[i].y);
	}
	
	i51KitG2DrawPolygon(count, PolygonPoint, 0XAA);

}

static void
drawObject(cpShape *shape, cpSpace *space)
{
	cpBody *body = shape->body;
	
	switch(shape->klass->type){
		case CP_CIRCLE_SHAPE:
			drawCircleShape(body, (cpCircleShape *)shape, space);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(body, (cpSegmentShape *)shape, space);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(body, (cpPolyShape *)shape, space);
			break;
		default:
			break;
	}
}

