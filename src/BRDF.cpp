//
//  BRDF.cpp
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "BRDF.h"


#include "Light.h"
#include "Vertex.h"
#include "Material.h"
#include "Ray.h"

inline static float max(float a, float b)
{
    return a > b ? a : b;
}

Vec3Df Phong::brdf(const Vec3Df& hitPoint, const Vec3Df& pov, const std::vector<float> coords, const Triangle& t, const Object &object, const Scene *scene)
{
    
    
    // Phong
    Material mat = object.getMaterial();
    Mesh mesh = object.getMesh();
    
    Vertex p0 = mesh.getVertices()[t.getVertex(0)];
    Vertex p1 = mesh.getVertices()[t.getVertex(1)];
    Vertex p2 = mesh.getVertices()[t.getVertex(2)];
    
    Vec3Df n0 = p0.getNormal();
    Vec3Df n1 = p1.getNormal();
    Vec3Df n2 = p2.getNormal();
    
    Vec3Df norm = coords[0] * n0 + coords[1] * n1 + coords[2] * n2;
    norm.normalize();
    

    
    
    Vec3Df color(0,0,0);
    
    for (Light light : scene->getLights()){
        
        Vec3Df realHitPoint = hitPoint + object.getTrans();
        
        Vec3Df lightDirection = light.getPos() - realHitPoint;
        float maxDistance = lightDirection.normalize();
        
        Ray lightRay = Ray(realHitPoint, lightDirection);
        // test lightray intersection with scene
        bool intersection = false;
        for (unsigned int k = 0; k < scene->getObjects().size(); k++) {
            
            const Object& o = scene->getObjects()[k];
            
            float you;
            Vec3Df should;
            std::vector<float> code_in;
            Triangle objective_c; // We don't care about return arguments
            float epsilon = 10e-3; // Min intersection distance to avoid self intersection

            intersection =  lightRay.intersect(o, you, should, code_in, objective_c , epsilon, maxDistance - epsilon);
            if (intersection) {
                break;
            }
        }

        if(!intersection)
        {
            color += singleLightBRDF(realHitPoint, pov, norm, object.getMaterial(), light);
        }
        
    }
    
    return color;
}

Vec3Df Phong::singleLightBRDF(const Vec3Df &hitPoint, const Vec3Df &pov, const Vec3Df normal, Material material, Light light)
{
    Vec3Df wo = pov - hitPoint ;
    wo.normalize();
    
    Vec3Df wi = light.getPos() - hitPoint; // wi is light direction
    wi.normalize();
    Vec3Df r = 2 * normal * (Vec3Df::dotProduct(normal, wi)) - wi;
    
    float f = 0;
    f += max(material.getDiffuse() * Vec3Df::dotProduct(normal, wi), 0 );                              // Diffuse
    f += max(material.getSpecular() * powf( Vec3Df::dotProduct(r, wo) , material.getShininess()), 0);     // Specular
    
    return 255*f*material.getColor()*light.getColor();

}