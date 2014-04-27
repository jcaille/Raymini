//
//  Geometry.cpp
//  RayTracer
//
//  Created by Florian Denis on 27/04/14.
//  Copyright (c) 2014 Florian Denis. All rights reserved.
//

#include "Geometry.h"

void Geometry::_updateBoundingBox()
{
    const vector<Vertex> & V = getMesh().getVertices ();
    if (V.empty ())
        _bbox = BoundingBox ();
    else {
        _bbox = BoundingBox (V[0].getPos ());
        for (unsigned int i = 1; i < V.size (); i++)
            _bbox.extendTo (V[i].getPos ());
    }
}
