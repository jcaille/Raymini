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

static inline int32_t intlog2(size_t n)
{
    if (n==0) return -1;
    
    int32_t logValue = -1;
    
    while (n) {
        logValue++;
        n >>= 1;
    }
    
    return logValue;
}

KDTree::KDTree(const Mesh& mesh, int maxDepth)
 :  _mesh(mesh)
{
    if (maxDepth < 0){
        
        // if no maxdepth specified, let's make it so that ~ 50 items are in the leaves
        maxDepth = 1+intlog2(mesh.getTriangles().size()/50);
    }
    
    buildRootNode(maxDepth);
}

void KDTree::buildRootNode(int maxDepth)
{
    vector<int> allTrianglesIndexes;
    for (int i = 0; i < _mesh.getTriangles().size(); i++) {
        allTrianglesIndexes.push_back(i);
    }
    
    _root = Node(allTrianglesIndexes, 0, *this);
    _root.buildNode(*this, maxDepth);
}

#pragma mark - Node

KDTree::Node::Node()
 :  left_node(NULL),
    right_node(NULL)
{
}

KDTree::Node::Node(const vector<int>& _triangleIndexes, int _depth, const KDTree& tree)
 :  depth(_depth),
    triangleIndexes(_triangleIndexes),
    boundingBox(tree.getMesh().getTriangles(), triangleIndexes, tree.getMesh().getVertices()),
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
 :  depth(node.depth),
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

KDTree::Node& KDTree::Node::operator= (const Node& node)
{
    depth = node.depth;
    triangleIndexes = node.triangleIndexes;
    boundingBox = node.boundingBox;
    plan = node.plan;
    left_node = NULL;
    right_node = NULL;

    if (node.left_node)
        left_node = new KDTree::Node(*node.left_node);
    
    if (node.right_node)
        right_node = new KDTree::Node(*node.right_node);
    
    return *this;
}

void KDTree::Node::buildNode(const KDTree& tree, int maxDepth)
{
    
    // Find the median plan
    Vec3Df n;
    if (boundingBox.getSize() == boundingBox.getWidth() ){
        n = Vec3Df (boundingBox.getMax()[0] - boundingBox.getMin()[0], 0.0, 0.0);
    } else if (boundingBox.getSize() == boundingBox.getHeight()){
        n = Vec3Df (0.0, boundingBox.getMax()[1] - boundingBox.getMin()[1], 0.0);
    } else{
        n = Vec3Df( 0.0, 0.0, boundingBox.getMax()[2] - boundingBox.getMin()[2]);
    }
    
    n.normalize();
    Vec3Df position = getMedianPoint(tree.getMesh(), n);;
    
    plan.n = n;
    plan.position = position;
    
    if (depth == maxDepth) {
        return;
    }
    
    //seperate triangles
    
    vector<int> leftIndexes, rightIndexes;
    for (int idx : triangleIndexes){
        
        const Triangle& t = tree.getMesh().getTriangles()[idx];
        
        const Vec3Df& a = tree.getMesh().getVertices()[ t.getVertex(0)].getPos();
        const Vec3Df& b = tree.getMesh().getVertices()[ t.getVertex(1)].getPos();
        const Vec3Df& c = tree.getMesh().getVertices()[ t.getVertex(2)].getPos();
        
        
        //regarder si tous les sommets sont du même côté du plan...
        if ( plan.isLeft(a) && plan.isLeft(b) && plan.isLeft(c) ){
            //Ils sont tous à gauche
            leftIndexes.push_back(idx);
            
        } else if( plan.isRight(a) && plan.isRight(b) && plan.isRight(c) ){
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

Vec3Df KDTree::Node::getMedianPoint(const Mesh& mesh, const Vec3Df& normal) const {
    
    vector<float> median_float;
    const size_t n = triangleIndexes.size();
    
    for (unsigned int i = 0; i<n ; ++i){
        
        const Triangle& t = mesh.getTriangles()[triangleIndexes[i]];
        const Vec3Df& a = mesh.getVertices()[t.getVertex(0)].getPos();
        const Vec3Df& b = mesh.getVertices()[t.getVertex(1)].getPos();
        const Vec3Df& c = mesh.getVertices()[t.getVertex(2)].getPos();
        
        Vec3Df average = (a + b + c) / 3;
        
        median_float.push_back(Vec3Df::dotProduct(average, normal) );
    }
    
    std::vector<float> median_float_copy = median_float;
    
    std::nth_element(median_float_copy.begin(), median_float_copy.begin()+median_float_copy.size()/2, median_float_copy.end());
    
    float median = median_float_copy[median_float.size()/2];
    
    std::vector<float>::iterator it = std::find(median_float.begin(), median_float.end(), median);
    std::vector<float>::size_type pos = std::distance(median_float.begin(), it);
    
    const Triangle& t = mesh.getTriangles()[triangleIndexes[pos]];
    const Vec3Df& a = mesh.getVertices()[t.getVertex(0)].getPos();
    const Vec3Df& b = mesh.getVertices()[t.getVertex(1)].getPos();
    const Vec3Df& c = mesh.getVertices()[t.getVertex(2)].getPos();
    
    return (a + b + c) / 3;
    
}

bool KDTree::Plan::isLeft(const Vec3Df& point) const {
    
    return Vec3Df::dotProduct(n , point-position) < 0;
    
}

bool KDTree::Plan::isRight(const Vec3Df& point) const {
    
    return Vec3Df::dotProduct(n , point-position) > 0;
    
}


#pragma mark - Ray Intersection

bool KDTree::Node::intersectRay(const Ray& ray, const KDTree& tree, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle) const
{
    // Check if ray intersect bouding box
    if(!ray.intersect(boundingBox, intersectionPoint)){
        return false;
    }
    
    // If there are no sons, check with node triangles
    if(left_node == NULL && right_node == NULL){
        
        bool intersection = false;
        
        const std::vector<Triangle>& triangles = tree.getMesh().getTriangles();
        const std::vector<Vertex>& vertices = tree.getMesh().getVertices();
    
        intersectionDistance = std::numeric_limits<float>::max();
        
        float triangleIntersectionDistance;
        Vec3Df triangleIntersectionPoint;
        
        for (int index : triangleIndexes) {

            if (ray.intersect(triangles[index], vertices, triangleIntersectionDistance, triangleIntersectionPoint)
                && triangleIntersectionDistance < intersectionDistance) {
                intersectionDistance = triangleIntersectionDistance;
                intersectionPoint = triangleIntersectionPoint;
                intersectionTriangle = triangles[index];
                intersection = true;
            }
        }
        
        return intersection;

    }
    
    // Otherwise, check with the 2 sons
    float intersectionDistanceRight;
    Vec3Df intersectionPointRight;
    Triangle intersectionTriangleRight;

    bool intersectionLeft = left_node && left_node->intersectRay(ray, tree, intersectionDistance, intersectionPoint, intersectionTriangle);
    bool intersectionRight = right_node && right_node->intersectRay(ray, tree, intersectionDistanceRight, intersectionPointRight, intersectionTriangleRight);
    
    if ((intersectionRight && !intersectionLeft)
        || (intersectionRight && intersectionLeft && intersectionDistanceRight < intersectionDistance)){
        
        intersectionDistance = intersectionDistanceRight;
        intersectionPoint = intersectionPointRight;
        intersectionTriangle = intersectionTriangleRight;
        
    }
    
    return intersectionLeft || intersectionRight;
}

bool KDTree::intersectRay(const Ray& ray, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle) const
{
    return _root.intersectRay(ray, *this, intersectionDistance, intersectionPoint, intersectionTriangle);
}