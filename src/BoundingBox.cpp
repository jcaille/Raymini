/*
LibBoubek/BoundingBox.cpp
Copyright (c) 2003-2008, Tamy Boubekeur

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* The name of the source code copyright owner cannot be used to endorse or
  promote products derived from this software without specific prior
  written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "BoundingBox.h"

BoundingBox::BoundingBox(const std::vector< Triangle > & list_triangles, const std::vector< int > & triangle_indexes, const std::vector<Vertex>& V){
    
    std::vector<float> triangle_x, triangle_y, triangle_z;
        
    for (unsigned int i=0; i< triangle_indexes.size() ; i++){
        for (unsigned int j=0; j<3; j++){
            
            triangle_x.push_back(V[list_triangles[triangle_indexes[i]].getVertex(j)].getPos()[0]);
            triangle_y.push_back(V[list_triangles[triangle_indexes[i]].getVertex(j)].getPos()[1]);
            triangle_z.push_back(V[list_triangles[triangle_indexes[i]].getVertex(j)].getPos()[2]);
        }
    }
    
    float maxX, maxY, maxZ;
    maxX = *std::max_element(triangle_x.begin(), triangle_x.end());
    maxY = *std::max_element(triangle_y.begin(), triangle_y.end());
    maxZ = *std::max_element(triangle_z.begin(), triangle_z.end());
    
   maxBb = Vec3Df(maxX, maxY, maxZ);
    
    minBb = Vec3Df(*std::min_element(triangle_x.begin(), triangle_x.end()), *std::min_element(triangle_y.begin(), triangle_y.end()), *std::min_element(triangle_z.begin(), triangle_z.end()));

}