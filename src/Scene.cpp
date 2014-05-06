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
    _currentScene = CORNELL;
    buildCornellBoxScene();
    updateBoundingBox ();
}

void Scene::setCurrentScene(AvailableScene s)
{
    objects.clear();
    lights.clear();
    _pointsOfInterest.clear();
    switch (s) {
        case CORNELL:
            buildCornellBoxScene();
            break;
        case CHESS:
            buildChessScene();
            break;
        default:
            buildCornellBoxScene();
            break;
    }
    _currentScene = s;
    updateBoundingBox();
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

void Scene::resampleLights(float density)
{
    for (int i = 0; i < lights.size(); i++) {
        cout << "# of samples before : " << lights[i].getSamples().size() << endl;
        lights[i].resample(density);
        cout << "Light has been resampled with density of " << density << ": " << lights[i].getSamples().size() << " samples selected" << endl;
    }
}

#pragma mark - DEFAULT SCENE

#pragma mark - Primitives

void Scene::buildCornellBox(float scale){
    
    /* Do not modify */
    /* If you want to add objects/lights, modify buildDefaultScene */
    
    /* White, ground */
    Mesh groundMesh;
    groundMesh.makePlane(scale*Vec3Df(0,0,0), Vec3Df(0,0,1), Vec3Df(0,1,0), scale, scale);
    Material groundMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), 0.0);
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);

    /* Red, left wall */
    Mesh leftWallMesh;
    leftWallMesh.makePlane(scale*Vec3Df(-.5,0,.5), Vec3Df(1,0,0), Vec3Df(0,1,0), scale, scale);
    Material leftWallMat(.8f, .2f, 10, Vec3Df (1.f, 0.f, 0.f), 0.0);
    Object leftWall (leftWallMesh, leftWallMat);
    objects.push_back (leftWall);

    /* Green, right wall */
    Mesh rightWallMesh;
    rightWallMesh.makePlane(scale*Vec3Df(.5,0,.5), Vec3Df(-1,0,0), Vec3Df(0,1,0), scale, scale);
    Material rightWallMat(.8f, .2f, 10, Vec3Df (0.f, 1.f, 0.f), 0.0);
    Object rightWall (rightWallMesh, rightWallMat);
    objects.push_back (rightWall);

    /* White, back wall */
    Mesh backWallMesh;
    backWallMesh.makePlane(scale*Vec3Df(.0,.5,.5), Vec3Df(0,-1,0), Vec3Df(0,0,1), scale, scale);
    Material backtWallMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), 0.0);
    Object backWall (backWallMesh, backtWallMat);
    objects.push_back (backWall);
    
    /* White, ceiling */
    Mesh ceilingMesh;
    ceilingMesh.makePlane(scale*Vec3Df(0, 0, 1), Vec3Df(0,0,-1), Vec3Df(0,1,0), scale, scale);
    Material ceilingMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), 0.0);
    Object ceiling (ceilingMesh, ceilingMat);
    objects.push_back (ceiling);


    /* Ceiling light - slightly yellowish */
    Mesh ceilingLightMesh;
    ceilingLightMesh.makePlane(Vec3Df (0.0, 0.0, 0.0), Vec3Df(0,0,-1), Vec3Df(0,1,0), 0.2*scale, 0.2*scale);
    Light ceilingLight (scale*Vec3Df (0.0, 0.0, 0.99), ceilingLightMesh, Vec3Df (1.0f, .85f, .7f), 1.0f);
    lights.push_back(ceilingLight);

}

void Scene::buildCornellBoxScene() {
    
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
    Material tallBoxMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), .5);
    Object tallBox (tallBoxMesh, tallBoxMat);
    objects.push_back (tallBox);

    Vec3Df smallBoxFront(-.2,-1, 0);
    Vec3Df smallBoxUp(0,0,1);
    Vec3Df smallBoxCenter = scale * Vec3Df(.2, -.2, .15);
    Vec3Df smallBoxSize = scale * Vec3Df(.3,.3,.3);

    Mesh smallBoxMesh;
    smallBoxMesh.makeBox(smallBoxCenter, smallBoxFront, smallBoxUp, smallBoxSize);
    Material smallBoxMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), 0.0, 0.8, 1.6);
    Object smallBox (smallBoxMesh, smallBoxMat);
    objects.push_back (smallBox);

    
    // Top of the boxes, useful to put stuff on them
    Vec3Df tallBoxTop = tallBoxCenter + .5 * tallBoxSize[2] * tallBoxUp;
    Vec3Df smallBoxTop = smallBoxCenter + .5 * smallBoxSize[2] * smallBoxUp;
    
    
    
    /** Put objects in the scene here **/


    Mesh ramMesh;
    ramMesh.loadOFF("models/ram.off");
    // I AM RAM-GOD. I SHINE WITH THE POWER OF A THOUSANDS SUN. BOW BEFORE ME!
    Light ramLight (smallBoxTop, ramMesh, Vec3Df (1.0f, .85f, .7f), 1.0f);
    lights.push_back(ramLight);
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("God Ram", ramLight.getPos()));

    
    Mesh rhinoMesh;
    rhinoMesh.loadOFF("models/rhino.off");
    Material rhinoMat (1.0f, 0.2f, 2.0, Vec3Df (0.9f, 0.9f, 1.0f), 0.0);
    Object rhino (rhinoMesh, rhinoMat);
    rhino.setTrans (tallBoxTop + Vec3Df(0,0,.4));
    objects.push_back (rhino);

    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("Rhino", rhino.getTrans()));
}

