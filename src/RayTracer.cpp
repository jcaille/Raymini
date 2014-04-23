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

#include "GridAARayIterator.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

static RayTracer * instance = NULL;

RayTracer * RayTracer::getInstance () {
    if (instance == NULL){
        instance = new BasicRayTracer ();
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

bool RayTracer::raySceneIntersection(const Ray& ray, const Scene* scene, float& intersectionDistance, Vec3Df& intersectionColor)
{
    
    const BoundingBox & sceneBoundingBox = scene->getBoundingBox ();
    
    if( !ray.intersect(sceneBoundingBox, intersectionColor) )
    {
        // The base ray does not intersect with the bouding box of the scene.
        // No need to explore further.
        return false;
    }
    
    bool intersection = false;
    intersectionDistance = std::numeric_limits<float>::max();
   
    float objectIntersectionDistance;
    Vec3Df objectIntersectionColor;
    
    // For each object in the scene, check if intersection exists
    for (unsigned int k = 0; k < scene->getObjects().size(); k++) {
        
        const Object& o = scene->getObjects()[k];
        
        bool objectIntersection = rayObjectIntersection(ray, o, scene, objectIntersectionDistance, objectIntersectionColor);
        
        if (objectIntersection && objectIntersectionDistance < intersectionDistance) {
            // Keep the intersection if it is closer to the camera
            
            intersectionDistance = objectIntersectionDistance;
            intersectionColor = objectIntersectionColor;
            intersection = true;
        }
    }

    // Return if there actually was an intersection
    return intersection;
}


QImage RayTracer::render (const Vec3Df & camPos,
                          const Vec3Df & direction,
                          const Vec3Df & upVector,
                          const Vec3Df & rightVector,
                          float fieldOfView,
                          float aspectRatio,
                          unsigned int screenWidth,
                          unsigned int screenHeight) {
    
    QImage image (QSize (screenWidth, screenHeight), QImage::Format_RGB888);

    QProgressDialog progressDialog ("Raytracing...", "Cancel", 0, 100);
    progressDialog.show ();
    
    Scene* scene = Scene::getInstance();
    
    rayIterator->setCameraInformation(camPos, direction, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);
    std::vector<Ray> rays;
    for (unsigned int i = 0; i < screenWidth; i++) {
        
        std::cout << i << " " << screenWidth << std::endl;
        progressDialog.setValue ((100*i)/screenWidth);
        
        for (unsigned int j = 0; j < screenHeight; j++) {
            
            rayIterator->raysForPixel(i, j, rays);
            
            Vec3Df color;
            Vec3Df c;
            float distance;
            
            for (int r = 0; r < rays.size(); r++) {
                if (!raySceneIntersection(rays[r], scene, distance, c)) {
                    // No intersection, get a default value for c
                    c = backgroundColor;
                }
                color += c;
            }
            color /= rays.size();
            image.setPixel (i, j, qRgb (clamp (color[0], 0, 255), clamp (color[1], 0, 255), clamp (color[2], 0, 255)));
        }
    }
    progressDialog.setValue (100);
    return image;

}
