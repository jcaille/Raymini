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
#include "Mesh.h"

class Object;

class Ray {
public:
    inline Ray () {}
    inline Ray (const Vec3Df & origin, const Vec3Df & direction) : _depth(1), _origin(origin), _direction(direction), _shouldBounce(true) {}
    inline virtual ~Ray () {}

    inline const Vec3Df & getOrigin () const { return _origin; }
    inline Vec3Df & getOrigin () { return _origin; }
    inline const Vec3Df & getDirection () const { return _direction; }
    inline Vec3Df & getDirection () { return _direction; }

    inline const int getDepth () const { return _depth; }
    inline void setDepth (int depth) { _depth = depth; }

    inline const bool shouldBounce () const { return _shouldBounce; }
    inline void setShouldBounce (bool bounce) { _shouldBounce = bounce; }

    
    // Intersection w/ a bounding box
    bool intersect (const BoundingBox & bbox, Vec3Df & intersectionPoint) const;
    
    // Intersection w/ a single triangle
    bool intersect(const Triangle& tri,
                   const std::vector<Vertex>& vertices,
                   float& intersectionDistance,
                   Vec3Df& intersectionPoint) const;
    
  
private:
    bool _shouldBounce;
    int _depth;
    Vec3Df _origin;
    Vec3Df _direction;
};


#endif // RAY_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
