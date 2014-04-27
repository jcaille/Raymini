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

# define PI 3.141592

//class Material;

class BRDF {
    
public:
    
    /**
     *  Compute the Phong's BRDF (adapted to polished and plastic surfaces)
     *
     *  @param hitPoint point of intersection between ray & mesh
     *  @param normal   normal of the mesh at the hit point
     *  @param pov      point of view
     *  @param object   object of intersection
     *  @param light    light
     *
     *  @return Color
     */
    static Vec3Df phong(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &lightPos, const Object& object, const Light& light);
    
    /**
     *  Compute the Cook-Torrance's BRDF (adapted to shading metal or other shiny and highly reflective surfaces )
     *
     *  @param hitPoint point of intersection between ray & mesh
     *  @param normal   normal of the mesh at the hit point
     *  @param pov      point of view
     *  @param object   object of intersection
     *  @param light    light
     *  @param n1       refractive index of object  (default : 1.49f => plexiglas )
     *  @param n2       refractive index of environment ( default : 1.f => air )
     *
     *  @return Color
     */
    static Vec3Df cook_torrance(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &lightPos, const Object& object, const Light& light, float n1 = 1.49f, float n2 = 1.f);
    
    
};

#endif /* defined(__RayTracer__BRDF__) */
