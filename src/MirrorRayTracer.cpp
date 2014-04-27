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
#include "Window.h"

#define EPSILON 1e-6

void MirrorRayTracer::rayColorForIntersection(const Vec3Df& pov, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    if(reflectiveness < EPSILON || !mainWindow->getMirrorCheckBoxState())
    {
        directContributionToRayColorForIntersection(pov, intersectionPoint, intersectionNormal, intersectionObject, scene, intersectionColor);
        return;
    }
    
    // Cast a new ray, reflected of the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = (intersectionPoint - pov);
    incidentRayDirection.normalize();
    
    Vec3Df reflecteRayDirection = incidentRayDirection - 2 * intersectionNormal * (Vec3Df::dotProduct(incidentRayDirection, intersectionNormal));
    Ray reflectedRay(intersectionPoint, reflecteRayDirection);
    
    Vec3Df reflectedColor;
    raySceneInteraction(reflectedRay, scene, reflectedColor);
    
    if(reflectiveness == 1.0){
        intersectionColor = reflectedColor;
        return;
    }
        
    Vec3Df baseColor;
    directContributionToRayColorForIntersection(pov, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness) * baseColor;

}
