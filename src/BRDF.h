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

class BRDF {
    
public:
    
    /**
     *  Compute the BRDF
     *
     *  @param hitPoint point of intersection between ray & mesh
     *  @param normal   normal of the mesh at the hit point
     *  @param pov      point of view
     *  @param object   object of intersection
     *  @param light    light
     *
     *  @return Color
     */
    static Vec3Df phong(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Object& object, const Light& light);
    
    
    
};

#endif /* defined(__RayTracer__BRDF__) */
