//
//  BasicRayTracer.h
//  RayTracer
//
//  Created by Jean Caillé on 21/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__BRDFRayTracer__
#define __RayTracer__BRDFRayTracer__

#include "RayTracer.h"

class BasicRayTracer : public RayTracer
{

public :
    
    virtual Vec3Df brdf(const Vec3Df &hitPoint, const Vec3Df& normal, const Vec3Df &pov, const Vec3Df &lightPos, const Object& object, const Light& light);
    
    /**
     *  A basic intersection finder that loops over all triangles of the given object
     */
    virtual bool rayGeometryIntersection(const Ray& ray,
                                         const Geometry& geometry,
                                         float& intersectionDistance,
                                         Vec3Df& intersectionPoint,
                                         Triangle& intersectionTriangle);
    
    /**
     *  Default Phong BRDF shading
     */
    
    virtual void rayColorForIntersection(const Vec3Df& pov,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Object& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);
    
    virtual void rayColorForIntersection(const Vec3Df& pov,
                                         const Vec3Df& intersectionPoint,
                                         const Vec3Df& intersectionNormal,
                                         const Light& intersectionObject,
                                         const Scene& scene,
                                         Vec3Df& intersectionColor);


protected:
    friend RayTracer;
    BasicRayTracer () {}
    virtual ~BasicRayTracer () {}

};


#endif /* defined(__RayTracer__BRDFRayTracer__) */
