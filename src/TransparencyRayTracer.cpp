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

void TransparencyRayTracer::refractedContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& totalContribution)
{
    
    
    int depth = ray.getDepth();
    
    // Stop-condition
    if (depth > maxRayDepth){
        // We are at the bottom of the recursion, let's just use black so that there is no indirect contribution for this point
        return;
    }
        
    // Cast a new ray, refracted under the surface and get its intersection with the scene.
    Vec3Df incidentRayDirection = (intersectionPoint - ray.getOrigin());
    incidentRayDirection.normalize();
    
    // pour calculer la direction du rayon refracted : http://cg.informatik.uni-freiburg.de/course_notes/graphics2_07_materials.pdf
    
    float eta;

    float cosIncidentAngle = Vec3Df::dotProduct(incidentRayDirection, intersectionNormal);
    if ( cosIncidentAngle < 0 ){
        //on vient de l'extérieur de l'objet : 1 correspond à l'indice de l'air
        eta = intersectionObject.getMaterial().getRefractiveIndex() / 1;
    }else{
        eta = 1 / intersectionObject.getMaterial().getRefractiveIndex();
    }
    
    float cos_interm = 1.f -  cosIncidentAngle * cosIncidentAngle;
    float cos_theta_t = 1.f - 1.f/(eta*eta) * cos_interm;
    
    // Angle limite : uniquement reflection
    if (cos_theta_t < 0){
        mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, totalContribution);
        return;
    }

    Vec3Df refractedContribution;
    Vec3Df reflectedContribution;

    float cos = sqrt(cos_theta_t);
    
    Vec3Df refractedRayDirection = 1/eta * incidentRayDirection - ( cos - 1/eta * cosIncidentAngle) * intersectionNormal;
    refractedRayDirection.normalize();
    
    Ray refractedRay(intersectionPoint, refractedRayDirection);
    refractedRay.setDepth(ray.getDepth()+1);
    raySceneInteraction(refractedRay, scene, refractedContribution);
    
    mirrorContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, reflectedContribution);
        
    float Fr_interm = ((eta * cosIncidentAngle - cos_theta_t) /(eta * cosIncidentAngle + cos_theta_t) )* ((eta * cosIncidentAngle - cos_theta_t) /(eta * cosIncidentAngle + cos_theta_t));
    
    float Fr_interm_2 = ((cosIncidentAngle - eta * cos_theta_t)/(cosIncidentAngle + eta * cos_theta_t)) * ((cosIncidentAngle - eta * cos_theta_t)/(cosIncidentAngle + eta * cos_theta_t));
    
    float Fr = .5f * (Fr_interm + Fr_interm_2);
    
    float Ft = 1 - Fr;
    
    totalContribution = Ft * refractedContribution + Fr * reflectedContribution;
    
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

