// *********************************************************
// Ray Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <vector>

#include "Vec3D.h"
#include "BoundingBox.h"
#include "Triangle.h"

class Ray {
public:
    inline Ray () {}
    inline Ray (const Vec3Df & origin, const Vec3Df & direction)
        : origin (origin), direction (direction) {}
    inline virtual ~Ray () {}

    inline const Vec3Df & getOrigin () const { return origin; }
    inline Vec3Df & getOrigin () { return origin; }
    inline const Vec3Df & getDirection () const { return direction; }
    inline Vec3Df & getDirection () { return direction; }

    bool intersect (const BoundingBox & bbox, Vec3Df & intersectionPoint) const;
    
    /**
     *  Checks if the ray intersects with the given triangle. We use the Möller-Trumbore algorithm described
     *  in http://en.wikipedia.org/wiki/Moller-Trumbore_intersection_algorithm
     *
     *  @param a                    The first vertex of the triangle
     *  @param b                    The second vertex of the triangle
     *  @param c                    The third vertex of the triangle
     *  @param intersectionDistance If the method returns true, this parameter will contain the distance from the ray origin to the intersection point.
     *
     *  @return true if the Ray and the triangle have a point in common (keeping in mind accury issues)
     */
    bool intersectTriangle(const Vec3Df& a, const Vec3Df& b, const Vec3Df& c, float &intersectionDistance) const;
    
private:
    Vec3Df origin;
    Vec3Df direction;
};


#endif // RAY_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
