//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:14:44 gtaubin>
//------------------------------------------------------------------------
//
// SaverStl.cpp
//
// Written by: Jorge Szabo
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "SaverStl.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

#include "core/Faces.hpp"

const char* SaverStl::_ext = "stl";

//////////////////////////////////////////////////////////////////////
bool SaverStl::save(const char* filename, SceneGraph& wrl) const {
  bool success = false;
  if(filename!=(char*)0) {

    // Check these conditions

    // 1) the SceneGraph should have a single child
    // 2) the child should be a Shape node
    // 3) the geometry of the Shape node should be an IndexedFaceSet node

    // - construct an instance of the Faces class from the IndexedFaceSet
    // - remember to delete it when you are done with it (if necessary)
    //   before returning

    // 4) the IndexedFaceSet should be a triangle mesh
    // 5) the IndexedFaceSet should have normals per face

    // if (all the conditions are satisfied) {

    //FIXME: What if I have more than one child?
    auto children = wrl.getChildren();
    IndexedFaceSet* geometry = nullptr;
    for (auto child : children) {
        if (child->isShape()) {
            geometry = (IndexedFaceSet*) ((Shape*) child)->getGeometry();
        }
    }
    if (!geometry) {
        return false; //TODO add exception
    }
    vector<int>& coordIndex = geometry->getCoordIndex();
    vector<float>& coord = geometry->getCoord();
    vector<int>& normalIndex = geometry->getNormalIndex();
    vector<float>& normal = geometry->getNormal();
    int numberVertex = geometry->getNumberOfCorners(); //is this okay?
    if (!geometry->isIndexedFaceSet()) {
        return false; //not supported yet
    }

    Faces faces = Faces(numberVertex, coordIndex);
    Vec3f vec3fBuffer;

    FILE* fp = fopen(filename,"w");
    if(	fp!=(FILE*)0) {

      // if set, use ifs->getName()
      // otherwise use filename,
      // but first remove directory and extension
      const string& solidName = wrl.getName();
      const char* solidNameCstr = solidName.empty() ? filename : solidName.c_str();
      fprintf(fp,"solid %s\n",solidNameCstr);
      for (int faceNumber = 0;;++faceNumber) {
            if (faces.getFaceVertex(faceNumber, 0) == -1) {
                break;
            }
            int normalNumber = normalIndex.empty() ? faceNumber : normalIndex[faceNumber];
            vec3fBuffer.x = normal[3 * normalNumber];
            vec3fBuffer.y = normal[3 * normalNumber + 1];
            vec3fBuffer.z = normal[3 * normalNumber + 2];
            fprintf(fp,"facet normal %f %f %f\n", vec3fBuffer.x, vec3fBuffer.y, vec3fBuffer.z);
            fprintf(fp, "  outer loop\n");
            for (int cornerNumber = 0; cornerNumber < 3; ++cornerNumber) {
                int vertexNumber = faces.getFaceVertex(faceNumber, cornerNumber);
                vec3fBuffer.x = coord[3 * vertexNumber];
                vec3fBuffer.y = coord[3 * vertexNumber + 1];
                vec3fBuffer.z = coord[3 * vertexNumber + 2];
                fprintf(fp,"    vertex %f %f %f\n", vec3fBuffer.x, vec3fBuffer.y, vec3fBuffer.z);
            }
            fprintf(fp, "  endloop\nendfacet\n");
      }
      fprintf(fp,"endsolid %s\n",solidNameCstr);

      // TODO ...
      // for each face {
      //   ...
      // }
      
      fclose(fp);
      success = true;
    }

  }
  return success;
}
