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
    
    Vec3Df wo = pov - hitPoint ;
    wo.normalize();
    
    Vec3Df color(0,0,0);
    
    for (Light light : scene->getLights()){
        
        Vec3Df wi = light.getPos() - hitPoint;
        wi.normalize();
        
        Vec3Df r = 2 * norm * (Vec3Df::dotProduct(norm, wi)) - wi;
        
        float f = 0;
        f += max(mat.getDiffuse() * Vec3Df::dotProduct(norm, wi), 0 );                              // Diffuse
        f += max(mat.getSpecular() * powf( Vec3Df::dotProduct(r, wo) , mat.getShininess()), 0);     // Specular
        
        color += 255*f*mat.getColor()*light.getColor();
        
    }
    
    return color;
    

}