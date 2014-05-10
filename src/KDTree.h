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

    
    struct Plan{
        
        Vec3Df position;
        Vec3Df n;
        
        Plan(){};
        bool isLeft(const Vec3Df& point) const;
        bool isRight(const Vec3Df& point) const;
    };
    
    struct Node{
        
        Node();
        ~Node();
        
        Node(const Node& node);
        Node& operator= (const Node& node);

        
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
        
        Node(const vector<int>& triangleIndexes, int depth, const KDTree& tree);
        
        Vec3Df getMedianPoint(const Mesh& mesh, const Vec3Df& normal) const ;

        bool intersectRay(const Ray& ray, const KDTree& tree, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle) const;
    };
    
    public :
    
        inline KDTree(){};
        inline ~KDTree(){};
    
        KDTree(const Mesh& mesh, int maxDepth = -1);
    
        inline const Mesh& getMesh() const { return _mesh; }
        inline Mesh& getMesh() { return _mesh; }
    
        bool intersectRay(const Ray& ray, float& intersectionDistance, Vec3Df& intersectionPoint, Triangle& intersectionTriangle) const;
    
    private :
    
        void buildRootNode(int maxDepth);
    
        Node _root;
        Mesh _mesh;
    
};

#endif /* defined(__RayTracer__KDTree__) */
