//
//  PathTracer.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "PathTracer.h"
#include "Scene.h"
#include "Path.h"
#include "BRDF.h"

static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>( rand() ) / ( static_cast <float> (RAND_MAX/(hi-lo)));
}

void PathTracer::buildRecursivePath(const Ray &ray, const Scene& scene, Vec3Df& intersectionColor){
    
    if(path.getRays().size() == depth_max){
        intersectionColor = getBackgroundColor();
        return;
    }
    
    // Determine where the ray intersects the scene
    
    float objectIntersectionDistance;
    Vec3Df objectIntersectionPoint;
    Triangle objectIntersectionTriangle;
    const Object* objectIntersectionObject;
    
    bool objectIntersection = raySceneIntersection(ray, scene, objectIntersectionDistance, objectIntersectionPoint, objectIntersectionTriangle, objectIntersectionObject);
    
    float lightIntersectionDistance;
    Vec3Df lightIntersectionPoint;
    Triangle lightIntersectionTriangle;
    const Light* lightIntersectionObject;
    
    bool lightIntersection = raySceneIntersection(ray, scene, lightIntersectionDistance, lightIntersectionPoint, lightIntersectionTriangle, lightIntersectionObject);
    
    if ( !objectIntersection && !lightIntersection){
        intersectionColor = getBackgroundColor();
        return;
        
    }
    
    if (lightIntersectionDistance <= objectIntersectionDistance){
        //on n'ajoute plus dans le path et on peut remonter le path pour calculer les brdf
        //car on a une light !
        path.setContainsLight(true);
        
        //si on touche directement la lumière sans toucher de points, alors on donne la couleur de la lumière
        //sinon on multiplie seulement par sa couleur
        if (path.getRays().size() == 0){
            intersectionColor = lightIntersectionObject->getColor();
        }else{
            intersectionColor *= lightIntersectionObject->getColor();
            
        }
        //intersectionColor.normalize();
        intersectionColor *= lightIntersectionObject->getIntensity();
        return;
        
    }else{
        
        // Compute normal at intersection
        const Mesh& mesh = objectIntersectionObject->getMesh();
        
        // Get barycentric coordinates
        std::vector<float> coords;
        mesh.barycentricCoordinates(objectIntersectionPoint, objectIntersectionTriangle, coords);
        
        Vertex p0 = mesh.getVertices()[objectIntersectionTriangle.getVertex(0)];
        Vertex p1 = mesh.getVertices()[objectIntersectionTriangle.getVertex(1)];
        Vertex p2 = mesh.getVertices()[objectIntersectionTriangle.getVertex(2)];
        
        Vec3Df n0 = p0.getNormal();
        Vec3Df n1 = p1.getNormal();
        Vec3Df n2 = p2.getNormal();
        
        Vec3Df norm = coords[0] * n0 + coords[1] * n1 + coords[2] * n2;
        norm.normalize();
        
        //illumination directe que l'on calcule que lors de la première intersection.
        //on lastocke dans la variable directIntersectionColor dont on ajoutera la contribution à la fin dans les return.
        
        //if (path.getRays().size() == 0){
            
            const vector<Light>& lights = scene.getLights();
            
            for (const Light& light : lights ){
                int index_sampleLight = random_float(0, light.getSamples().size()-1);
                
                Vec3Df sample_direction = light.getSamples()[index_sampleLight] - objectIntersectionPoint;
                float distanceSample = sample_direction.normalize();
                
                // Let's see if direct light from the light can go through to the intersectionPoint
                Ray lightRay(objectIntersectionPoint + 0.01*sample_direction, sample_direction);
                
                float obstructionDistance;
                Vec3Df obstructionPoint;
                Triangle obstructionTriangle;
                const Object* obstructionObject;
                
                bool direct_lightIntersection = raySceneIntersection(lightRay, scene, obstructionDistance, obstructionPoint, obstructionTriangle, obstructionObject);
                
                if (!direct_lightIntersection || obstructionDistance> distanceSample){
                    this->directIntersectionColor += BRDF::phong(objectIntersectionPoint, norm, ray.getOrigin(), light.getSamples()[index_sampleLight], *objectIntersectionObject, light);
                }
                
            }
        //}
        
    
        //illumination indirecte
        //on trace un nouveau rayon que l'on met dans le path
        float theta = random_float(0, 2*M_PI);
        float phi = random_float(0, M_PI/2);
        
        Vec3Df u,v;
        norm.getTwoOrthogonals(u, v);
        Vec3Df direction;
        Vec3Df direction_light = scene.getLights()[0].getPos() + scene.getLights()[0].getTrans() - objectIntersectionPoint;
        direction_light.normalize();
 
        direction = cos(theta)* sin(phi) * u + sin(theta)*sin(phi)*v + cos(phi)*norm;
        direction.normalize();
        Ray ray_new = Ray(objectIntersectionPoint +0.01*direction, direction);
        path.addRay(ray_new);
        
        intersectionColor += BRDF::phong(objectIntersectionPoint, norm, ray.getOrigin(), direction, *objectIntersectionObject);
        buildRecursivePath(ray_new, scene, intersectionColor );
                
    }

}

void PathTracer::raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor){
    
    //d'après http://www.inf.ed.ac.uk/teaching/courses/cg/lectures/cg15_2013.pdf

    depth_max = 8;
    path.setRays(vector<Ray>());
    path.setContainsLight(false);
    intersectionColor = Vec3Df(0,0,0);
    this->directIntersectionColor = Vec3Df(0,0,0);
    buildRecursivePath(ray, scene, intersectionColor);
    intersectionColor += directIntersectionColor;
    if (path.getRays().size() !=0){
        intersectionColor /= path.getRays().size();
    }
    //intersectionColor.normalize();

}
