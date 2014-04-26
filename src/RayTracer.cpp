// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"

#include "BasicRayTracer.h"
#include "BoundingBoxRayTracer.h"
#include "KDTreeRayTracer.h"

#include "GridAARayIterator.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

static RayTracer * instance = NULL;

RayTracer * RayTracer::getInstance () {
    if (instance == NULL){
        instance = new BoundingBoxRayTracer ();
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

bool RayTracer::raySceneIntersection(const Ray& ray, const Scene& scene, Vec3Df& intersectionPoint, Triangle& intersectionTriangle, const Object* &intersectionObject)
{
    
    const BoundingBox& sceneBoundingBox = scene.getBoundingBox();
    
    if(!ray.intersect(sceneBoundingBox, intersectionPoint)){
        // The ray does not intersect with the bouding box of the scene.
        return false;
    }
    
    bool intersection = false;
    
    // Determine the first point of intersection between the ray and the scene
    float intersectionDistance = std::numeric_limits<float>::max();
    
    float objectIntersectionDistance;
    Vec3Df objectIntersectionPoint;
    Triangle objectIntersectionTriangle;
    
    // For each object in the scene, check if intersection exists and remember point of intersection
    const vector<Object>& objects = scene.getObjects();
    
    for (const Object& object : objects) {
        
        // Instead of translating the object by object.trans, we translate
        // the ray by the -object.trans.
        Ray correctedRay(ray.getOrigin() - object.getTrans(), ray.getDirection());
        
        bool objectIntersection = rayObjectIntersection(correctedRay, object, objectIntersectionDistance, objectIntersectionPoint, objectIntersectionTriangle);
        
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



void RayTracer::raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor)
{
    
    // Determine where the ray intersects the scene
    const Object* intersectionObject;
    Vec3Df intersectionPoint;
    Triangle intersectionTriangle;
    bool intersection = raySceneIntersection(ray, scene, intersectionPoint, intersectionTriangle, intersectionObject);
    
    // No ray / scene intersection => background color
    if (!intersection){
        intersectionColor = _backgroundColor;
        return;
    }
    
    // Now that we have that point of intersection, let's compute the color it is supposed to have
    rayColorForIntersection(ray, intersectionPoint, intersectionTriangle, *intersectionObject, intersectionColor);
    
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
            pixelColor /= rays.size();
            
            QRgb rgb = qRgb(clamp(pixelColor[0], 0, 255), clamp(pixelColor[1], 0, 255), clamp(pixelColor[2], 0, 255));

            image.setPixel(i, j, rgb);
        }
    }
    progressDialog.setValue(100);
    return image;

}
