//
//  KDTreeRayTracer.cpp
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "KDTreeRayTracer.h"
#include "Object.h"

bool KDTreeRayTracer::rayGeometryIntersection(const Ray& ray, const Geometry& geometry, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle)
{
    return geometry.getKDTree().intersectRay(ray, intersectionDistance, intersectionPoint, intersectionTriangle);
}
