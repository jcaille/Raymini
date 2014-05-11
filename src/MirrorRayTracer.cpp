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

void MirrorRayTracer::mirrorContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& reflectedColor)
{
    
    if (ray.getDepth() > maxRayDepth){
        // The ray is too deep, we don't want to risk infinite loop, let's back out
        reflectedColor = Vec3Df(0,0,0);
        return;
    }
    
    // Cast a new ray, reflected of the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = (intersectionPoint - ray.getOrigin());
    incidentRayDirection.normalize();
    
    Vec3Df reflecteRayDirection = incidentRayDirection - 2 * intersectionNormal * (Vec3Df::dotProduct(incidentRayDirection, intersectionNormal));
    Ray reflectedRay(intersectionPoint, reflecteRayDirection);
    reflectedRay.setDepth(ray.getDepth()+1);
    reflectedRay.setShouldBounce(ray.shouldBounce());
    
    raySceneInteraction(reflectedRay, scene, reflectedColor);
    
}

void MirrorRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    if(reflectiveness < EPSILON || !enableMirrorEffet)
    {
        directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, intersectionColor);
        return;
    }
    
    Vec3Df reflectedColor;
    mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, reflectedColor);
    
    if(reflectiveness >= 1.0 - EPSILON){
        intersectionColor = reflectedColor;
        return;
    }
        
    Vec3Df baseColor;
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness) * baseColor;

}
