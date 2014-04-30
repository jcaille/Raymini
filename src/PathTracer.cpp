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

static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>( rand() ) / ( static_cast <float> (RAND_MAX/(hi-lo)));
}

void PathTracer::indirectContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& indirectLightContribution)
{

    indirectLightContribution = Vec3Df(0,0,0);
    
    int depth = ray.getDepth();
    
    // Stop-condition
    if (depth > maxRayDepth){
        // We are at the bottom of the recursion, let's just use black so that there is no indirect contribution for this point
        return;
    }
    
    Vec3Df bounceContribution;
    for (int i = 0; i < bounces; ++i)
    {
        // Indirect contribution : draw n random paths and average their contributions
        float theta = random_float(0, 2*M_PI);
        float phi = random_float(0, M_PI/2);
        
        Vec3Df u,v;
        intersectionNormal.getTwoOrthogonals(u, v);
        
        Vec3Df direction = cos(theta) * sin(phi) * u + sin(theta) * sin(phi) * v + cos(phi) * intersectionNormal;
        direction.normalize();
        
        Ray newRay(intersectionPoint, direction);
        newRay.setDepth(depth+1);
        
        raySceneInteraction(newRay, scene, bounceContribution);
        
        indirectLightContribution += bounceContribution;
    }
    
    indirectLightContribution /= float(bounces);
    
}

void PathTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    Vec3Df directContribution;
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, directContribution);
    
    Vec3Df indirectContribution;
    indirectContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, indirectContribution);
    
    intersectionColor = directContribution + 0.2*indirectContribution;
    
}


