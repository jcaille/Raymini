// ---------------------------------------------------------
// Mesh Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2008 Tamy Boubekeur.
// All rights reserved.
// ---------------------------------------------------------

#include "Mesh.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <OpenGL/gl.h>

using namespace std;

void Mesh::clear () {
    clearTopology ();
    clearGeometry ();
}

void Mesh::clearGeometry () {
    vertices.clear ();
}

void Mesh::clearTopology () {
    triangles.clear ();
}

void Mesh::unmarkAllVertices () {
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].unmark ();
}

void Mesh::computeTriangleNormals (vector<Vec3Df> & triangleNormals) {
    for (vector<Triangle>::const_iterator it = triangles.begin ();
         it != triangles.end ();
         it++) {
        Vec3Df e01 (vertices[it->getVertex (1)].getPos () - vertices[it->getVertex (0)].getPos ());
        Vec3Df e02 (vertices[it->getVertex (2)].getPos () - vertices[it->getVertex (0)].getPos ());
        Vec3Df n (Vec3Df::crossProduct (e01, e02));
        n.normalize ();
        triangleNormals.push_back (n);
    }
}

void Mesh::recomputeSmoothVertexNormals (unsigned int normWeight) {
    vector<Vec3Df> triangleNormals;
    computeTriangleNormals (triangleNormals);
    for (std::vector<Vertex>::iterator it = vertices.begin (); it != vertices.end (); it++)
        it->setNormal (Vec3Df (0.0, 0.0, 0.0));
    vector<Vec3Df>::const_iterator itNormal = triangleNormals.begin ();
    vector<Triangle>::const_iterator it = triangles.begin ();
    for ( ; it != triangles.end (); it++, itNormal++) 
        for (unsigned int  j = 0; j < 3; j++) {
            Vertex & vj = vertices[it->getVertex (j)];
            float w = 1.0; // uniform weights
            Vec3Df e0 = vertices[it->getVertex ((j+1)%3)].getPos () - vj.getPos ();
            Vec3Df e1 = vertices[it->getVertex ((j+2)%3)].getPos () - vj.getPos ();
            if (normWeight == 1) { // area weight
                w = Vec3Df::crossProduct (e0, e1).getLength () / 2.0;
            } else if (normWeight == 2) { // angle weight
                e0.normalize ();
                e1.normalize ();
                w = (2.0 - (Vec3Df::dotProduct (e0, e1) + 1.0)) / 2.0;
            } 
            if (w <= 0.0)
                continue;
            vj.setNormal (vj.getNormal () + (*itNormal) * w);
        }
    Vertex::normalizeNormals (vertices);
}

void Mesh::collectOneRing (vector<vector<unsigned int> > & oneRing) const {
    oneRing.resize (vertices.size ());
    for (unsigned int i = 0; i < triangles.size (); i++) {
        const Triangle & ti = triangles[i];
        for (unsigned int j = 0; j < 3; j++) {
            unsigned int vj = ti.getVertex (j);
            for (unsigned int k = 1; k < 3; k++) {
                unsigned int vk = ti.getVertex ((j+k)%3);
                if (find (oneRing[vj].begin (), oneRing[vj].end (), vk) == oneRing[vj].end ())
                    oneRing[vj].push_back (vk);
            }
        }
    }
}

void Mesh::collectOrderedOneRing (vector<vector<unsigned int> > & oneRing) const {
    oneRing.resize (vertices.size ());
    for (unsigned int t = 0; t < triangles.size (); t++) {
        const Triangle & ti = triangles[t];
        for (unsigned int i = 0; i < 3; i++) {
            unsigned int vi = ti.getVertex (i);
            unsigned int vj = ti.getVertex ((i+1)%3);
            unsigned int vk = ti.getVertex ((i+2)%3);
            vector<unsigned int> & oneRingVi = oneRing[vi];
            vector<unsigned int>::iterator begin = oneRingVi.begin ();
            vector<unsigned int>::iterator end = oneRingVi.end ();
            vector<unsigned int>::iterator nj = find (begin, end, vj);
            vector<unsigned int>::iterator nk = find (begin, end, vk);
            if (nj != end && nk == end) {
                if (nj == begin)
                    nj = end;
                nj--;
                oneRingVi.insert (nj, vk);
            } else if (nj == end && nk != end) 
                oneRingVi.insert (nk, vj);
            else if (nj == end && nk == end) {
                oneRingVi.push_back (vk);
                oneRingVi.push_back (vj);
            }
        }
    }
}

void Mesh::computeDualEdgeMap (EdgeMapIndex & dualVMap1, EdgeMapIndex & dualVMap2) {
    for (vector<Triangle>::iterator it = triangles.begin ();
         it != triangles.end (); it++) {
        for (unsigned int i = 0; i < 3; i++) {
            Edge eij (it->getVertex (i), it->getVertex ((i+1)%3)); 
            if (dualVMap1.find (eij) == dualVMap1.end ())
                dualVMap1[eij] = it->getVertex ((i+2)%3);
            else
                dualVMap2[eij] = it->getVertex ((i+2)%3);
        }
    } 
}

