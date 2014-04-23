//
//  RayIterator.h
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__RayIterator__
#define __RayTracer__RayIterator__

#include "Ray.h"
#include "Vec3D.h"
#include <vector>

class RayIterator {

protected:
    // informations about scene
    Vec3Df _camPos;
    Vec3Df _direction;
    Vec3Df _upVector;
    Vec3Df _rightVector;
    float _fieldOfView;
    float _aspectRatio;
    unsigned int _screenWidth;
    unsigned int _screenHeight;
    int _idx;
    
    
public:
    
    virtual void setCameraInformation(Vec3Df cameraPosition, Vec3Df direction, Vec3Df upVector, Vec3Df rightVector, float fov, float ar, unsigned int screenWidth, unsigned int screenHeight);
    
    virtual void raysForPixel(int i, int j, std::vector<Ray>& res);
};

#endif /* defined(__RayTracer__RayIterator__) */
