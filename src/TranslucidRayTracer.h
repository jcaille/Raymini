//
//  TranslucidRayTracer.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 02/05/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__TranslucidRayTracer__
#define __RayTracer__TranslucidRayTracer__

#include <iostream>
#include "MirrorRayTracer.h"

class TranslucidRayTracer : public MirrorRayTracer{
    
    protected :
    
    void translucidContributionToRayColorForIntersection(const Ray& ray, const Vec3Df& intersectionPoint, const Vec3Df& intersectionNormal, const Object& intersectionObject, const Scene& scene, Vec3Df& refractedColor);
    
};

#endif /* defined(__RayTracer__TranslucidRayTracer__) */
