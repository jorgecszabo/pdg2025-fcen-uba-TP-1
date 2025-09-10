//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:12:21 gtaubin>
//------------------------------------------------------------------------
//
// LoaderStl.cpp
//
// Written by: <Your Name>
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

#include <stdio.h>
#include "TokenizerFile.hpp"
#include "LoaderStl.hpp"
#include "StrException.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"
#include <iostream> // TODO: deleteme

// reference
// https://en.wikipedia.org/wiki/STL_(file_format)

const char* LoaderStl::_ext = "stl";

bool LoaderStl::load(const char* filename, SceneGraph& wrl) {
  bool success = false;

  // clear the scene graph
  wrl.clear();
  wrl.setUrl("");

  FILE* fp = (FILE*)0;
  try {

    // open the file
    if(filename==(char*)0) throw new StrException("filename==null");
    fp = fopen(filename,"r");
    if(fp==(FILE*)0) throw new StrException("fp==(FILE*)0");

    wrl.setUrl(filename);

    // use the io/Tokenizer class to parse the input ascii file

    TokenizerFile tkn(fp);
    // first token should be "solid"
    if(tkn.expecting("solid") && tkn.get()) {
        string stlName = tkn; // second token should be the solid name

        Shape* shape = new Shape();
        wrl.addChild(shape);

        Appearance* appearance = new Appearance();
        shape->setAppearance(appearance);

        Material* material = new Material();
        appearance->setMaterial(material);

        IndexedFaceSet* geometry = new IndexedFaceSet();
        shape->setGeometry(geometry);

        vector<float>& normal = geometry->getNormal();
        vector<int>& coordIndex = geometry->getCoordIndex();
        vector<float>& coord = geometry->getCoord();
        float floatBuffer;
        Vec3f vec3fBuffer;
        uint facetNumber = 0;
        // This code is terrible >:(
        for (;;) {
            if(!(tkn.expecting("facet") && tkn.expecting("normal"))) {
                if (facetNumber > 0) {
                    break; // We parsed at least one face
                } else {
                    throw StrException("Invalid stl file, expecting facet normal");
                }
            }

            bool success = tkn.getVec3f(vec3fBuffer);
            if (!success) {
                throw StrException("Invalid stl file, failed to read float value");
            }

            // One normal per vertex (Can I save memory by not repeating this information)
            for (int i = 0; i < 3; ++i) {
                normal.push_back(vec3fBuffer.x);
                normal.push_back(vec3fBuffer.y);
                normal.push_back(vec3fBuffer.z);
            }

            if(!(tkn.expecting("outer") && tkn.expecting("loop"))) {
                throw StrException("Invalid stl file, expecting outer loop");
            }

            for (int i = 0; i < 3; ++i) {
                if (!tkn.expecting("vertex")) {
                    throw StrException("Invalid stl file, expecting vertex");
                }
                for (int j = 0; j < 3; ++j) {
                    bool success = tkn.getFloat(floatBuffer);
                    if (!success) {
                        throw StrException("Invalid stl file, failed to read float value");
                    }
                    coord.push_back(floatBuffer);
                }
                coordIndex.push_back(3 * facetNumber + i); // The coords for each facet are <v1 v2 v3> counterclockwise
            }
            coordIndex.push_back(-1);
            tkn.expecting("endloop");
            tkn.expecting("endfacet");
            ++facetNumber;
        }
        success = true;

      // create the scene graph structure :
      // 1) the SceneGraph should have a single Shape node a child
      // 2) the Shape node should have an Appearance node in its appearance field
      // 3) the Appearance node should have a Material node in its material field
      // 4) the Shape node should have an IndexedFaceSet node in its geometry node

      // from the IndexedFaceSet
      // 5) get references to the coordIndex, coord, and normal arrays
      // 6) set the normalPerVertex variable to false (i.e., normals per face)  

      // the file should contain a list of triangles in the following format

      // facet normal ni nj nk
      //   outer loop
      //     vertex v1x v1y v1z
      //     vertex v2x v2y v2z
      //     vertex v3x v3y v3z
      //   endloop
      // endfacet

      // - run an infinite loop to parse all the faces
      // - write a private method to parse each face within the loop
      // - the method should return true if successful, and false if not
      // - if your method returns tru
      //     update the normal, coord, and coordIndex variables
      // - if your method returns false
      //     throw an StrException explaining why the method failed

    }

    // close the file (this statement may not be reached)
    fclose(fp);
    
  } catch(StrException* e) { 
    
    if(fp!=(FILE*)0) fclose(fp);
    fprintf(stderr,"ERROR | %s\n",e->what());
    delete e;

  }

  return success;
}

