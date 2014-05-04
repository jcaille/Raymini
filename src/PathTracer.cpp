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
    if (depth > maxRayDepth){
        // We are at the bottom of the recursion, let's just use black so that there is no indirect contribution for this point
        return;
    }
    
    int n = ceil(bounces/(depth+1));
    
    Vec3Df u,v;
    intersectionNormal.getTwoOrthogonals(u, v);
    
    
    Vec3Df bounceContribution;
    for (int i = 0; i < n; ++i)
    {
        // Indirect contribution : draw n random paths and average their contributions
        float theta = random_float(0, 2*M_PI);
        float phi = random_float(0, M_PI/2);
        
        Vec3Df direction = cos(theta) * sin(phi) * u + sin(theta) * sin(phi) * v + cos(phi) * intersectionNormal;
        direction.normalize();
        
        Ray newRay(intersectionPoint, direction);
        newRay.setDepth(depth+1);
        
        raySceneInteraction(newRay, scene, bounceContribution);
        
        indirectLightContribution += bounceContribution/float(n);
    }
    
    
}

void PathTracer::refractedContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& refractedContribution)
{
    
    //refractedContribution = Vec3Df(0,0,0);
    
    int depth = ray.getDepth();
    
    // Stop-condition
    if (depth > maxRayDepth){
        // We are at the bottom of the recursion, let's just use black so that there is no indirect contribution for this point
        return;
    }
    
    Vec3Df bounceContribution;
    // Cast a new ray, refracted under the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = (intersectionPoint - ray.getOrigin());
    float distance = incidentRayDirection.normalize();
        
    // pour calculer la direction du rayon refracted : http://cg.informatik.uni-freiburg.de/course_notes/graphics2_07_materials.pdf
        
    float eta;
    float cosIncidentAngle = Vec3Df::dotProduct(incidentRayDirection, intersectionNormal);
    if ( cosIncidentAngle < 0 ){
        //on vient de l'extérieur de l'objet - 1 correspond à l'indice de l'air
        eta = intersectionObject.getMaterial().getRefractiveIndex() / 1;
    }else{
        eta = 1 / intersectionObject.getMaterial().getRefractiveIndex();
    }
    
    float cos_interm = 1.f - pow ( cosIncidentAngle , 2);
    float cos_interm_2 = 1.f - 1.f/ pow(eta,2) * cos_interm;
    
    if (cos_interm_2 < 0){
        //directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
        mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
        return;
    }
    
    float cos = sqrt(cos_interm_2);
    Vec3Df refractedRayDirection = 1/eta * incidentRayDirection - ( cos - 1/eta * cosIncidentAngle) * intersectionNormal;
    refractedRayDirection.normalize();
        
    Ray refractedRay(intersectionPoint, refractedRayDirection);
    refractedRay.setDepth(ray.getDepth()+1);
    
    raySceneInteraction(refractedRay, scene, bounceContribution);
    mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
    
    refractedContribution +=  1 / pow(eta,2) * intersectionObject.getMaterial().getTransmitance()*bounceContribution;
    
}

void PathTracer::allDiffuseContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& diffuseLightContribution)
{
    Vec3Df directContribution;
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, directContribution);
    
    Vec3Df indirectContribution;
    indirectContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, indirectContribution);
    
//    Vec3Df refractedContribution;
//    refractedContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
    
    diffuseLightContribution = directContribution + 0.4*indirectContribution;

}

void PathTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    float transmitance = intersectionObject.getMaterial().getTransmitance();
    
    if(transmitance < EPSILON && (reflectiveness < EPSILON || !enableMirrorEffet))
    {
        allDiffuseContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, intersectionColor);
        return;
    }
    
    Vec3Df reflectedColor;
    mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, reflectedColor);
    
    if(reflectiveness >= 1.0 - EPSILON){
        intersectionColor = reflectedColor;
        return;
    }
    
    Vec3Df refractedContribution;
    refractedContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);

    Vec3Df baseColor;
    allDiffuseContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    
    //intersectionColor = transmitance * refractedContribution;
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness - transmitance) * baseColor + transmitance * refractedContribution;

}


