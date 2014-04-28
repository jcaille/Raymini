// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "PathTracer.h"

#include "BasicRayTracer.h"
#include "BoundingBoxRayTracer.h"
#include "KDTreeRayTracer.h"
#include "ExtendedLightSourcesRayTracer.h"

#include "GridAARayIterator.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

static RayTracer * instance = NULL;

RayTracer * RayTracer::getInstance () {
    if (instance == NULL){
        instance = new PathTracer();
        std::cout << "Creating raytracer" << std::endl;
    }
    return instance;
}

void RayTracer::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        std::cout << "Deleting raytracer" << std::endl;
        instance = NULL;
    }
}

static inline int clamp (float f, int inf, int sup) {
    int v = static_cast<int> (f);
    return (v < inf ? inf : (v > sup ? sup : v));
}

bool RayTracer::raySceneIntersection(const Ray& ray, const Scene& scene, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle, const Object* &intersectionObject)
{
    
    const BoundingBox& sceneBoundingBox = scene.getBoundingBox();
    
    if(!ray.intersect(sceneBoundingBox, intersectionPoint)){
        // The ray does not intersect with the bouding box of the scene.
        return false;
    }
    
    bool intersection = false;
    
    // Determine the first point of intersection between the ray and the scene
    intersectionDistance = std::numeric_limits<float>::max();
    
    float objectIntersectionDistance;
    Vec3Df objectIntersectionPoint;
    Triangle objectIntersectionTriangle;
    
    // For each object in the scene, check if intersection exists and remember point of intersection
    const vector<Object>& objects = scene.getObjects();
    
    for (const Object& object : objects) {
        
        // Instead of translating the object by object.trans, we translate
        // the ray by the -object.trans.
        Ray correctedRay(ray.getOrigin() - object.getTrans(), ray.getDirection());
        
        bool objectIntersection = rayGeometryIntersection(correctedRay, object, objectIntersectionDistance, objectIntersectionPoint, objectIntersectionTriangle);
        
        if (objectIntersection && objectIntersectionDistance < intersectionDistance) {
            
            // Keep the intersection if it is closer to the camera
            intersectionDistance = objectIntersectionDistance;
            intersectionPoint = objectIntersectionPoint;
            intersectionTriangle = objectIntersectionTriangle;
            intersectionObject = &object;
            intersection = true;
        }
    }
    
    return intersection;
}

bool RayTracer::raySceneIntersection(const Ray& ray, const Scene& scene, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle, const Light* &intersectionLight)
{
    
    const BoundingBox& sceneBoundingBox = scene.getBoundingBox();
    
    if(!ray.intersect(sceneBoundingBox, intersectionPoint)){
        // The ray does not intersect with the bouding box of the scene.
        return false;
    }
    
    bool intersection = false;
    
    // Determine the first point of intersection between the ray and the scene
    intersectionDistance = std::numeric_limits<float>::max();
    
    float lightIntersectionDistance;
    Vec3Df lightIntersectionPoint;
    Triangle lightIntersectionTriangle;
    
    // For each object in the scene, check if intersection exists and remember point of intersection
    const vector<Light>& lights = scene.getLights();
    
    for (const Light& light : lights) {
        
        // Instead of translating the object by object.trans, we translate
        // the ray by the -object.trans.
        Ray correctedRay(ray.getOrigin() - light.getTrans(), ray.getDirection());
        
        bool objectIntersection = rayGeometryIntersection(correctedRay, light, lightIntersectionDistance, lightIntersectionPoint, lightIntersectionTriangle);
        
        if (objectIntersection && lightIntersectionDistance < intersectionDistance) {
            
            // Keep the intersection if it is closer to the camera
            intersectionDistance = lightIntersectionDistance;
            intersectionPoint = lightIntersectionPoint;
            intersectionTriangle = lightIntersectionTriangle;
            intersectionLight = &light;
            intersection = true;
        }
    }
    
    return intersection;
}



void RayTracer::raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor)
{
    
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

    
    // No ray / scene intersection => background color
    if (!objectIntersection && !lightIntersection){
        intersectionColor = _backgroundColor;
        return;
    }
    
    if (lightIntersectionDistance <= objectIntersectionDistance){
        
        intersectionColor = lightIntersectionObject->getColor();
        
    } else {
        
        // We need to compute the normal on the object at the intersection point
        const Mesh& mesh = objectIntersectionObject->getMesh();
        
        // Get barycentric coordinates
        std::vector<float> coords;
        mesh.barycentricCoordinates(objectIntersectionPoint, objectIntersectionTriangle, coords);
        
        // Compute normal at intersection
        Vertex p0 = mesh.getVertices()[objectIntersectionTriangle.getVertex(0)];
        Vertex p1 = mesh.getVertices()[objectIntersectionTriangle.getVertex(1)];
        Vertex p2 = mesh.getVertices()[objectIntersectionTriangle.getVertex(2)];
        
        Vec3Df n0 = p0.getNormal();
        Vec3Df n1 = p1.getNormal();
        Vec3Df n2 = p2.getNormal();
        
        Vec3Df norm = coords[0] * n0 + coords[1] * n1 + coords[2] * n2;
        norm.normalize();
        
        
        // Now that we have that point of intersection, let's compute the color it is supposed to have
        rayColorForIntersection(ray.getOrigin(), objectIntersectionPoint + objectIntersectionObject->getTrans(), norm, *objectIntersectionObject, scene, intersectionColor);
        

    }
    
}


QImage RayTracer::render (const Vec3Df & camPos,
                          const Vec3Df & direction,
                          const Vec3Df & upVector,
                          const Vec3Df & rightVector,
                          float fieldOfView,
                          float aspectRatio,
                          unsigned int screenWidth,
                          unsigned int screenHeight) {
    

    QProgressDialog progressDialog ("Raytracing...", "Cancel", 0, 100);
    progressDialog.show ();
    
    Scene* scene = Scene::getInstance();
    
    rayIterator->setCameraInformation(camPos, direction, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);
    
    QImage image(screenWidth, screenHeight, QImage::Format_RGB888);
    
    std::vector<Ray> rays;
    for (unsigned int i = 0; i < screenWidth; i++) {
        
        std::cout << i << " " << screenWidth << std::endl;
        progressDialog.setValue ((100*i)/screenWidth);
        
        for (unsigned int j = 0; j < screenHeight; j++) {
            
            rayIterator->raysForPixel(i, j, rays);
            
            Vec3Df pixelColor;
            Vec3Df rayColor;
            
            for (const Ray& ray : rays) {
                raySceneInteraction(ray, *scene, rayColor);
                pixelColor += rayColor;
            }
            pixelColor *= 255/float(rays.size());
            
            QRgb rgb = qRgb(clamp(pixelColor[0], 0, 255), clamp(pixelColor[1], 0, 255), clamp(pixelColor[2], 0, 255));

            image.setPixel(i, j, rgb);
        }
    }
    progressDialog.setValue(100);
    return image;

}
