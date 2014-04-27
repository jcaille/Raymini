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
    
    inline Light () : _color(1.0f, 1.0f, 1.0f), _intensity(1.0f) {}
    inline Light (const Vec3Df & pos, const Vec3Df & color, float intensity) : _color(color), _intensity(intensity) {
            setTrans(pos);
            _samples.push_back(pos);
        }
    
    inline Light (const Vec3Df & pos, const Mesh & mesh, const Vec3Df & color, float intensity) : Geometry(mesh), _color(color), _intensity(intensity) {
        setTrans(pos);
        _samples.push_back(pos);
        sample(200,_samples);
    }

    virtual ~Light () {}

    inline float getIntensity () const { return _intensity; }
    inline const Vec3Df & getPos () const { return getTrans(); }
    inline const Vec3Df & getColor () const { return _color; }
    inline const vector<Vec3Df> & getSamples () const { return _samples; }


    inline void setPos (const Vec3Df & p) { setTrans(p); }
    inline void setColor (const Vec3Df & c) { _color = c; }
    inline void setIntensity (float i) { _intensity = i; }

    
private:
    Vec3Df _color;
    float _intensity;
    vector<Vec3Df> _samples;
};


#endif // LIGHT_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
