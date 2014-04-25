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


KDTree::KDTree(const Mesh& mesh, int maxDepth)
  : mesh(mesh)
{
    buildRootNode(maxDepth);
}

void KDTree::buildRootNode(int maxDepth)
{
    vector<int> allTrianglesIndexes;
    for (int i = 0; i < mesh.getTriangles().size(); i++) {
        allTrianglesIndexes.push_back(i);
    }
    
    root = Node(allTrianglesIndexes, 0, *this);
    root.buildNode(*this, maxDepth);
}

#pragma mark - Node

KDTree::Node::Node()
  : left_node(NULL),
    right_node(NULL)
{
    
}

KDTree::Node::Node(vector<int>& _triangleIndexes, int _depth, const KDTree& tree)
  :  depth(_depth),
     triangleIndexes(_triangleIndexes),
     boundingBox(tree.mesh.getTriangles(), triangleIndexes, tree.mesh.getVertices()),
     plan(),
     left_node(NULL),
     right_node(NULL)
{
}

KDTree::Node::~Node()
{
    if (left_node)
        delete left_node;
    
    if (right_node)
        delete right_node;

}

KDTree::Node::Node(const Node& node)
  : depth(node.depth),
    triangleIndexes(node.triangleIndexes),
    boundingBox(node.boundingBox),
    plan(node.plan),
    left_node(NULL),
    right_node(NULL)
{
    
    if (node.left_node)
        left_node = new KDTree::Node(*node.left_node);
    
    if (node.right_node)
        right_node = new KDTree::Node(*node.right_node);
}


void KDTree::Node::buildNode(const KDTree& tree, int maxDepth)
{
    
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
    position = getMedianPoint(tree.mesh, n);
    
    plan.n = n;
    plan.position = position;
    
    if (depth == maxDepth) {
        return;
    }
    
    //seperate triangles
    
    vector<int> leftIndexes, rightIndexes;
    for (int idx : triangleIndexes){
        
        Triangle t = tree.mesh.getTriangles()[idx];
            
        Vec3Df a = tree.mesh.getVertices()[ t.getVertex(0) ].getPos() - position;
        Vec3Df b = tree.mesh.getVertices()[ t.getVertex(1) ].getPos() - position;
        Vec3Df c = tree.mesh.getVertices()[ t.getVertex(2) ].getPos() - position;
        
        bool aLeft = plan.isLeft(a);
        bool bLeft = plan.isLeft(b);
        bool cLeft = plan.isLeft(c);
        
        //regarder si tous les sommets sont du même côté du plan...
        if ( aLeft && bLeft && cLeft ){
            //Ils sont tous à gauche
            leftIndexes.push_back(idx);
            
        } else if( !aLeft && !bLeft && !cLeft){
            //Ils sont tous à droite
            rightIndexes.push_back(idx);
        } else {
            // Le triangle doit être ajouté des deux côtés
            rightIndexes.push_back(idx);
            leftIndexes.push_back(idx);
        }
        
    }
    
    if(rightIndexes == leftIndexes)
        return;
    
    if (triangleIndexes.size() == rightIndexes.size() || triangleIndexes.size() == leftIndexes.size())
        return;
    
    
    if(rightIndexes.size() != 0){
        right_node = new KDTree::Node(rightIndexes, depth + 1, tree);
        right_node->buildNode(tree, maxDepth);
    } 
    
    if(leftIndexes.size() != 0){
        left_node = new KDTree::Node(leftIndexes, depth + 1, tree);
        left_node->buildNode(tree, maxDepth);
    }
      
}

Vec3Df KDTree::Node::getMedianPoint(const Mesh& mesh, Vec3Df normal){
    
    vector<float> median_float;
    
    for (unsigned int i = 0; i< triangleIndexes.size(); i++){
        
        Triangle t = mesh.getTriangles()[triangleIndexes[i]];
        Vec3Df a = mesh.getVertices()[t.getVertex(0)].getPos();
        Vec3Df b = mesh.getVertices()[t.getVertex(1)].getPos();
        Vec3Df c = mesh.getVertices()[t.getVertex(2)].getPos();
        
        Vec3Df average = (a + b + c) / 3;
        
        median_float.push_back(Vec3Df::dotProduct(average, normal) );
    }
    
    std::vector<float> median_float_copy = median_float;
    
    std::nth_element(median_float_copy.begin(), median_float_copy.begin()+median_float_copy.size()/2, median_float_copy.end());
    
    float median = median_float_copy[median_float.size()/2];
    
    std::vector<float>::iterator it = std::find(median_float.begin(), median_float.end(), median);
    std::vector<float>::size_type pos = std::distance(median_float.begin(), it);
    
    Triangle t = mesh.getTriangles()[triangleIndexes[pos]];
    Vec3Df a = mesh.getVertices()[t.getVertex(0)].getPos();
    Vec3Df b = mesh.getVertices()[t.getVertex(1)].getPos();
    Vec3Df c = mesh.getVertices()[t.getVertex(2)].getPos();
    
    return (a + b + c) / 3;
    
}

bool KDTree::Plan::isLeft(Vec3Df point){
    
    return Vec3Df::dotProduct(n , point) <= 0;
    
}

#pragma mark - Ray Intersection

bool KDTree::Node::intersectRay(const Ray& ray, const KDTree& tree, float& intersectionDistance, Triangle& intersectionTriangle) const
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
        
        float minDistance = std::numeric_limits<float>::max();
        int minDistanceIndex = -1;
        
        for (int index : triangleIndexes) {
            bool intersection = ray.intersect(tree.mesh.getTriangles()[index], tree.mesh, position, barycentricCoordinates);
            float d = Vec3Df::distance(ray.getOrigin(), position);
            if (intersection && d < minDistance) {
                minDistance = d;
                minDistanceIndex = index;
            }
        }
        
        if(minDistanceIndex != -1)
        {
            intersectionDistance = minDistance;
            intersectionTriangle = tree.mesh.getTriangles()[minDistanceIndex];
            return true;
        } else {
            return false;
        }
    }

    
    // Otherwise, the two sons are present.
    float intersectionDistanceLeft, intersectionDistanceRight;
    intersectionDistanceLeft = intersectionDistanceRight = std::numeric_limits<float>::max();
    
    Triangle intersectionTriangleLeft, intersectiontriangleRight;
    
    // Recursive part
    bool intersectionLeft   = left_node != NULL && left_node->intersectRay(ray, tree, intersectionDistanceLeft, intersectionTriangleLeft);
    bool intersectionRight  = right_node != NULL && right_node->intersectRay(ray, tree, intersectionDistanceRight, intersectiontriangleRight);
    
    if (intersectionRight && !intersectionLeft) {
        intersectionDistance = intersectionDistanceRight;
        intersectionTriangle = intersectiontriangleRight;
    } else if (!intersectionRight && intersectionLeft) {
        intersectionDistance = intersectionDistanceLeft;
        intersectionTriangle = intersectionTriangleLeft;
    } else if(intersectionDistanceLeft < intersectionDistanceRight) {
        intersectionDistance = intersectionDistanceLeft;
        intersectionTriangle = intersectionTriangleLeft;
    } else {
        intersectionDistance = intersectionDistanceRight;
        intersectionTriangle = intersectiontriangleRight;
    }
    
    return intersectionLeft || intersectionRight;
}

bool KDTree::intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle) const
{
    return root.intersectRay(ray, *this, intersectionDistance, intersectionTriangle);
}