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

void BasicRayTracer::rayColorForIntersection(const Vec3Df& pov, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& intersectionColor)
{

    intersectionColor = Vec3Df(0,0,0);
    
    const std::vector<Light>& lights = scene.getLights();
    for (const Light& light : lights){
        intersectionColor += BRDF::phong(intersectionPoint, intersectionNormal, pov, intersectionObject, light);
    }
    
}


bool BasicRayTracer::rayGeometryIntersection(const Ray& ray, const Geometry& geometry, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle)
{

    if (!ray.intersect(geometry.getBoundingBox(), intersectionPoint)) {
        return false;
    }
    
    // Find the triangle (if any) containing the closest intersection
    intersectionDistance = std::numeric_limits<float>::max();
    
    bool intersection = false;
    
    const std::vector<Triangle>& triangles = geometry.getMesh().getTriangles();
    const std::vector<Vertex>& vertices = geometry.getMesh().getVertices();
    
    for(const Triangle& triangle : triangles)
    {
        
        float triangleIntersectionDistance;
        Vec3Df triangleIntersectionPoint;
        
        if (ray.intersect(triangle, vertices, triangleIntersectionDistance, triangleIntersectionPoint) && triangleIntersectionDistance < intersectionDistance)
        {
            intersectionDistance = triangleIntersectionDistance;
            intersectionPoint = triangleIntersectionPoint;
            intersectionTriangle = triangle;
            intersection = true;

        }
    }
    
    return intersection;
    
}