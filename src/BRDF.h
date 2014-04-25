//
//  BRDF.h
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__BRDF__
#define __RayTracer__BRDF__

#include "Vec3D.h"
#include "Scene.h"
#include "Object.h"

//class Material;

class Phong {
    
public:
    
    /**
     *  Compute the BRDF
     *
     *  @param hitPoint point of intersection between ray & mesh
     *  @param pov      point of view
     *  @param coords   coordinates of the hitpoint in the barycentric coordinate frame
     *  @param t        triangle of intersection
     *  @param object   object of intersection
     *  @param scene    scene
     *
     *  @return Color
     */
    static Vec3Df brdf(const Vec3Df& hitPoint, const Vec3Df& pov, const std::vector<float> coords, const Triangle& t, const Object &object, const Scene *scene);
    
    static Vec3Df singleLightBRDF(const Vec3Df &hitPoint, const Vec3Df &pov, const Vec3Df normal, Material material, Light light);
    
    
};

#endif /* defined(__RayTracer__BRDF__) */
