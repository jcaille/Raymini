//
//  MirrorRayTracer.cpp
//  RayTracer
//
//  Created by Jean Caillé on 27/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "MirrorRayTracer.h"
#include "Object.h"
#include "BRDF.h"

void MirrorRayTracer::rayColorWithoutReflection(const Vec3Df& pov, const Vec3Df intersectionPoint, const Vec3Df intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    intersectionColor = Vec3Df(0,0,0);
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){
        
        const Vec3Df& lightPos = light.getPos();
        Vec3Df direction = lightPos-intersectionPoint;
        float lightDistance = direction.normalize();
        
        // Let's see if direct light from the light can go through to the intersectionPoint
        Ray lightRay(intersectionPoint, direction);
        
        float obstructionDistance;
        Vec3Df obstructionPoint;
        Triangle obstructionTriangle;
        const Object* obstructionObject;
        
        bool intersection = raySceneIntersection(lightRay, scene, obstructionDistance, obstructionPoint, obstructionTriangle, obstructionObject);

        if (!intersection || obstructionDistance >= lightDistance){
            intersectionColor += BRDF::phong(intersectionPoint, intersectionNormal, pov, light.getPos(), intersectionObject, light);
        }
        
    }
}

void MirrorRayTracer::rayColorForIntersection(const Vec3Df& pov, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    if(reflectiveness == 0)
    {
        Vec3Df baseColor;
        rayColorWithoutReflection(pov, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
        
        intersectionColor = baseColor;
        return;
    }
    // Cast a new ray, reflected of the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = (intersectionPoint - pov);
    incidentRayDirection.normalize();
    
    Vec3Df reflecteRayDirection = incidentRayDirection - 2 * intersectionNormal * (Vec3Df::dotProduct(incidentRayDirection, intersectionNormal));
    Ray reflectedRay(intersectionPoint, reflecteRayDirection);
    
    Vec3Df reflectedColor;
    raySceneInteraction(reflectedRay, scene, reflectedColor);
    
    if(reflectiveness == 1.0)
    {
        intersectionColor = reflectedColor;
        return;
    }
        
    Vec3Df baseColor;
    rayColorWithoutReflection(pov, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness) * baseColor;
    return;
}
