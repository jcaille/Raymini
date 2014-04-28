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

Vec3Df BRDF::phong(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &lightPos, const Object& object, const Light& light)
{

    const Material& material = object.getMaterial();
    
    Vec3Df wo = pov - hitPoint ;// wi is camera-hitPoint direction
    wo.normalize();

    Vec3Df wi = lightPos - hitPoint; // wi is light direction
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

//Phong for Path Tracing
Vec3Df BRDF::phong(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &nextDirection, const Object& object)
{
    
    const Material& material = object.getMaterial();
    
    Vec3Df wo = pov - hitPoint ;// wi is camera-hitPoint direction
    wo.normalize();
    
    Vec3Df wi = nextDirection; // wi is light direction
    wi.normalize();
    
    float a = Vec3Df::dotProduct(normal, wi);
    if (a <= 0)
        return Vec3Df(0,0,0);
    
    Vec3Df r = 2 * a * normal - wi;
    float b = Vec3Df::dotProduct(r, wo);
    
    float f = material.getDiffuse() * a;    // Diffuse
    
    if (b > 0)
        f += material.getSpecular() * powf( b , material.getShininess());               // Specular
    
    return f * material.getColor();
    
    
}

Vec3Df BRDF::cook_torrance(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &lightPos, const Object& object, const Light& light, float n1, float n2)
{
    const Material& material = object.getMaterial();

    Vec3Df wo = pov - hitPoint ; // wi is camera-hitPoint direction
    wo.normalize();

    Vec3Df wi = lightPos - hitPoint; // wi is light direction
    wi.normalize();

    Vec3Df h = wi + wo; // half-vector
    h.normalize();

    //Diffuse contribution
    float a = Vec3Df::dotProduct(normal, wi);
    if (a <= 0)
        return Vec3Df(0,0,0);

    float diffuse = material.getDiffuse() * a;

    //1-Schlick approximation (for Fresnel term)
    float R0 = pow( (n1-n2) / (n1+n2) , 2);
    float cos = Vec3Df::dotProduct(h,wo);
    float schlickTerm = R0 + (1.f-R0)*pow(1.f-cos,5);

    //2-Geometric attenuation
    float tmp = max(Vec3Df::dotProduct(normal,h),0.f);
    float tmp2 = max (cos,0.f);
    float tmpMin = min (1.f , 2.f*tmp*max(Vec3Df::dotProduct(normal,wi) , 0.f)/tmp2 );
    float geometricAttenuation = min( tmpMin , 2.f*tmp*max(Vec3Df::dotProduct(normal,wo),0.f)/tmp2 );

    //3-Beckmann distribution
    float alpha = std::acos(Vec3Df::dotProduct(normal,h));
    //A way to convert shininess into m factor ( Source : http://simonstechblog.blogspot.fr/2011/12/microfacet-brdf.html )
    float m = pow( (2.f/(2.f+object.getMaterial().getShininess ())),0.5f);
    float beckmannDenom = PI * pow(m,2) * pow( Vec3Df::dotProduct(h,normal) ,4);
    float beckmannNum = std::exp( -1.f*pow( (tan(alpha)/m) ,2 ) );
    float beckmannTerm = beckmannNum/beckmannDenom;


    //Specular contribution
    tmp = max( Vec3Df::dotProduct(normal,wo) , 0.f );
    float spec = schlickTerm*geometricAttenuation*beckmannTerm / (PI*diffuse*tmp);
    // Total

    float f = diffuse + spec;

    return f * light.getIntensity() * material.getColor() * light.getColor();

}
