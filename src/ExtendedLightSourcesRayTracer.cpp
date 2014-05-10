//
//  ExtendedLightSources.cpp
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "ExtendedLightSourcesRayTracer.h"
#include "Scene.h"
#include "BRDF.h"

void ExtendedLightSourcesRayTracer::directContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& directContribution)
{

    directContribution = Vec3Df(0,0,0);
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){
        
        Vec3Df lightContribution(0,0,0);
        Vec3Df rayContribution;
        
        const std::vector<Vec3Df>& lightSamples = light.getSamples();
        
        for (const Vec3Df& lightPos : lightSamples){
                
            lightContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, lightPos, light, intersectionObject, scene, rayContribution);
            lightContribution += rayContribution;
            
        }
        
        directContribution += lightContribution/lightSamples.size();
    }
    
}

void ExtendedLightSourcesRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, intersectionColor);

}

