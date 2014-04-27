//
//  BRDF.cpp
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "BRDF.h"

#include "Light.h"
#include "Object.h"

Vec3Df BRDF::phong(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Object& object, const Light& light)
{

    const Material& material = object.getMaterial();
    
    Vec3Df wo = pov - hitPoint ;
    wo.normalize();

    Vec3Df wi = light.getPos() - hitPoint; // wi is light direction
    wi.normalize();
    
    float a = Vec3Df::dotProduct(normal, wi);
    if (a <= 0)
        return Vec3Df(0,0,0);
    
    Vec3Df r = 2 * a * normal - wi;
    float b = Vec3Df::dotProduct(r, wo);
    
    float f = material.getDiffuse() * a;                                                // Diffuse
    
    if (b > 0)
        f += material.getSpecular() * powf( b , material.getShininess());               // Specular
    
    return f * light.getIntensity() * material.getColor() * light.getColor();


}