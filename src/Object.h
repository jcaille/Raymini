// *********************************************************
// Object Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "BoundingBox.h"
#include "KDTree.h"

class Ray;

class Object {
public:
    inline Object (const Mesh & aMesh, const Material & aMat) : mesh (aMesh), mat (aMat), tree(mesh,10) {
        updateBoundingBox ();
    }
    
    virtual ~Object () {}

    inline const Vec3Df & getTrans () const { return trans;}
    inline void setTrans (const Vec3Df & t) { trans = t; }

    inline const Mesh & getMesh () const { return mesh; }
    inline Mesh & getMesh () { return mesh; }
    
    inline const Material & getMaterial () const { return mat; }
    inline Material & getMaterial () { return mat; }

    inline const BoundingBox & getBoundingBox () const { return bbox; }
    void updateBoundingBox ();
    
    inline const KDTree & getKDTree () const { return tree; }

private:
    Mesh mesh;
    Material mat;
    BoundingBox bbox;
    Vec3Df trans;
    KDTree tree;
};


#endif // Scene_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
