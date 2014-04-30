//
//  MirrorRayTracer.h
//  RayTracer
//
//  Created by Jean Caillé on 27/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__MirrorRayTracer__
#define __RayTracer__MirrorRayTracer__

#include "ExtendedLightSourcesRayTracer.h"

class MirrorRayTracer : public ExtendedLightSourcesRayTracer
{
    
    /**
     *  Default shading, basic shadow casting, Mirroring with 1-depth path tracing
     */
    
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);
    
protected:
    
    void mirrorContributionToRayColorForIntersection(const Ray& ray,
                                                     const Vec3Df& intersectionPoint,
                                                     const Vec3Df& intersectionNormal,
                                                     const Object& intersectionObject,
                                                     const Scene& scene,
                                                     Vec3Df& reflectedColor);

    
    friend RayTracer;
    MirrorRayTracer () {}
    virtual ~MirrorRayTracer () {}
  
};
#endif /* defined(__RayTracer__MirrorRayTracer__) */
