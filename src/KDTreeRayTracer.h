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
    
    /**
     *  Compute intersections using a KDTree
     */
    virtual bool rayObjectIntersection(const Ray& ray,
                                       const Object& object,
                                       float& intersectionDistance,
                                       Vec3Df& intersectionPoint,
                                       Triangle& intersectionTriangle);
    
    /**
     *  Default Phong BRDF shading
     */
    
    virtual void rayColorForIntersection(const Vec3Df& pov,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);


    
protected:
    friend RayTracer;
    KDTreeRayTracer () {}
    virtual ~KDTreeRayTracer () {}
    
};


#endif /* defined(__RayTracer__KDTreeRayTracer__) */
