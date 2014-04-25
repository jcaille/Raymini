//
//  KDTree.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "KDTree.h"
#include "BoundingBox.h"
#include <algorithm>

#pragma mark - KDTree

KDTree::KDTree(){}

KDTree::KDTree( const vector<Triangle> & T, const vector<Vertex> & V ){
    
    triangles = T;
    vertices = V;
    root = NULL;
    
}

KDTree::KDTree(const Mesh& mesh)
  : KDTree(mesh.getTriangles(),mesh.getVertices())
{
}

void KDTree::buildRootNode(int maxDepth)
{
    vector<int> allTrianglesIndexes;
    for (int i = 0; i < triangles.size(); i++) {
        allTrianglesIndexes.push_back(i);
    }
    
    root = new KDTree::Node(allTrianglesIndexes, 0, this);
    root->buildNode(maxDepth);
}

#pragma mark - Node

KDTree::Node::Node(vector<int>& _triangleIndexes, int _depth, KDTree* _tree)
{
    depth = _depth;
    triangleIndexes = _triangleIndexes;
    tree = _tree;
    boundingBox = BoundingBox(tree->triangles, triangleIndexes, tree->vertices);
    
    // Those are not initialized and must be build by explicite:y calling buildNode;
    plan = KDTree::Plan();
    left_node = NULL;
    right_node = NULL;
}

void KDTree::Node::buildNode(int maxDepth)
{
    if (depth == maxDepth) {
        return;
    }
    
    // Find the median plan
    Vec3Df n;
    if (boundingBox.getSize() == boundingBox.getWidth() ){
        n = Vec3Df (boundingBox.getMax()[0] - boundingBox.getMin()[0], 0.0, 0.0);
    }else if (boundingBox.getSize() == boundingBox.getHeight()){
        n = Vec3Df (0.0, boundingBox.getMax()[1] - boundingBox.getMin()[1], 0.0);
    }else{
        n = Vec3Df( 0.0, 0.0, boundingBox.getMax()[2] - boundingBox.getMin()[2]);
    }
    
    n.normalize();
    Vec3Df position;
    //il faut trouver un triangle médian, pour l'instant on prend le milieu de l'axe de la bounding box
    //position = boundingBox.getCenter();
    position = getMedianPoint(n);
    
    plan.n = n;
    plan.position = position;
    
    //seperate triangles
    
    vector<int> leftIndexes, rightIndexes;
    for (unsigned int i=0; i<triangleIndexes.size(); i++){
        Triangle t = tree->triangles[triangleIndexes[i]];
            
        Vec3Df a = tree->vertices[ t.getVertex(0) ].getPos() - position;
        Vec3Df b = tree->vertices[ t.getVertex(1) ].getPos() - position;
        Vec3Df c = tree->vertices[ t.getVertex(2) ].getPos() - position;
        
        bool aLeft = plan.isLeft(a);
        bool bLeft = plan.isLeft(b);
        bool cLeft = plan.isLeft(c);
                
        //regarder si tous les sommets sont du même côté du plan...
        if ( (aLeft && bLeft && cLeft) ){
            //Ils sont tous à gauche
            leftIndexes.push_back(triangleIndexes[i]);
        }else if( !aLeft && !bLeft && !cLeft){
            //Ils sont tous à droite
            rightIndexes.push_back(triangleIndexes[i]);
        } else {
            // Le triangle doit être ajouté des deux côtés
            rightIndexes.push_back(triangleIndexes[i]);
            leftIndexes.push_back(triangleIndexes[i]);
        }
    }
    
    if(rightIndexes == leftIndexes)
        return;
    
    if(rightIndexes.size() != 0 && triangleIndexes.size() != rightIndexes.size()){
        right_node = new KDTree::Node(rightIndexes, depth + 1, tree);
        right_node->buildNode(maxDepth);
    }
    
    if(leftIndexes.size() != 0 && triangleIndexes.size() != leftIndexes.size() ){
        left_node = new KDTree::Node(leftIndexes, depth + 1, tree);
        left_node->buildNode(maxDepth);
    }
    
    
}

Vec3Df KDTree::Node::getMedianPoint(Vec3Df normal){
    
    vector<float> median_float;
    
    for (unsigned int i =0; i< triangleIndexes.size(); i++){
        Vec3Df average = ( tree->vertices [ tree->triangles[triangleIndexes[i]].getVertex(0) ].getPos()
                         + tree->vertices [ tree->triangles[triangleIndexes[i]].getVertex(1) ].getPos()
                         + tree->vertices [ tree->triangles[triangleIndexes[i]].getVertex(2) ].getPos())/3;
        
        median_float.push_back(Vec3Df::dotProduct(average, normal) );
    }
    
    std::vector<float> median_float_copy = median_float;
    
    std::nth_element(median_float_copy.begin(), median_float_copy.begin()+median_float_copy.size()/2, median_float_copy.end());
    
    float median = median_float_copy[median_float.size()/2];
    
    std::vector<float>::iterator it = std::find(median_float.begin(), median_float.end(), median);
    std::vector<float>::size_type pos = std::distance(median_float.begin(), it);
    
    //    std::sort(median_float.begin(), median_float.end());
    
    return (tree->vertices[ tree->triangles[triangleIndexes[pos]].getVertex(0) ].getPos() + tree->vertices [ tree->triangles[triangleIndexes[pos]].getVertex(1) ].getPos() + tree->vertices [ tree->triangles[triangleIndexes[pos]].getVertex(2) ].getPos())/3;
    
}

bool KDTree::Plan::isLeft(Vec3Df point){
    
    return Vec3Df::dotProduct(n , point) <= 0;
    
}