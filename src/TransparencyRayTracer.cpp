//
//  TransparencyRayTracer.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 05/05/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "TransparencyRayTracer.h"
#include "Object.h"

#define EPSILON 1e-6

static inline float sqr(float x)
{
    return x*x;
}

void TransparencyRayTracer::refractedContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& totalContribution)
{
    
    int depth = ray.getDepth();
    
    // Stop-condition
    if (depth > 50){
        // We are at the bottom of the recursion, let's just use black so that there is no indirect contribution for this point
        totalContribution = Vec3Df(0,0,0);
        return;
    }
        
    // Cast a new ray, refracted under the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = ray.getOrigin() - intersectionPoint;
    incidentRayDirection.normalize();
    
    // pour calculer la direction du rayon refracted : http://cg.informatik.uni-freiburg.de/course_notes/graphics2_07_materials.pdf
    
    float eta;

    float cosIncidentAngle = Vec3Df::dotProduct(incidentRayDirection, intersectionNormal);
    if ( cosIncidentAngle > 0 ){
        //on vient de l'extérieur de l'objet : 1 correspond à l'indice de l'air
        eta = intersectionObject.getMaterial().getRefractiveIndex() / 1;
    } else {
        eta = 1 / intersectionObject.getMaterial().getRefractiveIndex();
    }
    
    float cos_interm = 1.f -  sqr(cosIncidentAngle);
    float cos_theta_t_sqr = 1.f - 1.f/sqr(eta) * cos_interm;
    
    // Angle limite : uniquement reflection
    if (cos_theta_t_sqr < 0){
        totalContribution = Vec3Df(0,1,0);
        return;
        mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, totalContribution);
        return;
    }

    Vec3Df refractedContribution;
    Vec3Df reflectedContribution;

    float cos_theta = sqrt(cos_theta_t_sqr);
    
    Vec3Df refractedRayDirection = - 1/eta * incidentRayDirection - ( cos_theta - 1/eta * cosIncidentAngle) * intersectionNormal;
    refractedRayDirection.normalize();
    
    Ray refractedRay(intersectionPoint, refractedRayDirection);
    refractedRay.setDepth(ray.getDepth()+1);
    
    raySceneInteraction(refractedRay, scene, refractedContribution);
    
//    mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, reflectedContribution);
    
    float Fr_interm = sqr((eta * cosIncidentAngle - cos_theta) /(eta * cosIncidentAngle + cos_theta));
    
    float Fr_interm_2 = sqr((cosIncidentAngle - eta * cos_theta)/(cosIncidentAngle + eta * cos_theta));
    
    float Fr = .5f * (Fr_interm + Fr_interm_2);
    
    float Ft = 1 - Fr;
        
    totalContribution = refractedContribution;//Ft * refractedContribution + Fr * reflectedContribution;
    
}

void TransparencyRayTracer::rayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{
    
    float reflectiveness = intersectionObject.getMaterial().getReflectiveness();
    float transmitance = intersectionObject.getMaterial().getTransmitance();
    
    if(transmitance < EPSILON && (reflectiveness < EPSILON || !enableMirrorEffet))
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
    
    Vec3Df refractedContribution;
    refractedContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedContribution);
    
    Vec3Df baseColor;
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, baseColor);
    
    intersectionColor = reflectiveness * reflectedColor + (1 - reflectiveness - transmitance) * baseColor + transmitance * refractedContribution;
}

