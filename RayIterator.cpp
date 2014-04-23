//
//  RayIterator.cpp
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "RayIterator.h"

RayIterator::RayIterator(Vec3Df cameraPosition,
                         Vec3Df direction,
                         Vec3Df upVector,
                         Vec3Df rightVector,
                         float fov,
                         float ar,
                         unsigned int screenWidth,
                         unsigned int screenHeight)
{
    _camPos = cameraPosition;
    _direction = direction;
    _upVector = upVector;
    _rightVector = rightVector;
    _fieldOfView = fov;
    _aspectRatio = ar;
    _screenHeight = screenHeight;
    _screenWidth = screenWidth;
}

void RayIterator::raysForPixel(int i, int j, std::vector<Ray>& res)
{
    float tanX = tan (_fieldOfView)*_aspectRatio;
    float tanY = tan (_fieldOfView);
    Vec3Df stepX = ((float) i - _screenWidth/2.f)/(_screenWidth) * tanX * _rightVector;
    Vec3Df stepY = ((float) j - _screenHeight/2.f)/(_screenHeight) * tanY * _upVector;
    Vec3Df step = stepX + stepY;
    
    Vec3Df dir = _direction + step;
    dir.normalize ();

    res.clear();
    res.push_back(Ray(_camPos, dir));
}
