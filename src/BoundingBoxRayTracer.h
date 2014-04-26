//
//  BoundingBoxRayTracer.h
//  RayTracer
//
//  Created by Florian Denis on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__BasicRayTracer__
#define __RayTracer__BasicRayTracer__

#include "RayTracer.h"

class BoundingBoxRayTracer : public RayTracer {
    
public:
    
    
    /**
     *  This is the default code from Boubekeur for the project : it effectively computes the intersection between the object and a bounding box
     */
    virtual bool rayObjectIntersection(const Ray& ray,
                                       const Object& object,
                                       float& intersectionDistance,
                                       Vec3Df& intersectionPoint,
                                       Triangle& intersectionTriangle);
    
    /**
     *  This is the default code from Boubekeur for the project : it affects the color of the material for the intersectionObject
     */
  
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Triangle& intersectionTriangle,
                                         const Object& intersectionObject,
                                         Vec3Df& intersectionColor);

protected:
    friend RayTracer;
    BoundingBoxRayTracer () {}
    virtual ~BoundingBoxRayTracer () {}

    
};

#endif /* defined(__RayTracer__BasicRayTracer__) */
