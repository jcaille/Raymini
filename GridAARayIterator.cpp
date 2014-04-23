//
//  GridAARayIterator.cpp
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "GridAARayIterator.h"

GridAARayIterator::GridAARayIterator(Vec3Df cameraPosition, Vec3Df direction, Vec3Df upVector, Vec3Df rightVector, float fov, float ar, unsigned int screenWidth, unsigned int screenHeight)
:RayIterator(cameraPosition, direction, upVector, rightVector, fov, ar, screenWidth, screenHeight)
{
    
}

void GridAARayIterator::raysForPixel(int i, int j, std::vector<Ray>& res)
{
    res.clear();

    float tanX = tan (_fieldOfView)*_aspectRatio;
    float tanY = tan (_fieldOfView);
    
    for (int k = 0; k < gridSize; k++) {
        for(int l = 0; l < gridSize; l++)
        {
            Vec3Df stepX = (float (k+gridSize*i) - gridSize*_screenWidth/2.f)/(gridSize * _screenWidth) * tanX * _rightVector;
            Vec3Df stepY = (float (l+gridSize*j) - gridSize*_screenHeight/2.f)/(gridSize * _screenHeight) * tanY * _upVector;
            Vec3Df step = stepX + stepY;
            Vec3Df dir = _direction + step;
            dir.normalize ();
            res.push_back(Ray(_camPos, dir));

        }
    }
}