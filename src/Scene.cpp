// *********************************************************
// Scene Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Scene.h"


static inline int random_int(int lo, int hi)
{
    return lo + (rand() % (hi-lo+1));
}


static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(hi-lo)));
}

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
    _currentScene = CORNELLANIMAL;
    buildCornellBoxAnimalScene();
    updateBoundingBox ();
}

void Scene::setCurrentScene(AvailableScene s)
{
    objects.clear();
    lights.clear();
    _pointsOfInterest.clear();
    switch (s) {
        case CORNELLANIMAL:
            buildCornellBoxAnimalScene();
            break;
        case CORNELLSPHERE:
            buildCornellBoxSphereScene();
            break;
        case CHESS:
            buildChessScene();
            break;
        case SPHERES:
            buildSpheresScene();
            break;
        default:
            buildCornellBoxAnimalScene();
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
    Material groundMat(1.0f, 0.0f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);

    /* Red, left wall */
    Mesh leftWallMesh;
    leftWallMesh.makePlane(scale*Vec3Df(-.5,0,.5), Vec3Df(1,0,0), Vec3Df(0,1,0), scale, scale);
    Material leftWallMat(1.0f, 0.0f, 10, Vec3Df (1.f, 0.f, 0.f));
    Object leftWall (leftWallMesh, leftWallMat);
    objects.push_back (leftWall);

    /* Blue, right wall */
    Mesh rightWallMesh;
    rightWallMesh.makePlane(scale*Vec3Df(.5,0,.5), Vec3Df(-1,0,0), Vec3Df(0,1,0), scale, scale);
    Material rightWallMat(1.0f, 0.0f, 10, Vec3Df (0.5f, .7f, 1.0f));
    Object rightWall (rightWallMesh, rightWallMat);
    objects.push_back (rightWall);

    /* White, back wall */
    Mesh backWallMesh;
    backWallMesh.makePlane(scale*Vec3Df(.0,.5,.5), Vec3Df(0,-1,0), Vec3Df(0,0,1), scale, scale);
    Material backtWallMat(1.0f, 0.0f, 10, Vec3Df (1.f, 1.0f, 1.0f));
    Object backWall (backWallMesh, backtWallMat);
    objects.push_back (backWall);
    
    
    /* White, ceiling */
    Mesh ceilingMesh;
    ceilingMesh.makePlane(scale*Vec3Df(0, 0, 1), Vec3Df(0,0,-1), Vec3Df(0,1,0), scale, scale);
    Material ceilingMat(1.0f, 0.0f, 10, Vec3Df (1.f, 1.f, 1.f));
    Object ceiling (ceilingMesh, ceilingMat);
    objects.push_back (ceiling);


    /* Ceiling light - slightly yellowish */
    Mesh ceilingLightMesh;
    ceilingLightMesh.makePlane(Vec3Df (0.0, 0.0, 0.0), Vec3Df(0,0,-1), Vec3Df(0,1,0), 0.3*scale, 0.3*scale);
    Light ceilingLight (scale*Vec3Df (0.0, 0.0, 0.99), ceilingLightMesh, Vec3Df (1.0f, 1.0f, 1.0f), 0.8f);
    lights.push_back(ceilingLight);

}
void Scene::buildCornellBoxSphereScene() {
    
    float scale = 5;
    
    buildCornellBox(scale);
    
    Mesh sphereMesh;
    sphereMesh.loadOFF("models/sphere.off");
    sphereMesh.scale(.6);
    
    Material glassSphereMat(.0f, 1.0f, 128.0, Vec3Df (1.0f, 1.0f, 1.0f), 0.0, 1.0f, 1.7);
    Object glassSphere(sphereMesh, glassSphereMat);
    glassSphere.setTrans(scale*Vec3Df(.25,0,.18));
    objects.push_back(glassSphere);
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("Glass sphere", glassSphere.getTrans()));
    
    Material mirrorSphereMat(.0f, 1.0f, 128.0, Vec3Df (1.0f, 1.0f, 1.0f), 1.0, 0.0f, 1.7);
    Object mirrorSphere(sphereMesh, mirrorSphereMat);
    mirrorSphere.setTrans(scale*Vec3Df(-.25,0.15,.18));
    objects.push_back(mirrorSphere);
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("Mirror sphere", mirrorSphere.getTrans()));

    
}

