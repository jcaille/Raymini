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

void ExtendedLightSourcesRayTracer::rayColorForIntersection(const Vec3Df& pov, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    intersectionColor = Vec3Df(0,0,0);
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){
        
        Vec3Df lightContribution(0,0,0);
        
        const std::vector<Vec3Df>& lightSamples = light.getSamples();
        
        for (const Vec3Df& lightPos : lightSamples){
         
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
                lightContribution += BRDF::phong(intersectionPoint, intersectionNormal, pov, lightPos, intersectionObject, light);
            }
            

        }
        
        intersectionColor += lightContribution/lightSamples.size();
    }
    

}

