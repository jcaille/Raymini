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

#include "Geometry.h"
#include "Material.h"

class Ray;

class Object : public Geometry
{
public:
    
    inline Object() {}
    inline ~Object () {}
    
    inline Object (const Mesh & mesh, const Material & aMat) : Geometry(mesh), mat (aMat) {}
    
    inline const Material & getMaterial () const { return mat; }
    inline Material & getMaterial () { return mat; }


private:
    Material mat;
};


#endif // Scene_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
