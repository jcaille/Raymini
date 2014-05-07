// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"

#include "Window.h"

#include "BasicRayTracer.h"
#include "ShadowRayTracer.h"
#include "BoundingBoxRayTracer.h"
#include "ExtendedLightSourcesRayTracer.h"
#include "MirrorRayTracer.h"
#include "PathTracer.h"

#include "GridAARayIterator.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

#define PROGRESS_BAR_SIZE 50

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif


static RayTracer * instance = NULL;

RayTracer* RayTracer::getInstance () {
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
    
    // || (lightIntersection && objectIntersection && objectIntersectionObject->getMaterial().getTransmitance() > 0.2f)
    if ( (lightIntersectionDistance <= objectIntersectionDistance) ){
        
        // We need to compute the normal on the object at the intersection point
        const Mesh& mesh = lightIntersectionObject->getMesh();

        // Get barycentric coordinates
        std::vector<float> coords;
        mesh.barycentricCoordinates(lightIntersectionPoint, lightIntersectionTriangle, coords);
        
        Vec3Df norm = mesh.getNormal(lightIntersectionTriangle, coords);
        
        // Now that we have that point of intersection, let's compute the color it is supposed to have
        rayColorForIntersection(ray, lightIntersectionPoint + lightIntersectionObject->getTrans(), norm, *lightIntersectionObject, scene, intersectionColor);
        
    } else {
        
        // We need to compute the normal on the object at the intersection point
        const Mesh& mesh = objectIntersectionObject->getMesh();
        
        // Get barycentric coordinates
        std::vector<float> coords;
        mesh.barycentricCoordinates(objectIntersectionPoint, objectIntersectionTriangle, coords);
        Vec3Df norm = mesh.getNormal(objectIntersectionTriangle, coords);
        
        // Now that we have that point of intersection, let's compute the color it is supposed to have
        rayColorForIntersection(ray, objectIntersectionPoint + objectIntersectionObject->getTrans(), norm, *objectIntersectionObject, scene, intersectionColor);
        

    }
}


bool RayTracer::render (const Vec3Df & camPos,
                          const Vec3Df & direction,
                          const Vec3Df & upVector,
                          const Vec3Df & rightVector,
                          float fieldOfView,
                          float aspectRatio,
                          unsigned int screenWidth,
                          unsigned int screenHeight,
                          QImage & image) {
    
    
    Scene* scene = Scene::getInstance();
    
    rayIterator->setCameraInformation(camPos, direction, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);
    
    for (unsigned int i = 0; i < screenWidth; i++) {
        
#if defined(__APPLE__)
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        __block vector<QRgb> columnsColor;
        columnsColor.resize(screenHeight);
        dispatch_apply(screenHeight, queue, ^(size_t idx) {
            int j = static_cast<int>(idx);
            
#else
        vector<QRgb> columnsColor;
        columnsColor.resize(screenHeight);
#if defined(_OPENMP)
#pragma omp parallel for
#endif
        for (unsigned int j = 0; j < screenHeight; j++) {
#endif
            std::vector<Ray> rays;
            rayIterator->raysForPixel(i, j, rays);
            
            Vec3Df pixelColor;
            Vec3Df rayColor;
            
            for (const Ray& ray : rays) {
                raySceneInteraction(ray, *scene, rayColor);
                pixelColor += rayColor;
            }
            pixelColor *= 255/float(rays.size());
            
            QRgb rgb = qRgb(clamp(pixelColor[0], 0, 255), clamp(pixelColor[1], 0, 255), clamp(pixelColor[2], 0, 255));
            columnsColor[j] = rgb;


#if defined(__APPLE__)
        });
#else
        }
#endif
       for(int j = 0 ; j < screenHeight; j++)
       {
           image.setPixel(i, j, columnsColor[j]);
       }
    }
                            

    return true;

}
