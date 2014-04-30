//
//  LensBlurIterator.cpp
//  RayTracer
//
//  Created by Jean Caillé on 30/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "LensBlurIterator.h"
#include "Scene.h"

float random_float(float lo, float hi)
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
    
    // Focus on Rhino
    Scene* scene = Scene::getInstance();
    Vec3Df rhinoCenter = scene->getObjects()[scene->getObjects().size() -1].getTrans();
    
    apertureRadius = 0.000001;
    focalLength = Vec3Df::distance(_camPos, rhinoCenter);
    samples = 1;
    
    for (int i = 0; i < samples; i++) {
        radiuses.push_back(random_float(0, apertureRadius));
        thetas.push_back(random_float(0, M_PI * 2));
    }
    
}

void LensBlurIterator::raysForPixel(int i, int j, std::vector<Ray>& res){
    float tanX = tan (_fieldOfView)*_aspectRatio;
    float tanY = tan (_fieldOfView);
    Vec3Df stepX = ((float) i - _screenWidth/2.f)/(_screenWidth) * tanX * _rightVector;
    Vec3Df stepY = ((float) j - _screenHeight/2.f)/(_screenHeight) * tanY * _upVector;
    Vec3Df step = stepX + stepY;
    
    Vec3Df dir = _direction + step;
    dir.normalize ();
    
    Ray originRay = Ray(_camPos, dir);
    Vec3Df focalPoint = _camPos + focalLength * dir;
    res.clear();
    res.push_back(originRay);
    

    for(int i = 0; i < samples ; i++)
    {
        float theta = thetas[i];
        float r = radiuses[i];
        
        Vec3Df origin = _camPos + r * _upVector * cos(theta) + r * _rightVector * sin(theta);
        Vec3Df rayDir = focalPoint - origin;
        rayDir.normalize();
        res.push_back(Ray(origin, rayDir));
    }
    
    
    res.push_back(Ray(_camPos, dir));

}

