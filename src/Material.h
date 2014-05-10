// *********************************************************
// Material Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <vector>

#include "Vec3D.h"

// Ce modèle suppose une couleur spéculaire blanche (1.0, 1.0, 1.0)

class Material {
public:
    inline Material () : diffuse (0.8f), specular (0.2f), shininess(.0f), color (0.5f, 0.5f, 0.5f), reflectiveness(0), transmitance(0), refractiveIndex(1)  {}
    inline Material (float diffuse, float specular, float shininess, const Vec3Df & color, float reflectiveness = 0, float transmitance = 0, float refractiveIndex = 1)
    : diffuse (diffuse), specular (specular), shininess (shininess), color (color), reflectiveness(reflectiveness), transmitance(transmitance), refractiveIndex(refractiveIndex){}
    virtual ~Material () {}
    
    inline float getDiffuse () const { return diffuse; }
    inline float getSpecular () const { return specular; }
    inline float getShininess () const { return shininess; }
    inline Vec3Df getColor () const { return color; }
    inline float getReflectiveness() const {return reflectiveness;}
    inline float getTransmitance() const {return transmitance;}
    inline float getRefractiveIndex() const {return refractiveIndex;}
    
    inline void setDiffuse (float d) { diffuse = d; }
    inline void setSpecular (float s) { specular = s; }
    inline void setColor (const Vec3Df & c) { color = c; }
    inline void setReflectiveness(const float & r) {reflectiveness = r;}
    inline void setTransmitance(const float &t) {transmitance = t;}
    inline void setRefractiveIndex(const float &n) { refractiveIndex = n;}
    
private:
    
    float diffuse;
    float specular;
    float shininess;
    // la somme de reflectiveness + transmitance < 1, le cas d'égalité signifie que le matériaux n'est pas diffus.
    float reflectiveness;
    float transmitance;
    float refractiveIndex;
    
    Vec3Df color;
};


#endif // MATERIAL_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
