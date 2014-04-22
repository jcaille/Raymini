//
//  BRDFRayTracer.cpp
//  RayTracer
//
//  Created by Jean Caillé on 21/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "BRDFRayTracer.h"
#include "Ray.h"
#include "Scene.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

#pragma mark - Singleton pattern

static BRDFRayTracer * instance = NULL;

BRDFRayTracer * BRDFRayTracer::getInstance () {
    if (instance == NULL){
        instance = new BRDFRayTracer ();
        std::cout << "Creating BRDF raytracer" << std::endl;
    }
    return instance;
}

void BRDFRayTracer::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        std::cout << "Deleting BRDF raytracer" << std::endl;
        instance = NULL;
    }
}

#pragma mark - Overloading

// This class implements the simplest BRDF there is.
// We simply overload the rayObjectIntersection method, as there is no
// need to change the way we go through the scene

bool BRDFRayTracer::rayObjectIntersection(const Ray &ray, const Object &object, float &intersectionDistance, Vec3Df &intersectionColor)
{
    // Instead of translating the object by object.trans, we translate
    // the camera by the -object.trans. Clever, huh ?
    Ray correctedRay = Ray(ray.getOrigin() - object.getTrans(), ray.getDirection());
    
    if (correctedRay.intersect (object.getBoundingBox (), intersectionColor))
    {
        // The ray does not intersect the bouding box, no need to go deeper
        return false;
    }
    
    // Find the triangle (if any) containing the closest intersection
    float minIntersectionDistance = std::numeric_limits<float>::max();
    int minIntersectionIndex = -1;
    
    Mesh mesh = object.getMesh();
    
    for(int i = 0 ; i < mesh.getTriangles().size() ; i++)
    {
        float triangleDistance;
        Triangle t = object.getMesh().getTriangles()[i];
        if (correctedRay.intersectTriangle(
                                           mesh.getVertices()[t.getVertex(0)].getPos(),
                                           mesh.getVertices()[t.getVertex(1)].getPos(),
                                           mesh.getVertices()[t.getVertex(2)].getPos(),
                                           triangleDistance)) {
            if(triangleDistance < minIntersectionDistance)
            {
                minIntersectionDistance = triangleDistance;
                minIntersectionIndex = i;
            }
        }
    }
    
    if (minIntersectionIndex == -1) {
        // No intersection
        return false;
    }
    
    intersectionColor = object.getMaterial().getColor() * 255;
    return true;

}