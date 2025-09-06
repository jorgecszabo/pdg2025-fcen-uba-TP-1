//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:09:56 gtaubin>
//------------------------------------------------------------------------
//
// Faces.cpp
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

#include <math.h>
#include "Faces.hpp"
#include <algorithm>

Faces::Faces(const int nV, const vector<int>& coordIndex) {
    this->coordIndex = coordIndex; // vector deep copy
    int vertexCount = count_if(this->coordIndex.begin(), this->coordIndex.end(), [](int n){ return n >= 0; });
    this->nV = vertexCount;
}

int Faces::getNumberOfVertices() const {
    return this->nV;
}

int Faces::getNumberOfFaces() const {
  return count_if(this->coordIndex.begin(), this->coordIndex.end(), [](int n){ return n < 0; });
}

int Faces::getNumberOfCorners() const {
    return this->coordIndex.size();
}

int Faces::getFaceSize(const int iF) const {
    int faceIdx = this->getFaceFirstCorner(iF);
    if (faceIdx < 0) {
        return 0;
    }
    int vectorSize = this->coordIndex.size();
    int cornerCount = 0;
    while(faceIdx < vectorSize && this->coordIndex[faceIdx] >= 0) {
        ++cornerCount;
        ++faceIdx;
    }
    return cornerCount;
}

int Faces::getFaceFirstCorner(const int iF) const {
    if (iF < 0) {
        return -1;
    }
    int i, vectorSize;
    vectorSize = this->coordIndex.size();
    int indexFace = iF;
    for (i = 0; i < vectorSize; ++i) {
        if (indexFace == 0) {
            break;
        }
        if (this->coordIndex[i] < 0) {
            --indexFace;
        }
    }
    if (i < vectorSize) {
        return i;
    } else {
        return -1;
    }
}

int Faces::getFaceVertex(const int iF, const int j) const {
    int faceSize = this->getFaceSize(iF);
    if (faceSize == 0 || j >= faceSize) {
        return -1;
    }
    int faceIdx = this->getFaceFirstCorner(iF);
    return this->coordIndex[faceIdx + j];
}

int Faces::getCornerFace(const int iC) const {
    int vectorSize = this->coordIndex.size();
    if (iC >= vectorSize) {
        return -1;
    }
    if (this->coordIndex[iC] < 0) {
        return -1;
    }
    int faceIndex = 0;
    for (size_t i = iC; i >= 0; --i) {
        if (this->coordIndex[i] < 0) {
            ++faceIndex;
        }
    }
    return faceIndex;
}

int Faces::getNextCorner(const int iC) const {
    int vectorSize = this->coordIndex.size();
    if (iC >= vectorSize) {
        return -1;
    }
    if (this->coordIndex[iC] < 0) {
        return -1;
    }
    int nextCornerIndex = iC;
    while (this->coordIndex[nextCornerIndex] >= 0) {
        // this should be slightly faster than nextCornerIndex = (nextCornerIndex + 1) % vectorSize;
        nextCornerIndex = nextCornerIndex >= vectorSize ? 0 : nextCornerIndex + 1;
    }
    return nextCornerIndex + 1;
}

