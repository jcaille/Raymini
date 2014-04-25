// *********************************************************
// Ray Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Ray.h"
#include "Scene.h"
#include "Object.h"

#define ACCURACY 1e-6

using namespace std;

static const unsigned int NUMDIM = 3, RIGHT = 0, LEFT = 1, MIDDLE = 2;

#pragma mark - Bouding box

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

#pragma mark - Triangle and triangle lists

bool Ray::intersect(const Triangle & tri, const vector<Vertex>& vertices, float& intersectionDistance, Vec3Df & intersectionPoint, vector<float>& coords) const
{
    
    Vec3Df p0 = vertices[tri.getVertex(0)].getPos();
    Vec3Df p1 = vertices[tri.getVertex(1)].getPos();
    Vec3Df p2 = vertices[tri.getVertex(2)].getPos();

    Vec3Df e0 = p1 - p0;
    Vec3Df e1 = p2 - p0;
    
    Vec3Df n = Vec3Df::crossProduct(e0, e1);
    n.normalize();
    
    Vec3Df q = Vec3Df::crossProduct(direction, e1);
    
    float a = Vec3Df::dotProduct(e0, q);
    
    if(abs(a) <= ACCURACY)
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
        coords.push_back(b2);
        coords.push_back(b0);
        coords.push_back(b1);
        
        intersectionPoint = origin + t * direction;
        intersectionDistance = (origin - intersectionPoint).getLength();
        return true;
    }
    
    return false;
}

bool Ray::intersect(const vector<Triangle>& triangles,
                    const vector<Vertex>& vertices,
                    float& intersectionDistance,
                    Vec3Df & intersectionPoint,
                    vector<float>& coords,
                    Triangle& intersectionTriangle,
                    float minIntersectionDistance,
                    float maxIntersectionDistance) const
{
    
    // Find the triangle (if any) containing the closest intersection
    intersectionDistance = std::numeric_limits<float>::max();
    int minIntersectionIndex = -1;
    Vec3Df minIntersectionPos;
    vector<float> minCoords;
    
    for(int i = 0 ; i < triangles.size(); ++i)
    {
        Vec3Df intersectionPos;
        Triangle t = triangles[i];
        vector<float> coords;
        float iDistance;
        if (intersect(t, vertices, iDistance, intersectionPos, coords)) {
            float triangleDistance =  Vec3Df::distance(intersectionPos, origin);
            if(triangleDistance < intersectionDistance &&
               triangleDistance < maxIntersectionDistance &&
               triangleDistance > minIntersectionDistance)
            {
                intersectionDistance = triangleDistance;
                minIntersectionPos = intersectionPos;
                minCoords = coords;
                minIntersectionIndex = i;
            }
        }
    }
    
    if (minIntersectionIndex == -1) {
        // No intersection
        return false;
    }
    
    intersectionPoint = minIntersectionPos;
    coords = minCoords;
    intersectionTriangle = triangles[minIntersectionIndex];
    return true;
}

#pragma mark - Object

bool Ray::intersect(const Object &object,
                    float &intersectionDistance,
                    Vec3Df &intersectionPoint,
                    vector<float>& barycentricCoordinates,
                    Triangle& intersectionTriangle,
                    float minIntersectionDistance,
                    float maxIntersectionDistance) const
{

    
    // Instead of translating the object by object.trans, we translate
    // the camera by the -object.trans. Clever, huh ?
    Ray correctedRay(origin - object.getTrans(), direction);
    
    if (!correctedRay.intersect(object.getBoundingBox(), intersectionPoint))
    {
        // The ray does not intersect the bouding box, no need to go deeper
        return false;
    }
    
    // Object is basically just a list of triangles ! Yaaaaay
    
    return correctedRay.intersect(object.getMesh().getTriangles(),
                                  object.getMesh().getVertices(),
                                  intersectionDistance,
                                  intersectionPoint,
                                  barycentricCoordinates,
                                  intersectionTriangle,
                                  minIntersectionDistance,
                                  maxIntersectionDistance);
}