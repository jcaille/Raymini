//
//  BasicRayTracer.cpp
//  RayTracer
//
//  Created by Jean Caillé on 21/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include <vector>
#include "BasicRayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "BRDF.h"

using std::vector;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

#pragma mark - Overloading

// This class implements the simplest BRDF there is.
// We simply overload the rayObjectIntersection method, as there is no
// need to change the way we go through the scene

bool BasicRayTracer::rayObjectIntersection(const Ray &ray, const Object &object, const Scene* scene, float &intersectionDistance, Vec3Df &intersectionColor)
{
    
    Vec3Df intersectionPoint;
    vector<float> barycentricCoordinates;
    Triangle intersectionTriangle;
    bool intersection = ray.intersect(object, intersectionDistance, intersectionPoint, barycentricCoordinates, intersectionTriangle);
    if(!intersection){
        return false;
    }
    intersectionColor = Phong::brdf(intersectionPoint, ray.getOrigin(), barycentricCoordinates, intersectionTriangle, object, scene);
    return true;
}