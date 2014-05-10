//
//  KDTreeRayTracer.h
//  RayTracer
//
//  Created by Florian Denis on 25/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__ShadowRayTracer__
#define __RayTracer__ShadowRayTracer__

#include "KDTreeRayTracer.h"

class ShadowRayTracer : public KDTreeRayTracer
{
        
    /**
     *  Default shading, basic shadow casting
     */
    
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);
    
protected:

    
    virtual float lightContributionPowerToRayColorForIntersection(const Vec3Df& intersectionPoint,
                                                                  const Vec3Df& lightPos,
                                                                  const Scene& scene);

    
    void lightContributionToRayColorForIntersection(const Ray& ray,
                                                    const Vec3Df& intersectionPoint,
                                                    const Vec3Df& intersectionNormal,
                                                    const Vec3Df& lightPos,
                                                    const Light& light,
                                                    const Object& intersectionObject,
                                                    const Scene& scene,
                                                    Vec3Df& lightContribution);


    
    friend RayTracer;
    ShadowRayTracer () {}
    virtual ~ShadowRayTracer () {}
    
};


#endif /* defined(__RayTracer__KDTreeRayTracer__) */
