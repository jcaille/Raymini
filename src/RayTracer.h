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

#include "RayIterator.h"
#include "Vec3D.h"


// Forward declaration
class Geometry;
class Object;
class Ray;
class Scene;
class Light;
class Window;

/* RAY TRACING USER ENUM */

typedef enum ShadingFunction {
    CONSTANT,
    PHONG,
    COOK,
    CARTOON
} ShadingFunction;


class RayTracer {
public:
    /* User defined options */
    int maxRayDepth;
    int bounces;
    bool enableCastShadows;
    bool enableMirrorEffet;
    bool enableRefraction;
    ShadingFunction shadingFunction;
    
    
    static RayTracer * getInstance ();
    static void destroyInstance ();

    inline const Vec3Df & getBackgroundColor () const { return _backgroundColor;}
    inline void setBackgroundColor (const Vec3Df & c) { _backgroundColor = c; }
    

    bool render(const Vec3Df & camPos,
                   const Vec3Df & viewDirection,
                   const Vec3Df & upVector,
                   const Vec3Df & rightVector,
                   float fieldOfView,
                   float aspectRatio,
                   unsigned int screenWidth,
                   unsigned int screenHeight,
                   QImage &image);
    

    RayIterator *rayIterator;


protected:
    
    RayTracer () : maxRayDepth(10), bounces(10), enableCastShadows(true), enableMirrorEffet(true), enableRefraction(true), shadingFunction(PHONG) {}
    virtual ~RayTracer () {}
        
    /**
     *  Determines the color a ray should take given the object/light it intersects and the point & triangle of intersection
     *
     *  @param ray                  The ray
     *  @param intersectionPoint    The point of intersection
     *  @param intersectionTriangle The triangle the ray intersects
     *  @param intersectionObject   The object the ray intersects
     *  @param scene                The scene being rendered
     *  @param intersectionColor    Output parameter : will contain the color the ray should take
     */
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Light& intersectionLight,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor) = 0;
    
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor) = 0;

    
    /**
     *  Determines if a ray intersects a given geometry
     *
     *  @param ray                  the ray
     *  @param geometry             the geometry against which to check if it crosses the path of the ray
     *  @param intersectionDistance if there is intersection, will contain the position of intersection
     *  @param intersectionPoint    if there is intersection, will contain the position of intersection
     *  @param intersectionTriangle if there is intersection, will contain the triangle with which there is intersection
     *
     *  @return A boolean determining if the ray intersects a given object
     */
    virtual bool rayGeometryIntersection(const Ray& ray,
                                         const Geometry& geometry,
                                         float& intersectionDistance,
                                         Vec3Df& intersectionPoint,
                                         Triangle& intersectionTriangle) = 0;
    
    /**
     *  Determines if a ray intersects objects/lights on a scene
     *
     *  @param ray                  the ray
     *  @param scene                the scene against which to check if it crosses the path of the ray
     *  @param intersectionPoint    if there is intersection, will contain the position of intersection
     *  @param intersectionTriangle if there is intersection, will contain the triangle with which there is intersection
     *  @param intersectionObject   if there is intersection, will point to the object intersecting the ray
     *
     *  @return A boolean determining if the ray intersects a given object
     */
    bool raySceneIntersection(const Ray& ray,
                              const Scene& scene,
                              float& intersectionDistance,
                              Vec3Df& intersectionPoint,
                              Triangle& intersectionTriangle,
                              const Object* &intersectionObject);
    
    bool raySceneIntersection(const Ray& ray,
                              const Scene& scene,
                              float& intersectionDistance,
                              Vec3Df& intersectionPoint,
                              Triangle& intersectionTriangle,
                              const Light* &intersectionLight);
    
    /**
     *  Get the color to assign to a given ray given its intersection with the scene
     *
     *  @param ray                  The ray to make interact with the scene
     *  @param scene                The scene to render
     *  @param intersectionColor    The color that result from the interaction of the ray with the scene
     */
    virtual void raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor);


private:

    
    Vec3Df _backgroundColor;
};


#endif // RAYTRACER_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
