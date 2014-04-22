// *********************************************************
// Ray Tracer Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2012 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include <QtGui/QImage>
#pragma clang diagnostic pop


#include "Vec3D.h"

// Forward declaration
class Object;
class Ray;
class Scene;

class RayTracer {
public:
    static RayTracer * getInstance ();
    static void destroyInstance ();

    inline const Vec3Df & getBackgroundColor () const { return backgroundColor;}
    inline void setBackgroundColor (const Vec3Df & c) { backgroundColor = c; }
    
    QImage render (const Vec3Df & camPos,
                   const Vec3Df & viewDirection,
                   const Vec3Df & upVector,
                   const Vec3Df & rightVector,
                   float fieldOfView,
                   float aspectRatio,
                   unsigned int screenWidth,
                   unsigned int screenHeight);
    

    /**
     *  Get the color and distance of the interaction between a given ray and a given object. Overload this function to customize the behavior of the renderer. The base RayTracer uses bouding boxes to check for intersection.
     *
     *  @param ray                  The ray
     *  @param object               The object.
     *  @param intersectionDistance If the function returns true, this will contain the distance between the ray origin and the intersection point
     *  @param intersectionColor    If the function returns true, this will contain the color of the ray after intersecting the object.
     *
     *  @return True if the ray intersect the object
     */
    virtual bool rayObjectIntersection(const Ray& ray,
                               const Object& object,
                               float& intersectionDistance,
                               Vec3Df& intersectionColor) = 0;

     /**
     *  Get the color and distance of the intersection between a given ray and the global scene.
     *
     *  @param basePixelRay         The base ray
     *  @param intersectionDistance If the function returns true, this will contain the distance between the ray origin and the intersection point
     *  @param RayTracer            If the function returns true, this will contain the color of the ray after intersecting the object.
     *
     *  @return True if the ray intersects the scene;
     */
    virtual bool raySceneIntersection(const Ray& ray, const Scene* scene, float& intersectionDistance, Vec3Df& intersectionColor);
    
protected:
    inline RayTracer () {}
    inline virtual ~RayTracer () {}
    
private:
    Vec3Df backgroundColor;
};


#endif // RAYTRACER_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
