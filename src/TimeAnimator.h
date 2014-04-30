//
//  TimeAnimator.h
//  RayTracer
//
//  Created by Jean Caillé on 30/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__TimeAnimator__
#define __RayTracer__TimeAnimator__

#include <iostream>
#include <vector>

#include "Vec3D.h"

struct Movement
{
    Vec3Df startPosition;
    Vec3Df endPosition;
    
    float startTime;
    float endTime;
    
    inline Vec3Df positionAtTime(float time)
    {
        if(time < startTime)
            return startPosition;
        if(time > endTime)
            return endPosition;
        return startPosition + (time - startTime) * (endPosition - startPosition) / (endTime - startTime);
    }
};

class TimeAnimator
{
public:
    TimeAnimator();
    
    /**
     *  Modifies the shared scene instance.
     *
     *  @param time The time at which the scene is displayed.
     */
    void applyTransformationAtTime(float time);
    
    float startTime;
    float endTime;
    
    std::vector<Movement> objectsMovement;
    std::vector<Movement> lightsMovement;
};

#endif /* defined(__RayTracer__TimeAnimator__) */
