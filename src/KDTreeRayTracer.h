//
//  KDTreeRayTracer.h
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__KDTreeRayTracer__
#define __RayTracer__KDTreeRayTracer__

#include "BasicRayTracer.h"

class KDTreeRayTracer : public BasicRayTracer {
    
    /**
     *  Compute intersections using a KDTree
     */
    virtual bool rayGeometryIntersection(const Ray& ray,
                                         const Geometry& geometry,
                                         float& intersectionDistance,
                                         Vec3Df& intersectionPoint,
                                         Triangle& intersectionTriangle);

    
protected:
    friend RayTracer;
    KDTreeRayTracer () {}
    virtual ~KDTreeRayTracer () {}

    
};

#endif /* defined(__RayTracer__KDTreeRayTracer__) */