void Scene::buildCornellBoxAnimalScene() {
    
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
    Material tallBoxMat(.8f, .2f, 10, Vec3Df (1.0f, 1.0f, 1.0f), .5);
    Object tallBox (tallBoxMesh, tallBoxMat);
    objects.push_back (tallBox);

    Vec3Df smallBoxFront(-.2,-1, 0);
    Vec3Df smallBoxUp(0,0,1);
    Vec3Df smallBoxCenter = scale * Vec3Df(.2, -.2, .15);
    Vec3Df smallBoxSize = scale * Vec3Df(.3,.3,.3);

    Mesh smallBoxMesh;
    smallBoxMesh.makeBox(smallBoxCenter, smallBoxFront, smallBoxUp, smallBoxSize);
    Material smallBoxMat(.8f, .2f, 10, Vec3Df (1.0f, 1.0f, 1.0f));
    Object smallBox (smallBoxMesh, smallBoxMat);
    objects.push_back (smallBox);

    
    // Top of the boxes, useful to put stuff on them
    Vec3Df tallBoxTop = tallBoxCenter + .5 * tallBoxSize[2] * tallBoxUp;
    Vec3Df smallBoxTop = smallBoxCenter + .5 * smallBoxSize[2] * smallBoxUp;
    
    
    
    /** Put objects in the scene here **/


//    Mesh ramMesh;
//    ramMesh.loadOFF("models/ram.off");
//    // I AM RAM-GOD. I SHINE WITH THE POWER OF A THOUSANDS SUN. BOW BEFORE ME!
//    Light ramLight (smallBoxTop, ramMesh, Vec3Df (1.0f, .85f, .7f), 1.0f);
//    lights.push_back(ramLight);
//    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("God Ram", ramLight.getPos()));

    Mesh ramMesh;
    ramMesh.loadOFF("models/ram.off");
    Material ramMat(1.0f, 0.6f, 2.0, Vec3Df (1.0f, 0.5f, 0.0f));
    Object ramObject(ramMesh, ramMat);
    ramObject.setTrans(smallBoxTop);
    objects.push_back(ramObject);
    _pointsOfInterest.push_back(std::pair<std::string, Vec3Df>("Ram", ramObject.getTrans()));
    
    Mesh rhinoMesh;
    rhinoMesh.loadOFF("models/rhino.off");
    rhinoMesh.rotateAroundZ(-M_PI/3);
    Material rhinoMat (1.0f, 0.2f, 2.0, Vec3Df (0.9f, 0.9f, 1.0f));
    Object rhino (rhinoMesh, rhinoMat);
    rhino.setTrans(tallBoxTop + Vec3Df(0,0,.4));
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
        _pointsOfInterest.push_back(std::pair<std::string, Vec3Df> ("Pawn_" + std::to_string(i), scale * Vec3Df(i-4,2+i%2,0) + pawnCorrection));
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

#pragma mark - Random Spheres

void Scene::buildSpheresScene()
{
    
    std::vector<Vec3Df> colors;
    
    colors.push_back(Vec3Df(0.7, 0.3, 0.2));
    colors.push_back(Vec3Df(0.2, 0.7, 0.3));
    colors.push_back(Vec3Df(0.3, 0.2, 0.7));

    colors.push_back(Vec3Df(0.7, 0.2, 0.3));
    colors.push_back(Vec3Df(0.3, 0.7, 0.2));
    colors.push_back(Vec3Df(0.2, 0.3, 0.7));

    colors.push_back(Vec3Df(0.7, 0.6, 0.4));
    colors.push_back(Vec3Df(0.6, 0.4, 0.7));
    colors.push_back(Vec3Df(0.4, 0.7, 0.6));

    colors.push_back(Vec3Df(0.7, 0.4, 0.6));
    colors.push_back(Vec3Df(0.4, 0.6, 0.7));
    colors.push_back(Vec3Df(0.6, 0.7, 0.4));


    std::vector<Material> materials;
    for(int i = 0 ; i < colors.size(); i++)
    {
        materials.push_back(Material(1.0, 0.0, 0.4, colors[i], 0.1));
        materials.push_back(Material(1.0, 0.2, 0.4, colors[i], 0.1));
        materials.push_back(Material(1.0, 0.0, 0.8, colors[i], 0.3));
        materials.push_back(Material(1.0, 0.1, 0.4, colors[i], 0.1));
    }
    

    const int numDifferentScales = 10;
    std::vector<Mesh> scales;
    
    Mesh mesh;
    mesh.loadOFF("models/sphere.off");

    
    for (int i = 0; i < numDifferentScales; ++i){
        Mesh scaledMesh(mesh);
        scaledMesh.scale(random_float(0.15, 1.5));
        
        scales.push_back(scaledMesh);
    }
    

    for (int i = 0; i < 90; ++i)
    {

        float x = random_float(-10, 10);
        float y = random_float(-10, 10);
        float z = random_float(-10, 10);

        int matIdx = random_int(0, (int)materials.size()-1);
        int scaleIdx = random_int(0, numDifferentScales-1);
        
        Object o(scales[scaleIdx], materials[matIdx]);
        o.setTrans(Vec3Df(x, y, z));
        objects.push_back(o);
    }
    
    Light l0(Vec3Df(0, 0, 0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l0);
    
    Light l1(Vec3Df(20.0, 0.0, 0.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l1);
    
    Light l2(Vec3Df(-20.0, 0.0, 0.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l2);
    
    Light l3(Vec3Df(0.0, 20.0, 0.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l3);
    
    Light l4(Vec3Df(0.0, -20.0, 0.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l4);
    
    Light l5(Vec3Df(0.0, 0.0, -20.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l5);
    
    Light l6(Vec3Df(0.0, 0.0, 20.0), Vec3Df(1.0, 1.0, 1.0), 1.0);
    lights.push_back(l6);   
}
