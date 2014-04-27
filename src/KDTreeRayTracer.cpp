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

void KDTreeRayTracer::rayColorForIntersection(const Vec3Df& pov, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    intersectionColor = Vec3Df(0,0,0);
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){
        intersectionColor += BRDF::phong(intersectionPoint, intersectionNormal, pov, intersectionObject, light);
    }
    
}


bool KDTreeRayTracer::rayObjectIntersection(const Ray& ray, const Object& object, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle)
{
    return object.getKDTree().intersectRay(ray, intersectionDistance, intersectionPoint, intersectionTriangle);
}