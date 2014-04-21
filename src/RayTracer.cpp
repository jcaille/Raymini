// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#include <QProgressDialog>
#pragma clang diagnostic pop

static RayTracer * instance = NULL;

RayTracer * RayTracer::getInstance () {
    if (instance == NULL){
        instance = new RayTracer ();
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

inline int clamp (float f, int inf, int sup) {
    int v = static_cast<int> (f);
    return (v < inf ? inf : (v > sup ? sup : v));
}

bool RayTracer::rayObjectIntersection(Ray ray, Object object, float& intersectionDistance, Vec3Df& intersectionColor)
{
   
    // Instead of translating the object by object.trans, we translate
    // the camera by the -object.trans. Clever, huh ?
    Ray correctedRay = Ray(ray.getOrigin() - object.getTrans(), ray.getDirection());

    bool hasIntersection = correctedRay.intersect (object.getBoundingBox (), intersectionColor);
    if (hasIntersection) {
        intersectionDistance = Vec3Df::squaredDistance (intersectionColor, correctedRay.getOrigin());
        intersectionColor = 255 * object.getMaterial().getColor();
        return true;
    }
    return false;
}

 bool RayTracer::raySceneIntersection(const Ray ray, const Scene* scene, float& intersectionDistance, Vec3Df& intersectionColor)
{
    
    const BoundingBox & sceneBoundingBox = scene->getBoundingBox ();
    
    if( !ray.intersect(sceneBoundingBox, intersectionColor) )
    {
        // The base ray does not intersect with the bouding box of the scene.
        // No need to explore further.
        return false;
    }
    
    bool intersection = false;
    intersectionDistance = 1000000000; // Store the local Z-buffer in the return parameter
   
    float objectIntersectionDistance;
    Vec3Df objectIntersectionColor;
    
    // For each object in the scene, check if intersection exists
    for (unsigned int k = 0; k < 2; k++) {
        
        const Object& o = scene->getObjects()[k];
        
        bool objectIntersection = rayObjectIntersection(ray, o, objectIntersectionDistance, objectIntersectionColor);
        
        if (objectIntersection && objectIntersectionDistance < intersectionDistance) {
            // Keep the intersection if it is closer to the camera
            
            intersectionDistance = objectIntersectionDistance;
            intersectionDistance = objectIntersectionDistance;
            intersectionColor = objectIntersectionColor;
            intersection = true;
        }
    }

    // Return if there actually was an intersection
    return intersection;
}

// POINT D'ENTREE DU PROJET.
// Le code suivant ray trace uniquement la boite englobante de la scene.
// Il faut remplacer ce code par une veritable raytracer
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
    
    for (unsigned int i = 0; i < screenWidth; i++) {
        std::cout << i << " " << screenWidth << std::endl;
        progressDialog.setValue ((100*i)/screenWidth);
        for (unsigned int j = 0; j < screenHeight; j++) {
            
            // Get the ray starting at camPos and going through the (i,j) pixel
            
            float tanX = tan (fieldOfView)*aspectRatio;
            float tanY = tan (fieldOfView);
            Vec3Df stepX = (float (i) - screenWidth/2.f)/screenWidth * tanX * rightVector;
            Vec3Df stepY = (float (j) - screenHeight/2.f)/screenHeight * tanY * upVector;
            Vec3Df step = stepX + stepY;
            Vec3Df dir = direction + step;
            dir.normalize ();
            Ray ray(camPos, dir);

            Vec3Df c;
            float distance;

            if (!raySceneIntersection(ray, scene, distance, c)) {
                // No intersection, get a default value for c
                c = backgroundColor;
            }
            image.setPixel (i, j, qRgb (clamp (c[0], 0, 255), clamp (c[1], 0, 255), clamp (c[2], 0, 255)));
        }
    }
    progressDialog.setValue (100);
    return image;
}
