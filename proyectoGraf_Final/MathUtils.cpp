#include "MathUtils.h"

#include <cmath>

// Función para calcular el área de un triángulo en 3D
float areaTriangulo(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    Vertex AB = v2 - v1;
    Vertex AC = v3 - v1;

    Vertex crossProduct = AB.crossProduct(AC);

    // Magnitud del vector cruzado es el área del triángulo multiplicada por 2
    return std::sqrt(crossProduct.x * crossProduct.x +
        crossProduct.y * crossProduct.y +
        crossProduct.z * crossProduct.z) / 2.0f;
}

// Función para calcular el centroide de un triángulo
Vertex centroideTriangulo(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    return (v1 + v2 + v3) / 3.0f;
}

Vertex calculateFacePosition(const Face& face, const vector<Vertex>& vertices) {
    // Recuperar las posiciones de los vértices de la cara
    const Vertex& v1 = vertices[face.v1 - 1];
    const Vertex& v2 = vertices[face.v2 - 1];
    const Vertex& v3 = vertices[face.v3 - 1];

    // Calcular el centroide
    return centroideTriangulo(v1,v2,v3);
}

Vertex calcularCentroideFigura(const std::vector<Face>& faces) {
    Vertex centroideFinal;
    float sumaAreas = 0.0f;

    for (const auto& face : faces) {
        const Vertex& v1 = modelObjects.vertixes[face.v1 - 1];
        const Vertex& v2 = modelObjects.vertixes[face.v2 - 1];
        const Vertex& v3 = modelObjects.vertixes[face.v3 - 1];

        float area = areaTriangulo(v1, v2, v3);

        Vertex centroideTri = centroideTriangulo(v1, v2, v3);

        // Sumar el centroide ponderado por el área
        centroideFinal = centroideFinal + centroideTri * area;

        sumaAreas += area;
    }

    // Dividir por la suma total de las áreas para obtener el centroide ponderado
    if (sumaAreas > 0.0f) {
        centroideFinal = centroideFinal / sumaAreas;
    }

    return centroideFinal;
}

Vertex calculateNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    Vertex vector1 = v2 - v1;
    Vertex vector2 = v3 - v1;
    Vertex normal = vector1.crossProduct(vector2);

    return normal.normalize();
}

Vertex calculateFaceNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    //se calcula la normal para cada vertice
    Vertex n1 = calculateNormal(v1, v2, v3);

    Vertex n2 = calculateNormal(v2, v1, v3);

    Vertex n3 = calculateNormal(v3, v1, v2);

    //calcula promedio y se normaliza
    Vertex normalAvg = (n1 + n2 + n3) / 3.0f;
    return normalAvg.normalize();
}

void calculateVertexNormals() {
    // Inicializar todas las normales de vértices a cero
    modelObjects.normals.resize(modelObjects.vertixes.size(), { 0, 0, 0 });
    vector<int> normalCounts(modelObjects.vertixes.size(), 0);

    // Recorrer todas las caras del modelo
    for (const auto& object : modelObjects.objects) {
        for (const auto& face : object.faces) {
            int index1 = face.v1 - 1;
            int index2 = face.v2 - 1;
            int index3 = face.v3 - 1;

            // Obtener los vértices correspondientes
            Vertex v1 = modelObjects.vertixes[index1];
            Vertex v2 = modelObjects.vertixes[index2];
            Vertex v3 = modelObjects.vertixes[index3];
            
            Vertex normal = calculateNormal(v1, v2, v3);

            // Acumular la normal de la cara en cada vértice de la cara
            modelObjects.normals[index1] = modelObjects.normals[index1] + normal;
            modelObjects.normals[index2] = modelObjects.normals[index2] + normal;
            modelObjects.normals[index3] = modelObjects.normals[index3] + normal;
            
            normalCounts[index1]++;
            normalCounts[index2]++;
            normalCounts[index3]++;
        }
    }

    // Promediar las normales acumuladas y normalizarlas
    for (size_t i = 0; i < modelObjects.normals.size(); ++i) {
        if (normalCounts[i] > 0) {
            modelObjects.normals[i] = modelObjects.normals[i] / static_cast<float>(normalCounts[i]);
        }
        modelObjects.normals[i].normalize();
    }
}

