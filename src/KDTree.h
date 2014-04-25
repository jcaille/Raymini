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
    
    // The mesh the KDTree is related to
    const Mesh* mesh;
    
    struct Plan{
        
        Vec3Df position;
        Vec3Df n;
        
        Plan(Vec3Df position, Vec3Df n);
        Plan(){};
        bool isLeft(Vec3Df point);
    };
    
    struct Node{
        
        // The tree the node is in
        KDTree* tree;
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
        void buildNode(int maxDepth);
        Node(vector<int>& triangleIndexes, int depth, KDTree* tree);
        Vec3Df getMedianPoint(Vec3Df normal);
        static bool compareTriangles(const int triangleIndex1, const int triangleIndex2);

        bool intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle);
    };
    
      public :
        KDTree();
        KDTree(const Mesh* mesh);
        void buildRootNode(int maxDepth);
        bool intersectRay(const Ray& ray, float& intersectionDistance, Triangle& intersectionTriangle);
    private :
        Node* root;
    
};

#endif /* defined(__RayTracer__KDTree__) */
