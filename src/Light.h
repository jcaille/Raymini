// *********************************************************
// Light Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef LIGHT_H
#define LIGHT_H

#include "Geometry.h"

#include "Vec3D.h"

class Light : public Geometry {
public:
    
    inline Light () : color (Vec3Df (1.0f, 1.0f, 1.0f)), intensity (1.0f) {}
    inline Light (const Vec3Df & pos, const Vec3Df & color, float intensity) : color (color), intensity (intensity) {
            setTrans(pos);
        }
    
    inline Light (const Vec3Df & pos, const Mesh & mesh, const Vec3Df & color, float intensity) : Geometry(mesh), color (color), intensity (intensity) {
        setTrans(pos);
    }

    virtual ~Light () {}

    inline const Vec3Df & getPos () const { return getTrans(); }
    inline const Vec3Df & getColor () const { return color; }
    inline float getIntensity () const { return intensity; }

    inline void setPos (const Vec3Df & p) { setTrans(p); }
    inline void setColor (const Vec3Df & c) { color = c; }
    inline void setIntensity (float i) { intensity = i; }

    
private:
    Vec3Df color;
    float intensity;
};


#endif // LIGHT_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
