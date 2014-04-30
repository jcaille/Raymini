//
//  LensBlurIterator.h
//  RayTracer
//
//  Created by Jean Caillé on 30/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__LensBlurIterator__
#define __RayTracer__LensBlurIterator__

#include <iostream>
#include "RayIterator.h"

class LensBlurIterator : public RayIterator{
    public :
    
    float apertureRadius;
    float focalLength;
    
    std::vector<float> radiuses;
    std::vector<float> thetas;
    int samples;
    
    virtual void setCameraInformation(Vec3Df cameraPosition, Vec3Df direction, Vec3Df upVector, Vec3Df rightVector, float fov, float ar, unsigned int screenWidth, unsigned int screenHeight);
    
    virtual void raysForPixel(int i, int j, std::vector<Ray>& res);
};

#endif /* defined(__RayTracer__LensBlurIterator__) */
