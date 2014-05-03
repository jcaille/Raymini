//
//  TranslucidRayTracer.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 02/05/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "TranslucidRayTracer.h"
#include "Object.h"

void TranslucidRayTracer::translucidContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& refractedColor){
    
    if (ray.getDepth() > maxRayDepth){
        // The ray is too deep, we don't want to risk infinite loop, let's back out
        refractedColor = Vec3Df(0,0,0);
        return;
    }
    
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
    
    directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedColor);
    
    if (cos_interm_2 < 0){
        //refractedColor = Vec3Df(0,0,0);
        //pas de rayons réfracté car passé l'angle critique, seulement un rayon réfléchi
        //directContributionToRayColorForIntersection(ray, intersectionPoint, intersectionNormal, intersectionObject, scene, refractedColor);
        return;
    }
    
    float cos = sqrt(cos_interm_2);
    Vec3Df refractedRayDirection = 1/eta * incidentRayDirection - ( cos - 1/eta * cosIncidentAngle) * intersectionNormal;
    refractedRayDirection.normalize();
    
    Ray refractedRay(intersectionPoint, refractedRayDirection);
    refractedRay.setDepth(ray.getDepth()+1);
    
    //attenuation
    float attenuation_red = pow(0.8, 1/distance);
    
    refractedColor *= 100*1 / pow(eta,2) * intersectionObject.getMaterial().getTransmitance();//* Vec3Df(0.f, 1.0, 1.0);
    
    raySceneInteraction(refractedRay, scene, refractedColor);
    
}