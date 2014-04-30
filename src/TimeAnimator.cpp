//
//  TimeAnimator.cpp
//  RayTracer
//
//  Created by Jean Caillé on 30/04/2014.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "TimeAnimator.h"
#include "Scene.h"
#include "Object.h"
#include "Light.h"
#include "Geometry.h"

void TimeAnimator::applyTransformationAtTime(float time)
{
    Scene* scene = Scene::getInstance();
    
    std::vector<Object> objects = scene->getObjects();
    for (int i = 0; i < objects.size(); i++) {
        Movement m = objectsMovement[i];
        objects[i].setTrans(m.positionAtTime(time));
    }
    
    std::vector<Light> lights = scene->getLights();
    for(int i = 0 ; i < lights.size(); i++){
        Movement m = lightsMovement[i];
        lights[i].setPos(m.positionAtTime(time));
        lights[i].resample(lights[i].getDensity());
    }
}