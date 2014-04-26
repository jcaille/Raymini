//
//  BoundingBoxRayTracer.cpp
//  RayTracer
//
//  Created by Florian Denis on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "BoundingBoxRayTracer.h"
#include "Ray.h"
#include "Scene.h"


void BoundingBoxRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Triangle& intersectionTriangle, const Object& intersectionObject, Vec3Df& intersectionColor)
{
    intersectionColor = 255*intersectionObject.getMaterial().getColor();
}


bool BoundingBoxRayTracer::rayObjectIntersection(const Ray& ray, const Object& object, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle)
{
    bool intersection = ray.intersect(object.getBoundingBox(), intersectionPoint);
    if (intersection) {
        intersectionDistance = Vec3Df::squaredDistance(intersectionPoint, ray.getOrigin());
    }
    
    return intersection;
}