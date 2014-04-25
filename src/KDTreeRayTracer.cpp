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

    Ray correctedRay(ray.getOrigin() - object.getTrans(), ray.getDirection());
    
    Triangle intersectionTriangle;
    bool intersection = object.getKDTree().intersectRay(correctedRay, intersectionDistance, intersectionTriangle);
    
    if (!intersection)
        return false;
    
    // Get the intersection point
    Vec3Df hitPoint;
    std::vector<float> coords;
    
    float a;
    correctedRay.intersect(intersectionTriangle, object.getMesh().getVertices(), a, hitPoint, coords);
    
    intersectionColor = Phong::brdf(hitPoint, correctedRay.getOrigin(), coords, intersectionTriangle, object, scene);
    
    return true;
    
}