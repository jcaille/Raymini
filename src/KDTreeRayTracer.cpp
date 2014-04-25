//
//  KDTreeRayTracer.cpp
//  RayTracer
//
//  Created by Florian Denis on 25/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "KDTreeRayTracer.h"
#include "Object.h"
#include "BRDF.h"

bool KDTreeRayTracer::rayObjectIntersection(const Ray &ray, const Object &object, const Scene* scene, float &intersectionDistance, Vec3Df &intersectionColor)
{

    Triangle intersectionTriangle;
    bool intersection = object.getKDTree().intersectRay(ray, intersectionDistance, intersectionTriangle);
    
    if (!intersection)
        return false;
    
    // Get the intersection point
    Vec3Df hitPoint;
    std::vector<float> coords;
    
    ray.intersect(intersectionTriangle, object.getMesh(), hitPoint, coords);
    
    intersectionColor = Phong::brdf(hitPoint, ray.getOrigin(), coords, intersectionTriangle, object, scene);
    
    return true;
    
}