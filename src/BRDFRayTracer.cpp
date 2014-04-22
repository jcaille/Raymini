//
//  BRDFRayTracer.cpp
//  RayTracer
//
//  Created by Jean Caillé on 21/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include <vector>
#include "BRDFRayTracer.h"
#include "Ray.h"
#include "Scene.h"

using std::vector;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

#pragma mark - Overloading

inline static float max(float a, float b)
{
    return a > b ? a : b;
}

// This class implements the simplest BRDF there is.
// We simply overload the rayObjectIntersection method, as there is no
// need to change the way we go through the scene

bool BRDFRayTracer::rayObjectIntersection(const Ray &ray, const Object &object, const Scene* scene, float &intersectionDistance, Vec3Df &intersectionColor)
{
    // Instead of translating the object by object.trans, we translate
    // the camera by the -object.trans. Clever, huh ?
    Ray correctedRay(ray.getOrigin() - object.getTrans(), ray.getDirection());
    
    if (!correctedRay.intersect(object.getBoundingBox(), intersectionColor))
    {
        // The ray does not intersect the bouding box, no need to go deeper
        return false;
    }
    
    // Find the triangle (if any) containing the closest intersection
    intersectionDistance = std::numeric_limits<float>::max();
    int minIntersectionIndex = -1;
    Vec3Df minIntersectionPos;
    vector<float> minCoords;
    
    Mesh mesh = object.getMesh();
    
    size_t n = mesh.getTriangles().size();
    std::vector<Triangle> triangles = object.getMesh().getTriangles();
    
    for(int i = 0 ; i < n; ++i)
    {
        Vec3Df intersectionPos;
        Triangle t = triangles[i];
        vector<float> coords;
        if (correctedRay.intersect(t, mesh, intersectionPos, coords)) {
            float triangleDistance =  Vec3Df::distance(intersectionPos, correctedRay.getOrigin());
            if(triangleDistance < intersectionDistance)
            {
                intersectionDistance = triangleDistance;
                minIntersectionPos = intersectionPos;
                minCoords = coords;
                minIntersectionIndex = i;
            }
        }
    }
    
    if (minIntersectionIndex == -1) {
        // No intersection
        return false;
    }
    
    
    
    // Phong
    Triangle t = triangles[minIntersectionIndex];
    Material mat = object.getMaterial();
    
    Vertex p0 = mesh.getVertices()[t.getVertex(0)];
    Vertex p1 = mesh.getVertices()[t.getVertex(1)];
    Vertex p2 = mesh.getVertices()[t.getVertex(2)];
    
    Vec3Df n0 = p0.getNormal();
    Vec3Df n1 = p1.getNormal();
    Vec3Df n2 = p2.getNormal();
    
    Vec3Df norm = minCoords[0] * n0 + minCoords[1] * n1 + minCoords[2] * n2;
    norm.normalize();
    
    float f = 0;
    
    for (Light light : scene->getLights()){
        
        Vec3Df wi = light.getPos() - minIntersectionPos;
        wi.normalize();
        
         f += max(mat.getDiffuse() * Vec3Df::dotProduct(norm, wi),0);
        
    }
    
    intersectionColor = 255*(f * mat.getColor());
    
    
    return true;

}