void Mesh::markBorderEdges (EdgeMapIndex & edgeMap) {
    for (vector<Triangle>::iterator it = triangles.begin ();
         it != triangles.end (); it++) {
        for (unsigned int i = 0; i < 3; i++) {
            unsigned int j = (i+1)%3;
            Edge eij (it->getVertex (i), it->getVertex (j)); 
            if (edgeMap.find (eij) == edgeMap.end ())
                edgeMap[eij] = 0;
            else 
                edgeMap[eij] += 1;
        }
    } 
}

void Mesh::barycentricCoordinates(const Vec3Df pt, const Triangle& triangle, std::vector<float>& coords) const
{
    Vec3Df p0 = vertices[triangle.getVertex(0)].getPos();
    Vec3Df p1 = vertices[triangle.getVertex(1)].getPos();
    Vec3Df p2 = vertices[triangle.getVertex(2)].getPos();
    
    Vec3Df e0 = p1 - p0;
    Vec3Df e1 = p2 - p0;
    
    float ne0 = e0.getSquaredLength();
    float ne1 = e1.getSquaredLength();
    float e0e1 = Vec3Df::dotProduct(e0, e1);
    
    float det = ne0*ne1 - e0e1 * e0e1;
    
    float a0 = Vec3Df::dotProduct(pt-p0, e0);
    float a1 = Vec3Df::dotProduct(pt-p0, e1);
    
    float b0 = (ne1 * a0 - e0e1 * a1)/det;
    float b1 = (ne0 * a1 - e0e1 * a0)/det;
    
    coords.clear();
    coords.push_back(1-(b0+b1));
    coords.push_back(b0);
    coords.push_back(b1);
}

float Mesh::area(const Triangle& triangle) const
{
    Vec3Df p0 = vertices[triangle.getVertex(0)].getPos();
    Vec3Df p1 = vertices[triangle.getVertex(1)].getPos();
    Vec3Df p2 = vertices[triangle.getVertex(2)].getPos();
    
    Vec3Df e0 = p1 - p0;
    Vec3Df e1 = p2 - p0;

    return  .5* Vec3Df::crossProduct(e0, e1).getLength();
    
}

static inline float random_float(float lo, float hi)
{
    return lo + static_cast<float>( rand() ) / ( static_cast <float> (RAND_MAX/(hi-lo)));
}

void Mesh::sample(float density, std::vector<Vec3Df>& samples) const
{

    float acc = 0;
    
    for (const Triangle& triangle : triangles){
        
        // Compute the number of samples we want for this triangle
        acc += area(triangle) * density;
        
        int n = round(acc);
        if (n>0)
            acc -= n;
        
        Vec3Df p0 = vertices[triangle.getVertex(0)].getPos();
        Vec3Df p1 = vertices[triangle.getVertex(1)].getPos();
        Vec3Df p2 = vertices[triangle.getVertex(2)].getPos();
        
        // Draw those samples
        for (int i = 0; i < n; ++i){
            
            float b0 = random_float(0 , 1 );
            float b1 = random_float(0 , 1 - b0 );
            float b2 = 1 - b0 - b1;
            
            samples.push_back(b0*p0 + b1*p1 + b2*p2);
            
        }
        
    }
    
    
}

inline void glVertexVec3Df (const Vec3Df & v) {
    glVertex3f (v[0], v[1], v[2]);
}

inline void glNormalVec3Df (const Vec3Df & n) {
    glNormal3f (n[0], n[1], n[2]);
}
 
inline void glDrawPoint (const Vec3Df & pos, const Vec3Df & normal) {
    glNormalVec3Df (normal);
    glVertexVec3Df (pos);
}

inline void glDrawPoint (const Vertex & v) { 
    glDrawPoint (v.getPos (), v.getNormal ()); 
}

void Mesh::renderGL (bool flat) const {
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < triangles.size (); i++) {
        const Triangle & t = triangles[i];
        Vertex v[3];
        for (unsigned int j = 0; j < 3; j++)
            v[j] = vertices[t.getVertex(j)];
        if (flat) {
            Vec3Df normal = Vec3Df::crossProduct (v[1].getPos () - v[0].getPos (),
                                                  v[2].getPos () - v[0].getPos ());
            normal.normalize ();
            glNormalVec3Df (normal);
        }
        for (unsigned int j = 0; j < 3; j++) 
            if (!flat)
                glDrawPoint (v[j]);
            else
                glVertexVec3Df (v[j].getPos ());
    }
    glEnd ();
}

