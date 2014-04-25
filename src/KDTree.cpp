//
//  KDTree.cpp
//  RayTracer
//
//  Created by Audrey FOURNERET on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "KDTree.h"
#include "BoundingBox.h"
#include "Ray.h"

#include <algorithm>

#pragma mark - KDTree


KDTree::KDTree(const Mesh& mesh)
  : mesh(mesh)
{
}

void KDTree::buildRootNode(int maxDepth)
{
    vector<int> allTrianglesIndexes;
    for (int i = 0; i < mesh.getTriangles().size(); i++) {
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
    boundingBox = BoundingBox(tree->mesh.getTriangles(), triangleIndexes, tree->mesh.getVertices());
    
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
        Triangle t = tree->mesh.getTriangles()[triangleIndexes[i]];
            
        Vec3Df a = tree->mesh.getVertices()[ t.getVertex(0) ].getPos() - position;
        Vec3Df b = tree->mesh.getVertices()[ t.getVertex(1) ].getPos() - position;
        Vec3Df c = tree->mesh.getVertices()[ t.getVertex(2) ].getPos() - position;
        
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
        
        Triangle t = tree->mesh.getTriangles()[triangleIndexes[i]];
        Vec3Df a = tree->mesh.getVertices()[t.getVertex(0)].getPos();
        Vec3Df b = tree->mesh.getVertices()[t.getVertex(1)].getPos();
        Vec3Df c = tree->mesh.getVertices()[t.getVertex(2)].getPos();
        
        Vec3Df average = (a + b + c) / 3;
        
        median_float.push_back(Vec3Df::dotProduct(average, normal) );
    }
    
    std::vector<float> median_float_copy = median_float;
    
    std::nth_element(median_float_copy.begin(), median_float_copy.begin()+median_float_copy.size()/2, median_float_copy.end());
    
    float median = median_float_copy[median_float.size()/2];
    
    std::vector<float>::iterator it = std::find(median_float.begin(), median_float.end(), median);
    std::vector<float>::size_type pos = std::distance(median_float.begin(), it);
    
    Triangle t = tree->mesh.getTriangles()[triangleIndexes[pos]];
    Vec3Df a = tree->mesh.getVertices()[t.getVertex(0)].getPos();
    Vec3Df b = tree->mesh.getVertices()[t.getVertex(1)].getPos();
    Vec3Df c = tree->mesh.getVertices()[t.getVertex(2)].getPos();
    
    return (a + b + c) / 3;
    
}

bool KDTree::Plan::isLeft(Vec3Df point){
    
    return Vec3Df::dotProduct(n , point) <= 0;
    
}

#pragma mark - Ray Intersection

bool KDTree::Node::intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle)
{
    // Check if ray intersect bouding box
    Vec3Df position;
    if(!ray.intersect(boundingBox, position))
    {
        return false;
    }
    
    // If there are no sons, check with node triangles
    vector<float> barycentricCoordinates;
    if(left_node == NULL && right_node == NULL){
        float minDistance = __FLT_MAX__;
        int minDistanceIndex = -1;
        
        for (int i = 0; i < triangleIndexes.size(); i++) {
            bool intersection = ray.intersect(tree->mesh.getTriangles()[triangleIndexes[i]], tree->mesh, position, barycentricCoordinates);
            if (intersection && Vec3Df::distance(ray.getOrigin(), position) < minDistance) {
                minDistance = Vec3Df::distance(ray.getOrigin(), position);
                minDistanceIndex = i;
            }
        }
        if(minDistanceIndex != -1)
        {
            intersectionDistance = minDistance;
            intersectionTriangle = tree->mesh.getTriangles()[triangleIndexes[minDistanceIndex]];
            return true;
        } else {
            return false;
        }
    }

    
    // Otherwise, the two sons are present.
    float intersectionDistanceLeft, intersectionDistanceRight;
    Triangle intersectionTriangleLeft, intersectiontriangleRight;
    
    // Recursive part
    bool intersectionRight = left_node->intersectRay(ray, intersectionDistanceLeft, intersectionTriangleLeft);
    bool intersectionLeft = right_node->intersectRay(ray, intersectionDistanceRight, intersectiontriangleRight);
    
    if (!intersectionRight && ! intersectionLeft) {
        return false;
    } else if (intersectionRight && !intersectionLeft) {
        intersectionDistance = intersectionDistanceRight;
        intersectionTriangle = intersectiontriangleRight;
        return true;
    } else if (!intersectionRight && intersectionLeft) {
        intersectionDistance = intersectionDistanceLeft;
        intersectionTriangle = intersectionTriangleLeft;
        return true;
    } else {
        // Ray intersect in both sons, compare distances
        if(intersectionDistanceLeft < intersectionDistanceRight)
        {
            intersectionDistance = intersectionDistanceLeft;
            intersectionTriangle = intersectionTriangleLeft;
        } else {
            intersectionDistance = intersectionDistanceRight;
            intersectionTriangle = intersectiontriangleRight;
        }
        return true;
    }
}

bool KDTree::intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle)
{
    return root->intersectRay(ray, intersectionDistance, intersectionTriangle);
}