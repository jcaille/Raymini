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

void Scene::buildCornellBox(float scale){
    
    /* Do not modify */
    /* If you want to add objects/lights, modify buildDefaultScene */
    
    /* White, ground */
    Mesh groundMesh;
    groundMesh.makePlane(scale*Vec3Df(0,0,0), Vec3Df(0,0,1), Vec3Df(0,1,0), scale, scale);
    Material groundMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);

    /* Red, left wall */
    Mesh leftWallMesh;
    leftWallMesh.makePlane(scale*Vec3Df(-.5,0,.5), Vec3Df(1,0,0), Vec3Df(0,1,0), scale, scale);
    Material leftWallMat(.8f, .2f, 10, Vec3Df (1.f, 0.f, 0.f));
    Object leftWall (leftWallMesh, leftWallMat);
    objects.push_back (leftWall);

    /* Green, right wall */
    Mesh rightWallMesh;
    rightWallMesh.makePlane(scale*Vec3Df(.5,0,.5), Vec3Df(-1,0,0), Vec3Df(0,1,0), scale, scale);
    Material rightWallMat(.8f, .2f, 10, Vec3Df (0.f, 1.f, 0.f));
    Object rightWall (rightWallMesh, rightWallMat);
    objects.push_back (rightWall);

    /* White, back wall */
    Mesh backWallMesh;
    backWallMesh.makePlane(scale*Vec3Df(.0,.5,.5), Vec3Df(0,-1,0), Vec3Df(0,0,1), scale, scale);
    Material backtWallMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object backWall (backWallMesh, backtWallMat);
    objects.push_back (backWall);

    /* White, ceiling */
    Mesh ceilingMesh;
    ceilingMesh.makePlane(scale*Vec3Df(0, 0, 1), Vec3Df(0,0,-1), Vec3Df(0,1,0), scale, scale);
    Material ceilingMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object ceiling (ceilingMesh, ceilingMat);
    objects.push_back (ceiling);


    /* Ceiling light - slightly yellowish */
    Light l (scale*Vec3Df (0.0, 0.0, 0.99), Vec3Df (1.0f, .85f, .7f), 1.0f);
    lights.push_back(l);
}


void Scene::buildDefaultScene () {
    
    float scale = 5;
    
    buildCornellBox(scale);
    
    // The 2 usual boxes in the cornell box
    // I recommend leaving them here and put objects on top of them, see bellow
    
    Vec3Df tallBoxFront(.2,-1,0);
    Vec3Df tallBoxUp(0,0,1);
    Vec3Df tallBoxCenter = scale * Vec3Df(-.2, .2, .3);
    Vec3Df tallBoxSize = scale * Vec3Df(.3,.3, .6);
    
    Mesh tallBoxMesh;
    tallBoxMesh.makeBox(tallBoxCenter, tallBoxFront, tallBoxUp, tallBoxSize);
    Material tallBoxMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object tallBox (tallBoxMesh, tallBoxMat);
    objects.push_back (tallBox);

    Vec3Df smallBoxFront(-.2,-1, 0);
    Vec3Df smallBoxUp(0,0,1);
    Vec3Df smallBoxCenter = scale * Vec3Df(.2, -.2, .15);
    Vec3Df smallBoxSize = scale * Vec3Df(.3,.3,.3);

    Mesh smallBoxMesh;
    smallBoxMesh.makeBox(smallBoxCenter, smallBoxFront, smallBoxUp, smallBoxSize);
    Material smallBoxMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object smallBox (smallBoxMesh, smallBoxMat);
    objects.push_back (smallBox);

    
    // Top of the boxes, useful to put stuff on them
    Vec3Df tallBoxTop = tallBoxCenter + .5 * tallBoxSize[2] * tallBoxUp;
    Vec3Df smallBoxTop = smallBoxCenter + .5 * smallBoxSize[2] * smallBoxUp;
    
    
    
    /** Put objects in the scene here **/
    
    Mesh ramMesh;
    ramMesh.loadOFF("models/ram.off");
    Material ramMat (1.f, 1.f, 5.0,Vec3Df (1.f, .6f, .2f));
    Object ram (ramMesh, ramMat);
    ram.setTrans (smallBoxTop);
    objects.push_back (ram);
    
    Mesh rhinoMesh;
    rhinoMesh.loadOFF ("models/rhino.off");
    Material rhinoMat (1.0f, 0.2f, 2.0, Vec3Df (0.6f, 0.6f, 0.7f));
    Object rhino (rhinoMesh, rhinoMat);
    rhino.setTrans (tallBoxTop + Vec3Df(0,0,.4));
    objects.push_back (rhino);
    

    
    
    
//    Mesh groundMesh;
//    groundMesh.loadOFF ("models/ground.off");
//    Material groundMat;
//    Object ground (groundMesh, groundMat);
//    objects.push_back (ground);

//    Mesh sphereMesh;
//    sphereMesh.loadOFF ("models/sphere.off");
//    Material sphereMat (1.f, 1.f, 9.0, Vec3Df (0.5, 0.5, 0.5));
//    Object sphere (sphereMesh, sphereMat);
//    sphere.setTrans (Vec3Df (-1.0f, 0.0f, 0.5f));
//    objects.push_back (sphere);
    
//    Mesh sphere2Mesh;
//    sphere2Mesh.loadOFF ("models/sphere.off");
//    Material sphereMat2 (1.f, 1.f, 5.0,Vec3Df (0.5, 0.5, 0.5));
//    Object sphere2 (sphere2Mesh, sphereMat2);
//    sphere2.setTrans (Vec3Df (1.0f, 0.0f, 2.f));
//    objects.push_back (sphere2);
    
//    Mesh gargMesh;
//    gargMesh.loadOFF ("models/gargoyle.off");
//    Material gargMat (0.7f, 0.4f, 11.0, Vec3Df (0.5f, 0.8f, 0.5f));
//    Object garg (gargMesh, gargMat);
//    garg.setTrans (Vec3Df (-1.f, 1.0f, 0.1f));
//    objects.push_back (garg);
    
//    Light l1 (Vec3Df (0.0, 0.0, 10.0), Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
//    Light l2 (Vec3Df (10.0, 10.0, 23.0), Vec3Df (0.0f, 1.0f, 0.0f), 1.0f);
//    Light l3 (Vec3Df (-10, -10.0, 23.0), Vec3Df (0.0f, 0.0f, 1.0f), 1.0f);
//    Light l4 (Vec3Df (0.0, -10.0, 5.0), Vec3Df (1.0f, 1.0f, 1.0f), 1.0f);
//    
//    lights.push_back (l1);
//    lights.push_back(l2);
//    lights.push_back(l3);
//    lights.push_back(l4);
}
