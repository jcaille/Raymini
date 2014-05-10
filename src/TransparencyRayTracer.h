//
//  TransparencyRayTracer.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 05/05/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__TransparencyRayTracer__
#define __RayTracer__TransparencyRayTracer__

#include "MirrorRayTracer.h"

class TransparencyRayTracer : public MirrorRayTracer {
    
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

    void refractedContributionToRayColorForIntersection(const Ray& ray,
                                                        const Vec3Df& intersectionPoint,
                                                        const Vec3Df& intersectionNormal,
                                                        const Object& intersectionObject,
                                                        const Scene& scene,
                                                        Vec3Df& refractedContribution);

    
    friend RayTracer;
    TransparencyRayTracer () {}
    virtual ~TransparencyRayTracer () {}

};

#endif /* defined(__RayTracer__TransparencyRayTracer__) */
