#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Model::loadObject(const string& filename) {
    ifstream objFile(filename);
    if (!objFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    int numVertixes = 0;
    string line, currentMaterial;
    itemStruct currentItem;
    while (getline(objFile, line)) {
        stringstream ss(line);
        string token;
        ss >> token;

        if (token == "o") {
            if (!currentItem.faces.empty()) {
                objects.push_back(currentItem);
                currentItem = itemStruct();
                numVertixes = 0;
            }
        }
        else if (token == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertixes.push_back(v);
            numVertixes++;
        }
        else if (token == "usemtl") {
            ss >> currentMaterial;
        }
        else if (token == "f") {
            if (currentItem.faces.empty()) currentItem.numVertixes = numVertixes;

            Face f;
            ss >> f.v1 >> f.v2 >> f.v3;
            f.material = currentMaterial;
            currentItem.faces.push_back(f);
        }
    }

    if (!currentItem.faces.empty()) {
        objects.push_back(currentItem);
    }

    objFile.close();
}

void Model::loadObject2(const string& filename) {
    ifstream objFile(filename);
    if (!objFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line, currentMaterial;
    itemStruct currentItem;
    while (getline(objFile, line)) {
        stringstream ss(line);
        string token;
        ss >> token;

        if (token == "o") {
            if (!currentItem.faces.empty()) {
                objects.push_back(currentItem);
                currentItem = itemStruct();
            }
        }
        else if (token == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertixes.push_back(v);
        }
        else if (token == "vt") {
            TextureCoord tex;
            ss >> tex.u >> tex.v;
            texCoords.push_back(tex);
        }
        else if (token == "usemtl") {
            ss >> currentMaterial;
        }
        else if (token == "f") {
            Face f;
            char slash;
            ss >> f.v1 >> slash >> f.vt1
                >> f.v2 >> slash >> f.vt2
                >> f.v3 >> slash >> f.vt3;
            f.material = currentMaterial;
            currentItem.faces.push_back(f);
        }
    }

    if (!currentItem.faces.empty()) {
        objects.push_back(currentItem);
    }

    objFile.close();
}

