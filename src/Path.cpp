//
//  Path.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "Path.h"

void Path::addRay(Ray ray){
    
    rays.push_back(ray);
    depth++;
    
}