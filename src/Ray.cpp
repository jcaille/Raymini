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

bool Ray::intersectTriangle(const Vec3Df a, const Vec3Df b, const Vec3Df c, Vec3Df &intersectionPoint) const
{
    //e1 and e2 are the two edges containing a
    Vec3Df e1 = b - a;
    Vec3Df e2 = c -a ;
    
    //Begin calculating determinant
    Vec3Df P = Vec3Df::crossProduct(this->getDirection(), e2);
    
    //If determinant is near 0, ray lies in plane of triangle. Wer discard it
    float det = Vec3Df::dotProduct(e1, P);
    if (fabs(det) < ACCURACY) {
        return false;
    }
    
    // Get the distance from a to the ray origin
    Vec3Df T = this->getOrigin() - a;
    
    // Calculate U parameter
    float u = Vec3Df::dotProduct(T, P) / det;
    // Check if U is within correct bounds
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    
    // Get V parameter
    Vec3Df Q = Vec3Df::crossProduct(T, e1);
    float v = Vec3Df::dotProduct(this->getDirection(), Q) / det;
    // Check if V is within correct bounds
    if(v < 0.0 || v > 1.0){
        return false;
    }
    
    // Get the intersection point (that has to be not too close to the origin)
    float t = Vec3Df::dotProduct(e2, Q) / det;
    if(t > ACCURACY){
        intersectionPoint = this->getOrigin() + t * this->getDirection();
        return true;
        // Might get better precision using u and v.
    }
    return false;
}
