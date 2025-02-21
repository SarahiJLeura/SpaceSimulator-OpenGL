#pragma once

#include "Vertex.h"
#include "Face.h"
#include <vector>
#include "Model.h"

using namespace std;

extern Model modelObjects;

float areaTriangulo(const Vertex& v1, const Vertex& v2, const Vertex& v3);
Vertex centroideTriangulo(const Vertex& v1, const Vertex& v2, const Vertex& v3);
Vertex calcularCentroideFigura(const std::vector<Face>& faces);
Vertex calculateFaceNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3);
Vertex calculateNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3);
void calculateVertexNormals();