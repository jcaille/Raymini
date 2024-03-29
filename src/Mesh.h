// ---------------------------------------------------------
// Mesh Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2008 Tamy Boubekeur.
// All rights reserved.
// ---------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "Vertex.h"
#include "Triangle.h"
#include "Edge.h"

class Mesh {
public:
    inline Mesh () {} 
    inline Mesh (const std::vector<Vertex> & v) 
        : vertices (v) {}
    inline Mesh (const std::vector<Vertex> & v, 
                 const std::vector<Triangle> & t) 
        : vertices (v), triangles (t)  {}
    inline Mesh (const Mesh & mesh) 
        : vertices (mesh.vertices), 
          triangles (mesh.triangles) {}
        
    inline ~Mesh () {}
    std::vector<Vertex> & getVertices () { return vertices; }
    const std::vector<Vertex> & getVertices () const { return vertices; }
    std::vector<Triangle> & getTriangles () { return triangles; }
    const std::vector<Triangle> & getTriangles () const { return triangles; }
    void clear ();
    void clearGeometry ();
    void clearTopology ();
    void unmarkAllVertices ();
    void recomputeSmoothVertexNormals (unsigned int weight);
    void computeTriangleNormals (std::vector<Vec3Df> & triangleNormals);  
    void collectOneRing (std::vector<std::vector<unsigned int> > & oneRing) const;
    void collectOrderedOneRing (std::vector<std::vector<unsigned int> > & oneRing) const;
    void computeDualEdgeMap (EdgeMapIndex & dualVMap1, EdgeMapIndex & dualVMap2);
    void markBorderEdges (EdgeMapIndex & edgeMap);
    void barycentricCoordinates(const Vec3Df pt, const Triangle& triangle, std::vector<float>& coords) const;
    float area(const Triangle& triangle) const;
    void sample(float density, std::vector<Vec3Df>& samples) const;
    
    void renderGL (bool flat) const;
    
    void loadOFF (const std::string & filename);
    
    void makePlane(const Vec3Df& center, const Vec3Df& normal, const Vec3Df& up, float sizeX = 1, float sizeY = 1);
    void makeBox(const Vec3Df& center, const Vec3Df& front, const Vec3Df& up, const Vec3Df& size = Vec3Df(1,1,1));

    void rotateAroundZ(float angle);
    void rotateAroundY(float angle);
    void rotateAroundX(float angle);
    void scale(float lambda);
    
    Vec3Df getNormal(const Triangle& triangle, const std::vector<float>& barycentricCoordinates) const;
    
    class Exception {
    private: 
        std::string msg;
    public:
        Exception (const std::string & msg) : msg ("[Mesh Exception]" + msg) {}
        virtual ~Exception () {}
        inline const std::string & getMessage () const { return msg; }
    };

private:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};

#endif // MESH_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
