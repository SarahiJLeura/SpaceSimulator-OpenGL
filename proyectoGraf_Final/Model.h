#pragma once

#include "Vertex.h"
#include "Face.h"
#include <vector>
#include <string>
#include "Matrix4x4.h"

using namespace std;

class Model
{
public:
    struct itemStruct {
        vector<Face> faces;
        Matrix4x4 modelMatrix;
    
        itemStruct() : modelMatrix(Matrix4x4::Matrix4x4()) {}
        int numVertixes = 0;
    };

    struct TextureCoord {
        float u, v;
    };

    vector<Vertex> vertixes;
    vector<Vertex> normals;
    vector<TextureCoord> texCoords;
    vector<itemStruct> objects;

    void loadObject2(const string& filename);
    void loadObject(const string& filename);
    void loadMaterials(const string& mtlFilename);
    unsigned int loadTexture(const string& textureFile);
};

