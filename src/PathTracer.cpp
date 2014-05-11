//
//  PathTracer.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "PathTracer.h"
#include "Scene.h"
#include "BRDF.h"

#define EPSILON 1e-6

static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>( rand() ) / ( static_cast <float> (RAND_MAX/(hi-lo)));
}

void PathTracer::indirectContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& indirectLightContribution)
{

    indirectLightContribution = Vec3Df(0,0,0);
    
    int depth = ray.getDepth();
    
    // Stop-condition
    if (depth > 1){
        // Indirect contribution is non-zero only for first-order rays only
        return;
    }
    
    Vec3Df u,v;
    intersectionNormal.getTwoOrthogonals(u, v);
    
    Vec3Df bounceContribution;
    for (int i = 0; i < bounces; ++i)
    {
        // Indirect contribution : draw n random paths and average their contributions
        float theta = random_float(0, 2*M_PI);
        float phi = random_float(0, M_PI/2);
        
        Vec3Df direction = cos(theta) * sin(phi) * u + sin(theta) * sin(phi) * v + cos(phi) * intersectionNormal;
        direction.normalize();
        
        Ray newRay(intersectionPoint, direction);
        newRay.setDepth(depth+1);
        
        raySceneInteraction(newRay, scene, bounceContribution);
        
        indirectLightContribution += bounceContribution/float(bounces);
    }
    
    
}

void PathTracer::allDiffuseContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& diffuseLightContribution)
{
    Vec3Df directContribution;
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, directContribution);
    
    Vec3Df indirectContribution;
    indirectContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, indirectContribution);
    
    diffuseLightContribution = directContribution + .3* indirectContribution;

}

void PathTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{

    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    float transmitance = intersectionObject.getMaterial().getTransmitance();
   
    Vec3Df baseColor(0,0,0);
    if (1 - reflectiveness - transmitance > EPSILON){
        allDiffuseContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    }
    if (1 - reflectiveness - transmitance >= 1 - EPSILON){
        intersectionColor = baseColor;
        return;
    }
    
    Vec3Df reflectedColor(0,0,0);
    if (reflectiveness > EPSILON && enableMirrorEffet){
        mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, reflectedColor);
    }
    
    if(reflectiveness >= 1.0 - EPSILON){
        intersectionColor = reflectedColor;
        return;
    }
    
    Vec3Df refractedContribution(0,0,0);
    if (transmitance > EPSILON && enableRefraction){
        refractedContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
    }
    
    if(transmitance >= 1.0 - EPSILON){
        intersectionColor = refractedContribution;
        return;
    }
    
    
    
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness - transmitance) * baseColor + transmitance * refractedContribution;

}


