//
//  KDTreeRayTracer.cpp
//  RayTracer
//
//  Created by Florian Denis on 25/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "ShadowRayTracer.h"
#include "Object.h"
#include "BRDF.h"
#include "Window.h"

#define EPSILON 10e-4

bool ShadowRayTracer::lightContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Vec3Df& lightPos, const Light& light, const Object& intersectionObject, const Scene& scene, Vec3Df& lightContribution)
{
    
    const Vec3Df& pov = ray.getOrigin();
    
    if(!enableCastShadows)
    {
        //User has decided to not use shadows. Light contribution is basic BRDF
        lightContribution = brdf(intersectionPoint, intersectionNormal, pov, lightPos, intersectionObject, light);
        return true;
    }
    
    Vec3Df direction = lightPos-intersectionPoint;
    float lightDistance = direction.normalize();
    
    // Let's see if direct light from the light can go through to the intersectionPoint
    Ray lightRay(intersectionPoint, direction);
    
    float obstructionDistance;
    Vec3Df obstructionPoint;
    Triangle obstructionTriangle;
    const Object* obstructionObject;
    
    bool intersection = raySceneIntersection(lightRay, scene, obstructionDistance, obstructionPoint, obstructionTriangle, obstructionObject);
    
    if (!intersection || obstructionDistance + EPSILON >= lightDistance){
        lightContribution = brdf(intersectionPoint, intersectionNormal, pov, lightPos, intersectionObject, light);
        return true;
    }
    
    return false;

}

void ShadowRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    intersectionColor = Vec3Df(0,0,0);

    Vec3Df lightContribution;
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){

        if (lightContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, light.getPos(), light, intersectionObject, scene, lightContribution)){
            intersectionColor += lightContribution;
        }
        
    }
    
}


