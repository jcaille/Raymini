// *********************************************************
// Scene Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Scene.h"

using namespace std;

static Scene * instance = NULL;

Scene * Scene::getInstance () {
    if (instance == NULL)
        instance = new Scene ();
    return instance;
}

void Scene::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

Scene::Scene () {
    buildDefaultScene ();
    updateBoundingBox ();
}

Scene::~Scene () {
}

void Scene::updateBoundingBox () {
    if (objects.empty ())
        bbox = BoundingBox ();
    else {
        bbox = objects[0].getBoundingBox ();
        for (unsigned int i = 1; i < objects.size (); i++){
            BoundingBox b = objects[i].getBoundingBox();
            b.applyTranslation(objects[i].getTrans());
            bbox.extendTo (b);
        }
    }
}

// Changer ce code pour creer des scenes originales
void Scene::buildDefaultScene () {
    Mesh groundMesh;
    groundMesh.loadOFF ("models/ground.off");
    Material groundMat;
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);
  
//    Mesh sphereMesh;
//    sphereMesh.loadOFF ("models/sphere.off");
//    Material sphereMat (1.f, 1.f, 5.0,Vec3Df (0.5, 0.5, 0.5));
//    Object sphere (sphereMesh, sphereMat);
//    sphere.setTrans (Vec3Df (-1.0f, 0.0f, 2.f));
//    objects.push_back (sphere);
    
//    Mesh sphere2Mesh;
//    sphere2Mesh.loadOFF ("models/sphere.off");
//    Material sphereMat2 (1.f, 1.f, 5.0,Vec3Df (0.5, 0.5, 0.5));
//    Object sphere2 (sphere2Mesh, sphereMat2);
//    sphere2.setTrans (Vec3Df (1.0f, 0.0f, 2.f));
//    objects.push_back (sphere2);
    
    Mesh ramMesh;
    ramMesh.loadOFF("models/ram.off");
    Material ramMat (1.f, 1.f, 5.0,Vec3Df (1.f, .6f, .2f));
    Object ram (ramMesh, ramMat);
    ram.setTrans (Vec3Df (1.f, 0.f, 0.f));
    objects.push_back (ram);
    
//    Mesh rhinoMesh;
//    rhinoMesh.loadOFF ("models/rhino.off");
//    Material rhinoMat (1.0f, 0.2f, 2.0, Vec3Df (0.6f, 0.6f, 0.7f));
//    Object rhino (rhinoMesh, rhinoMat);
//    rhino.setTrans (Vec3Df (-1.f, -1.0f, 0.4f));
//    objects.push_back (rhino);
//    Mesh gargMesh;
//    gargMesh.loadOFF ("models/gargoyle.off");
//    Material gargMat (0.7f, 0.4f, 11.0, Vec3Df (0.5f, 0.8f, 0.5f));
//    Object garg (gargMesh, gargMat);
//    garg.setTrans (Vec3Df (-1.f, 1.0f, 0.1f));
//    objects.push_back (garg);
    
    Light l1 (Vec3Df (0.0, 0.0, 10.0), Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
    Light l2 (Vec3Df (-5.0, 0.0, 5.0), Vec3Df (1.0f, 0.0f, 0.0f),0.7f);
    Light l3 (Vec3Df (5.0, 0.0, 5.0), Vec3Df (0.0f, 0.0f, 1.0f), 0.7f);

    lights.push_back (l1);
    lights.push_back(l2);
    lights.push_back(l3);
}
