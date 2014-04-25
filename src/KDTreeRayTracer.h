//
//  KDTreeRayTracer.h
//  RayTracer
//
//  Created by Florian Denis on 25/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__KDTreeRayTracer__
#define __RayTracer__KDTreeRayTracer__

#include "RayTracer.h"

class KDTreeRayTracer : public RayTracer
{
    
    public :
    
    /**
     *  We overload this method to modify the way the ray interact with an object
     *  Instead of simply checking if we intersect the bouding box, we will first seek if the ray intersect a triangle
     *  If so, we will compute the BRDF at the intersection point, and pass that color back to the ray
     *
     *  @param ray                  The ray
     *  @param object               The object
     *  @param intersectionDistance If the function returns true, contains the distance between the ray origin and the intersection point
     *  @param intersectionColor    The color of the ray after meeting with the object
     *
     *  @return true if there is an intersection between the object and the ray;
     */
    virtual bool rayObjectIntersection(const Ray &ray, const Object &object, const Scene* scene, float &intersectionDistance, Vec3Df &intersectionColor);
    
protected:
    friend RayTracer;
    inline KDTreeRayTracer () {}
    inline virtual ~KDTreeRayTracer () {}
    
};


#endif /* defined(__RayTracer__KDTreeRayTracer__) */
