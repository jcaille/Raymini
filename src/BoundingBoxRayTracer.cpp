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
#include "Light.h"


void BoundingBoxRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    intersectionColor = intersectionObject.getMaterial().getColor();
}

void BoundingBoxRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Light& intersectionLight, const Scene& scene, Vec3Df& intersectionColor)
{
    intersectionColor = intersectionLight.getColor();
}


bool BoundingBoxRayTracer::rayGeometryIntersection(const Ray& ray, const Geometry& geometry, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle)
{
    bool intersection = ray.intersect(geometry.getBoundingBox(), intersectionPoint);
    if (intersection) {
        intersectionDistance = Vec3Df::squaredDistance(intersectionPoint, ray.getOrigin());
    }
    
    return intersection;
}