//
//  PathTracer.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__PathTracer__
#define __RayTracer__PathTracer__

#include <iostream>
#include "Path.h"
#include "KDTreeRayTracer.h"

#endif /* defined(__RayTracer__PathTracer__) */

class PathTracer : public KDTreeRayTracer{
public:
    
    void raySceneInteraction(const Ray& ray, const Scene& scene, Vec3Df& intersectionColor);
    void buildRecursivePath(const Ray& ray, const Scene& scene, Vec3Df &intersectionColor);
    
private:

    Path _path;
    Vec3Df _directIntersectionColor;

};