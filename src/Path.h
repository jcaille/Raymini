//
//  Path.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__Path__
#define __RayTracer__Path__

#include <iostream>
#include "Ray.h"
#include <vector>

class Path{
    
public :
    inline Path(){ depth = 0; contains_light = false;}
    void addRay(Ray ray);
    inline std::vector<Ray> getRays(){ return rays;}
    inline void setContainsLight(bool _light){ contains_light = _light;}
    inline bool getContainsLight(){ return contains_light; }
    
private :
    std::vector<Ray> rays; //contient ainsi les points intersecté et les directions d'émissions
    int depth; // profondeur du path
    bool contains_light;
    
    
};

#endif /* defined(__RayTracer__Path__) */
