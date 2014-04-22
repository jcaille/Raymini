//
//  BasicRayTracer.h
//  RayTracer
//
//  Created by Florian Denis on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__BasicRayTracer__
#define __RayTracer__BasicRayTracer__

#include "RayTracer.h"

class BasicRayTracer : public RayTracer {
    
public:
    
    
    /**
     *  This is the default code from Boubekeur for the project : it effectively computes the intersection between the object and a bounding box
     *
     *  @param ray                  The ray
     *  @param object               The object
     *  @param intersectionDistance If the function returns true, contains the distance between the ray origin and the intersection point
     *  @param intersectionColor    The color of the ray after meeting with the object
     *
     *  @return true if there is an intersection between the object and the ray;
     */
    virtual bool rayObjectIntersection(const Ray &ray, const Object &object, float &intersectionDistance, Vec3Df &intersectionColor);
  
protected:
    friend RayTracer;
    inline BasicRayTracer () {}
    inline virtual ~BasicRayTracer () {}

    
};

#endif /* defined(__RayTracer__BasicRayTracer__) */
