//
//  GridAARayIterator.h
//  RayTracer
//
//  Created by Florian Denis on 23/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__GridAARayIterator__
#define __RayTracer__GridAARayIterator__

#include <iostream>
#include "RayIterator.h"

#endif /* defined(__RayTracer__GridAARayIterator__) */

class GridAARayIterator : public RayIterator
{
    public :
    int gridSize;
    GridAARayIterator(){};
    
    virtual void setCameraInformations(Vec3Df cameraPosition, Vec3Df direction, Vec3Df upVector, Vec3Df rightVector, float fov, float ar, unsigned int screenWidth, unsigned int screenHeight);
    virtual void raysForPixel(int i, int j, std::vector<Ray>& res);

    protected :
};