#pragma mark - CHESS SCENE

void Scene::buildChessScene()
{
    
    float scale = .75;
    
    
    // Chessboard
    Material blackTileMat(.8f, .2f, 10, Vec3Df (.1, .1, .1), .3);
    Material whiteTileMat(.8f, .2f, 10, Vec3Df (.9f, .9f, .9f), .3);
    
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Mesh tileMesh;
            tileMesh.makeBox(scale*Vec3Df((i-4),(j-4),0), Vec3Df(0,1,0), Vec3Df(0,0,1), scale*Vec3Df(1,1,.1));
            Object tile(tileMesh, i%2 == j%2 ? blackTileMat : whiteTileMat);
            objects.push_back(tile);
        }
    }
    

    // Pieces
    Material blackPieceMat(.8f, .2f, 10, Vec3Df (.1, .1, .1), .1, .2, 1.5);
    Material whitePieceMat(.8f, .2f, 10, Vec3Df (1.f, 1.f, 1.f), .1, .5, 1.5);

    Mesh bishop;
    bishop.loadOFF("models/bishop.off");
    bishop.rotateAroundX(-M_PI_2);
    Vec3Df bishopCorrection(-.27,.3,0);

    Mesh queen;
    queen.loadOFF("models/queen.off");
    queen.rotateAroundX(-M_PI_2);
    Vec3Df queenCorrection(-.22,.25,0);

    Mesh pawn;
    pawn.loadOFF("models/pawn.off");
    pawn.rotateAroundX(-M_PI_2);
    Vec3Df pawnCorrection(-.3,.35,0);

    Mesh knight;
    knight.loadOFF("models/knight.off");
    knight.rotateAroundX(-M_PI_2);
    knight.rotateAroundZ(M_PI_2);
    Vec3Df knightCorrection(.32,.33,0);
    
    

    // White queen
    Object whiteQueen(queen, whitePieceMat);
    whiteQueen.setTrans(scale * Vec3Df(-2,-2,0) + queenCorrection);
//    objects.push_back(whiteQueen);

    // White pawn
    Object whitePawn(pawn, whitePieceMat);
    whitePawn.setTrans(scale * Vec3Df(-1,-1,0) + pawnCorrection);
//    objects.push_back(whitePawn);

    // White knight
    Object whiteKnight1(knight, whitePieceMat);
    whiteKnight1.setTrans(scale * Vec3Df(0,-2,0) + knightCorrection);
    objects.push_back(whiteKnight1);

    Object whiteKnight2(knight, whitePieceMat);
    whiteKnight2.setTrans(scale * Vec3Df(-1,-2,0) + knightCorrection);
    objects.push_back(whiteKnight2);

    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("White knights", scale*Vec3Df(0,-2,0)));
    
    // White bishop
    Object whiteBishop1(bishop, whitePieceMat);
    whiteBishop1.setTrans(scale * Vec3Df(0,-3,0) + bishopCorrection);
    objects.push_back(whiteBishop1);
    
    Object whiteBishop2(bishop, whitePieceMat);
    whiteBishop2.setTrans(scale * Vec3Df(-1,-3,0) + bishopCorrection);
    objects.push_back(whiteBishop2);
    
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("White bishops", scale*Vec3Df(0,-3,0)));
    
    // A row of black pawns
    for (int i = 0; i < 8; ++i)
    {
        Object blackPawn(pawn, blackPieceMat);
        blackPawn.setTrans(scale * Vec3Df(i-4,2+i%2,0) + pawnCorrection);
        objects.push_back(blackPawn);
    }
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("Black pawns", scale*Vec3Df(0,2,0)));
    
    
    Mesh ceilingLightMesh;
    ceilingLightMesh.makePlane(Vec3Df (0.0, 0.0, 0.0), Vec3Df(0,0,-1), Vec3Df(0,1,0), 8*scale, 8*scale);
    
    Light ceilingLight1 (scale*Vec3Df (4*scale, 4*scale, 10), ceilingLightMesh, Vec3Df (1.0f, 1.0f, 1.0f), .50f);
    lights.push_back(ceilingLight1);
    Light ceilingLight2 (scale*Vec3Df (4*scale, -4*scale, 10), ceilingLightMesh, Vec3Df (1.0f, 1.0f, 1.0f), .50f);
    lights.push_back(ceilingLight2);
    Light ceilingLight3 (scale*Vec3Df (-4*scale, 4*scale, 10), ceilingLightMesh, Vec3Df (1.0f, 1.0f, 1.0f), .50f);
    lights.push_back(ceilingLight3);
    Light ceilingLight4 (scale*Vec3Df (-4*scale, -4*scale, 10), ceilingLightMesh, Vec3Df (1.0f, 1.0f, 1.0f), .50f);
    lights.push_back(ceilingLight4);

    
}