void Mesh::loadOFF (const std::string & filename) {
    clear ();
    ifstream input (filename.c_str ());
    if (!input)
        throw Exception ("Failing opening the file.");
    string magic_word;
    input >> magic_word;
    if (magic_word != "OFF")
        throw Exception ("Not an OFF file.");
    unsigned int numOfVertices, numOfTriangles, numOfWhat;
    input >> numOfVertices >> numOfTriangles >> numOfWhat;
    for (unsigned int i = 0; i < numOfVertices; i++) {
        Vec3Df pos;
        Vec3Df col;
        input >> pos;
        vertices.push_back (Vertex (pos, Vec3Df (1.0, 0.0, 0.0)));
    }
    for (unsigned int i = 0; i < numOfTriangles; i++) {
        unsigned int polygonSize;
        input >> polygonSize;
        vector<unsigned int> index (polygonSize);
        for (unsigned int j = 0; j < polygonSize; j++)
            input >> index[j];
        for (unsigned int j = 1; j < (polygonSize - 1); j++)
            triangles.push_back (Triangle (index[0], index[j], index[j+1]));
    }
    input.close ();
    recomputeSmoothVertexNormals (0);
}

void Mesh::makePlane(const Vec3Df& center, const Vec3Df& normal, const Vec3Df& up, float sizeX, float sizeY)
{
    Vec3Df u = -Vec3Df::crossProduct(normal, up);
    u.normalize();
    u *= sizeX;
    
    Vec3Df v = up;
    v.normalize();
    v *= sizeY;
    
    Vec3Df origin = center - .5*( u + v );
    
    int idx = (int)vertices.size();
    
    vertices.push_back (Vertex (origin        , normal));
    vertices.push_back (Vertex (origin + u    , normal));
    vertices.push_back (Vertex (origin + v    , normal));
    vertices.push_back (Vertex (origin + u + v, normal));
    
    triangles.push_back (Triangle (idx+0,idx+1,idx+3));
    triangles.push_back (Triangle (idx+0,idx+3,idx+2));
    
    recomputeSmoothVertexNormals (0);
}

void Mesh::makeBox(const Vec3Df& center, const Vec3Df& front, const Vec3Df& up, const Vec3Df& size)
{

    Vec3Df u = -Vec3Df::crossProduct(front, up);
    u.normalize();
    u *= size[0];

    Vec3Df v = -Vec3Df::crossProduct(u, up);
    v.normalize();
    v *= size[1];

    Vec3Df w = up;
    w.normalize();
    w *= size[2];
    
    
    makePlane(center - .5 * w,  -w,   v, size[0], size[1]);
    makePlane(center + .5 * w,   w,   v, size[0], size[1]);
    makePlane(center - .5 * v,  -v,   w, size[0], size[2]);
    makePlane(center + .5 * v,   v,   w, size[0], size[2]);
    makePlane(center - .5 * u,  -u,   w, size[1], size[2]);
    makePlane(center + .5 * u,   u,   w, size[1], size[2]);
}

void Mesh::rotateAroundZ(float angle)
{
    vector<Vertex> v;
    for (int i = 0; i < vertices.size(); i++) {
        Vec3Df newV;
        Vec3Df V = vertices[i].getPos();
        newV[0] = cos(angle) * V[0] + sin(angle) * V[1];
        newV[1] = -sin(angle) * V[0] + cos(angle) * V[1];
        newV[2] = V[2];
        vertices[i].setPos(newV);
    }
    recomputeSmoothVertexNormals(0);
}

void Mesh::rotateAroundY(float angle)
{
    vector<Vertex> v;
    for (int i = 0; i < vertices.size(); i++) {
        Vec3Df newV;
        Vec3Df V = vertices[i].getPos();
        newV[0] = cos(angle) * V[0] + sin(angle) * V[2];
        newV[1] = V[1];
        newV[2] = -sin(angle) * V[0] + cos(angle) * V[2];
        vertices[i].setPos(newV);
    }
    recomputeSmoothVertexNormals(0);
}

void Mesh::rotateAroundX(float angle)
{
    vector<Vertex> v;
    for (int i = 0; i < vertices.size(); i++) {
        Vec3Df newV;
        Vec3Df V = vertices[i].getPos();
        newV[0] = V[0];
        newV[1] = cos(angle) * V[1] + sin(angle) * V[2];
        newV[2] = -sin(angle) * V[1] + cos(angle) * V[2];
        vertices[i].setPos(newV);
    }
    recomputeSmoothVertexNormals(0);
}

void Mesh::scale(float lambda)
{
    vector<Vertex> v;
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].setPos(lambda * vertices[i].getPos());
    }
    recomputeSmoothVertexNormals(0);
    
}
Vec3Df Mesh::getNormal(const Triangle &triangle, const std::vector<float> &barycentricCoordinates) const {
    Vertex p0 = vertices[triangle.getVertex(0)];
    Vertex p1 = vertices[triangle.getVertex(1)];
    Vertex p2 = vertices[triangle.getVertex(2)];
    
    Vec3Df n0 = p0.getNormal();
    Vec3Df n1 = p1.getNormal();
    Vec3Df n2 = p2.getNormal();
    
    Vec3Df norm = barycentricCoordinates[0] * n0 + barycentricCoordinates[1] * n1 + barycentricCoordinates[2] * n2;
    norm.normalize();
    return norm;
}
