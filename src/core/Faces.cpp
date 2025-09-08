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
//TODO: This could be better implemented with an auxiliary array that stores the starting idx for each face
// idxArray[i] = <idx where face i starts in coordIndex>. I want to test the trivial implementation first and refactor it later.
Faces::Faces(const int nV, const vector<int>& coordIndex) {
    _coordIndex = coordIndex; // vector deep copy
    _faceStartingIndex = vector<int>();
    _nV = nV;
    _faceStartingIndex.push_back(0);
    int nextFace = 1;
    for (size_t i = 0; i < _coordIndex.size() - 1; ++i) {
        if (_coordIndex[i] < 0) {
            _faceStartingIndex.push_back(i + 1);
            _coordIndex[i] = -nextFace;
            ++nextFace;
        }
    }
    _coordIndex[_coordIndex.size() - 1] = -nextFace;
}

int Faces::getNumberOfVertices() const {
    _nV; // TODO: check this value in the constructos
}

int Faces::getNumberOfFaces() const {
    return _faceStartingIndex.size();
}

int Faces::getNumberOfCorners() const {
    return _coordIndex.size();
}

int Faces::getFaceSize(const int iF) const {
    int faceIdx = getFaceFirstCorner(iF);
    if (faceIdx < 0) {
        return 0;
    }
    int count = 0;
    for(; faceIdx < _coordIndex.size(); ++faceIdx) {
        if (_coordIndex[faceIdx] < 0) break;
        ++count;
    }
    return count;
}

int Faces::getFaceFirstCorner(const int iF) const {
    if (iF < 0 || iF >= _faceStartingIndex.size()) {
        return -1;
    }
    return _faceStartingIndex[iF];
}

int Faces::getFaceVertex(const int iF, const int j) const {
    int faceSize = getFaceSize(iF);
    if (faceSize == 0 || j >= faceSize) {
        return -1;
    }
    int faceIdx = getFaceFirstCorner(iF);
    return _coordIndex[faceIdx + j];
}

int Faces::getCornerFace(const int iC) const {
    int vectorSize = _coordIndex.size();
    if (iC >= vectorSize) {
        return -1;
    }
    if (_coordIndex[iC] < 0) {
        return -1;
    }
    for (size_t i = iC; i < _coordIndex.size(); ++i) {
        if (_coordIndex[i] < 0) {
            return -_coordIndex[i] - 1;
        }
    }
}

int Faces::getNextCorner(const int iC) const {
    int vectorSize = _coordIndex.size();
    if (iC >= vectorSize) {
        return -1;
    }
    if (_coordIndex[iC] < 0) {
        return -1;
    }
    int nextCoordValue = _coordIndex[iC + 1];
    if (nextCoordValue < 0) {
        return _faceStartingIndex[-nextCoordValue - 1];
    } else {
        return iC + 1;
    }
}

