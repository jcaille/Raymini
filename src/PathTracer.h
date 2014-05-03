//
//  PathTracer.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__PathTracer__
#define __RayTracer__PathTracer__

#include "MirrorRayTracer.h"
#include "TranslucidRayTracer.h"

class PathTracer : public TranslucidRayTracer{
public:
    
//    void raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor);
//    void buildRecursivePath(const Ray& ray, const Scene& scene, Vec3Df &intersectionColor);

    /**
     *  Direct + indirect light contribution by path tracing
     */
    
    virtual void rayColorForIntersection(const Ray& ray,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);

    
protected:
    
    void indirectContributionToRayColorForIntersection(const Ray& ray,
                                                       const Vec3Df& intersectionPoint,
                                                       const Vec3Df& intersectionNormal,
                                                       const Object& intersectionObject,
                                                       const Scene& scene,
                                                       Vec3Df& indirectLightContribution);

    void allDiffuseContributionToRayColorForIntersection(const Ray& ray,
                                                         const Vec3Df& intersectionPoint,
                                                         const Vec3Df& intersectionNormal,
                                                         const Object& intersectionObject,
                                                         const Scene& scene,
                                                         Vec3Df& diffuseLightContribution);

    
};

#endif /* defined(__RayTracer__PathTracer__) */