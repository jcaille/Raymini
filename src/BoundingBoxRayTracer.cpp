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


bool BoundingBoxRayTracer::rayObjectIntersection(const Ray& ray, const Object& object, const Scene* scene, float& intersectionDistance, Vec3Df& intersectionColor)
{
    
    // Instead of translating the object by object.trans, we translate
    // the camera by the -object.trans. Clever, huh ?
    Ray correctedRay = Ray(ray.getOrigin() - object.getTrans(), ray.getDirection());
    
    bool hasIntersection = correctedRay.intersect (object.getBoundingBox (), intersectionColor);
    if (hasIntersection) {
        intersectionDistance = Vec3Df::squaredDistance (intersectionColor, correctedRay.getOrigin());
        intersectionColor = 255 * object.getMaterial().getColor();
        return true;
    }
    return false;
}