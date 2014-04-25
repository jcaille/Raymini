//
//  KDTree.h
//  RayTracer
//
//  Created by Audrey FOURNERET on 22/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#ifndef __RayTracer__KDTree__
#define __RayTracer__KDTree__

#include <iostream>
#include <vector>
#include "Triangle.h"
#include "Vertex.h"
#include "Mesh.h"
#include "BoundingBox.h"

using namespace std;

class Ray;

class KDTree {

    protected :
    
    Mesh mesh;
    
    struct Plan{
        
        Vec3Df position;
        Vec3Df n;
        
        Plan(Vec3Df position, Vec3Df n);
        Plan(){};
        bool isLeft(Vec3Df point);
    };
    
    struct Node{
        
        Node();
        ~Node();
        Node(const Node& node);
                
        // The depth of the node - If 0, node is root
        unsigned int depth;
        // The indices of node triangles in the tree->triangle vector.
        vector<int> triangleIndexes;
        
        // A bouding box of the list_triangles
        BoundingBox boundingBox;
        // The plan seperating left_node and right_node
        Plan plan;
        
        // Sons
        Node* left_node;
        Node* right_node;
        
        // Builds a node by finding the bouding box, the separating plan and dividing triangles between left and right node.
        void buildNode(const KDTree& tree, int maxDepth);
        
        Node(vector<int>& triangleIndexes, int depth, const KDTree& tree);
        
        Vec3Df getMedianPoint(const Mesh& mesh, Vec3Df normal);
        static bool compareTriangles(const int triangleIndex1, const int triangleIndex2);

        bool intersectRay(const Ray& ray, const KDTree& tree, float& intersectionDistance, Triangle& intersectionTriangle) const;
    };
    
    public :
    
        KDTree(const Mesh& mesh, int maxDepth);
        bool intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle) const;
    
    private :
    
        void buildRootNode(int maxDepth);
    
        Node root;
    
};

#endif /* defined(__RayTracer__KDTree__) */
