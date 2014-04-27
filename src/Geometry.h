//
//  Geometry.h
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__Geometry__
#define __RayTracer__Geometry__

#include "KDTree.h"
#include "BoundingBox.h"

class Geometry {
    
public:

    inline Geometry(){};
    
    inline Geometry(const Mesh& mesh) : _tree(mesh) {
        _updateBoundingBox();
    }
    
    inline ~Geometry(){}
    
    inline const BoundingBox& getBoundingBox () const { return _bbox; }
    
    inline const KDTree& getKDTree() const { return _tree; }

    inline const Mesh& getMesh () const { return _tree.getMesh(); }
    inline Mesh& getMesh () { return _tree.getMesh(); }
    
    inline const Vec3Df & getTrans () const { return _trans;}
    inline void setTrans (const Vec3Df & t) { _trans = t; }


    
private:
    
    void _updateBoundingBox();

    KDTree _tree;
    BoundingBox _bbox;
    Vec3Df _trans;
};

#endif /* defined(__RayTracer__Geometry__) */
