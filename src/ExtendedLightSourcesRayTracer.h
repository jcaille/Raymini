//
//  ExtendedLightSources.h
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__SmoothShadowRayTracer__
#define __RayTracer__SmoothShadowRayTracer__

#include "KDTreeRayTracer.h"

class ExtendedLightSourcesRayTracer : public KDTreeRayTracer {
    
    
    /**
     *  Default Phong BRDF shading with smooth shadows
     */
    
    virtual void rayColorForIntersection(const Vec3Df& pov,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);
    
    
    
protected:
    friend RayTracer;
    ExtendedLightSourcesRayTracer () {}
    virtual ~ExtendedLightSourcesRayTracer () {}

};

#endif /* defined(__RayTracer__SmoothShadowRayTracer__) */