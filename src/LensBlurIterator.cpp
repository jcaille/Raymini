//
//  LensBlurIterator.cpp
//  RayTracer
//
//  Created by Jean Caillé on 30/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "LensBlurIterator.h"
#include "Scene.h"

static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(hi-lo)));
}

void LensBlurIterator::setCameraInformation(Vec3Df cameraPosition,
                                       Vec3Df direction,
                                       Vec3Df upVector,
                                       Vec3Df rightVector,
                                       float fov,
                                       float ar,
                                       unsigned int screenWidth,
                                       unsigned int screenHeight)
{
    std::cout << cameraPosition << std::endl;
    _camPos = cameraPosition;
    _direction = direction;
    _upVector = upVector;
    _rightVector = rightVector;
    _fieldOfView = fov;
    _aspectRatio = ar;
    _screenHeight = screenHeight;
    _screenWidth = screenWidth;
}

void LensBlurIterator::raysForPixel(int i, int j, std::vector<Ray>& res){
    
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
            
            // At least one center ray;
            res.push_back(Ray(_camPos, dir));
            
            Vec3Df focalPoint = _camPos + _focalLength * dir;
            for(int p = 0; p < _samples.size() ; p++)
            {
                Vec3Df s = _samples[p];
                Vec3Df origin = _camPos + _upVector * s[0] + _rightVector * s[1];
                Vec3Df rayDir = focalPoint - origin;
                rayDir.normalize();
                res.push_back(Ray(origin, rayDir));
            }

        }
    }

}

