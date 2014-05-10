// *********************************************************
// Scene Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <string>

#include "Object.h"
#include "Light.h"
#include "BoundingBox.h"

typedef enum AvailableScene {
    CORNELLANIMAL,
    CORNELLSPHERE,
    CHESS,
    SPHERES,
} AvailableScene;

class Scene {
public:
    static Scene * getInstance ();
    static void destroyInstance ();
    
    void setCurrentScene(AvailableScene s);
    inline AvailableScene getCurrentScene(){return _currentScene;}
        
    inline std::vector<Object> & getObjects () { return objects; }
    inline const std::vector<Object> & getObjects () const { return objects; }
    
    inline std::vector<Light> & getLights () { return lights; }
    inline const std::vector<Light> & getLights () const { return lights; }
    
    inline std::vector< std::pair<std::string, Vec3Df> > getPOI()
    {
        return _pointsOfInterest;
    }
    
    inline const BoundingBox & getBoundingBox () const { return bbox; }
    void updateBoundingBox ();
    void resampleLights(float density);
    
protected:
    Scene ();
    virtual ~Scene ();
    
private:
    
    AvailableScene _currentScene;
        
    void buildCornellBox(float scale = 1);
    void buildCornellBoxAnimalScene();
    void buildCornellBoxSphereScene();
    
    void buildChessScene();
    void buildSpheresScene();
    
    std::vector<Object> objects;
    std::vector<Light> lights;
    BoundingBox bbox;
    
    std::vector< std::pair<std::string, Vec3Df> > _pointsOfInterest;
};


#endif // SCENE_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
