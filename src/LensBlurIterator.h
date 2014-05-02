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
#include "GridAARayIterator.h"
#include "Geometry.h"

class LensBlurIterator : public GridAARayIterator{
    public :

    inline void resample(){
        _samples.clear();

        if(_focalLength < 0){
            std::cout << "Aperture has been resampled : " << _samples.size() << std::endl;
            return;
        }

        _geometry.sample(_density, _samples);
        std::cout << "Aperture has been resampled : " << _samples.size() << std::endl;
    }

    inline float getFocalLength(){return _focalLength;}
    
    inline void focusOn(const Geometry& object){
        focusOn(object.getTrans());
    }
    
    inline void focusOn(const Vec3Df& point){
        _focalLength = Vec3Df::distance(_camPos, point);
    }
    inline void setFocalLength(const float f){_focalLength = f;}
    
    inline Geometry getGeometry() const{return _geometry;}
    inline void setGeometry(const Geometry g){
        _geometry = g;
        resample();
    }
    
    inline float getDensity() const {return _density;}
    inline void setDensity(const float d)
    {
        _density = d;
        resample();
    }
    
    virtual void setCameraInformation(Vec3Df cameraPosition, Vec3Df direction, Vec3Df upVector, Vec3Df rightVector, float fov, float ar, unsigned int screenWidth, unsigned int screenHeight);
    
    virtual void raysForPixel(int i, int j, std::vector<Ray>& res);

private:
    Geometry _geometry;
    float _density;
    
    // Negative focal length means pinhole camera.
    float _focalLength;
    
    std::vector<Vec3Df> _samples;
    
    friend RayIterator;
};

#endif /* defined(__RayTracer__LensBlurIterator__) */
