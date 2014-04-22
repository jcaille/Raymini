// *********************************************************
// Ray Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Ray.h"

#define ACCURACY 10e-6

using namespace std;

static const unsigned int NUMDIM = 3, RIGHT = 0, LEFT = 1, MIDDLE = 2;

bool Ray::intersect (const BoundingBox & bbox, Vec3Df & intersectionPoint) const {
    const Vec3Df & minBb = bbox.getMin ();
    const Vec3Df & maxBb = bbox.getMax ();
    bool inside = true;
    unsigned int  quadrant[NUMDIM];
    unsigned int i;
    unsigned int whichPlane;
    Vec3Df maxT;
    Vec3Df candidatePlane;
    
    for (i=0; i<NUMDIM; i++)
        if (origin[i] < minBb[i]) {
            quadrant[i] = LEFT;
            candidatePlane[i] = minBb[i];
            inside = false;
        } else if (origin[i] > maxBb[i]) {
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxBb[i];
            inside = false;
        } else	{
            quadrant[i] = MIDDLE;
        }

    if (inside)	{
        intersectionPoint = origin;
        return (true);
    }

    for (i = 0; i < NUMDIM; i++)
        if (quadrant[i] != MIDDLE && direction[i] !=0.)
            maxT[i] = (candidatePlane[i]-origin[i]) / direction[i];
        else
            maxT[i] = -1.;

    whichPlane = 0;
    for (i = 1; i < NUMDIM; i++)
        if (maxT[whichPlane] < maxT[i])
            whichPlane = i;

    if (maxT[whichPlane] < 0.) return (false);
    for (i = 0; i < NUMDIM; i++)
        if (whichPlane != i) {
            intersectionPoint[i] = origin[i] + maxT[whichPlane] *direction[i];
            if (intersectionPoint[i] < minBb[i] || intersectionPoint[i] > maxBb[i])
                return (false);
        } else {
            intersectionPoint[i] = candidatePlane[i];
        }
    return (true);			
}

bool Ray::intersect_jean (const Triangle & tri, const Mesh & mesh, float &intersectionDistance) const
{
    
    Vec3Df a = mesh.getVertices()[tri.getVertex(0)].getPos();
    Vec3Df b = mesh.getVertices()[tri.getVertex(1)].getPos();
    Vec3Df c = mesh.getVertices()[tri.getVertex(2)].getPos();
    
    //e1 and e2 are the two edges containing a
    Vec3Df e1 = b - a;
    Vec3Df e2 = c - a;
    
    //Begin calculating determinant
    Vec3Df P = Vec3Df::crossProduct(direction, e2);
    
    //If determinant is near 0, ray lies in plane of triangle. We discard it
    float det = Vec3Df::dotProduct(e1, P);
    if (fabs(det) < ACCURACY) {
        return false;
    }
    float detInverse = 1.0 / det;
    
    // Get the distance from a to the ray origin
    Vec3Df T = origin - a;
    
    // Calculate U parameter
    float u = Vec3Df::dotProduct(T, P) * detInverse;
    // Check if U is within correct bounds
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    
    // Get V parameter
    Vec3Df Q = Vec3Df::crossProduct(T, e1);
    float v = Vec3Df::dotProduct(direction, Q) * detInverse;
    // Check if V is within correct bounds
    if(v < 0.0 || v > 1.0){
        return false;
    }
    
    // Get the intersection point (that has to be not too close to the origin)
    float t = Vec3Df::dotProduct(e2, Q) * detInverse;
    if(t > ACCURACY){
        intersectionDistance = t;
        return true;
    }
    return false;
}

bool Ray::intersect_florian(const Triangle & tri, const Mesh & mesh, float &intersectionDistance) const
{
    
    Vec3Df p0 = mesh.getVertices()[tri.getVertex(0)].getPos();
    Vec3Df p1 = mesh.getVertices()[tri.getVertex(1)].getPos();
    Vec3Df p2 = mesh.getVertices()[tri.getVertex(2)].getPos();

    Vec3Df e0 = p1 - p0;
    Vec3Df e1 = p2 - p0;
    
    Vec3Df n = Vec3Df::crossProduct(e0, e1);
    n.normalize();
    
    Vec3Df q = Vec3Df::crossProduct(direction, e1);
    
    float a = Vec3Df::dotProduct(e0, q);
    
    if(abs(a) < ACCURACY)
        return false;
    
    Vec3Df s = (origin-p0)/a;
    
    Vec3Df r = Vec3Df::crossProduct(s, e0);
    
    float b0 = Vec3Df::dotProduct(s, q);
    
    if (b0 < 0)
        return false;
    
    float b1 = Vec3Df::dotProduct(r, direction);
    
    if (b1 < 0)
        return false;

    float b2 = 1 - b0 - b1;
    
    if (b2 < 0)
        return false;
    
    float t = Vec3Df::dotProduct(e1, r);
    
    if (t>=0){
        intersectionDistance = t;
        return true;
    }
    
    return false;
}