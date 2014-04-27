//
//  MirrorRayTracer.h
//  RayTracer
//
//  Created by Jean Caillé on 27/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__MirrorRayTracer__
#define __RayTracer__MirrorRayTracer__

#include <iostream>
#include "KDTreeRayTracer.h"

class MirrorRayTracer : public KDTreeRayTracer
{
    
    /**
     *  Default Phong BRDF shading, basic shadow casting, Mirroring with 1-depth path tracing
     */
    
    virtual void rayColorForIntersection(const Vec3Df& pov,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);
    
protected:
    
    virtual void rayColorWithoutReflection(const Vec3Df& pov,
                                           const Vec3Df intersectionPoint,
                                           const Vec3Df intersectionNormal,
                                           const Object& intersectionObject,
                                           const Scene& scene,
                                           Vec3Df& intersectionColor);
    
    friend KDTreeRayTracer;
    friend RayTracer;
    
    MirrorRayTracer () {}
    virtual ~MirrorRayTracer () {}
  
};
#endif /* defined(__RayTracer__MirrorRayTracer__) */
