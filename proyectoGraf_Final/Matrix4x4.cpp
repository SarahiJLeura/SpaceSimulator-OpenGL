#include "Matrix4x4.h"

#define M_PI 3.14159265358979323846

// Constructor por defecto: inicializa la matriz a la identidad
Matrix4x4::Matrix4x4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = (i == j) ? 1.0f : 0.0f;
}

//matriz inicializada en 0s
Matrix4x4::Matrix4x4(int x) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = 0.0f;
}

// Sobrecarga del operador * para multiplicar matrices
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 result(0);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

// Multiplicar un Vertex por la matriz
Vertex Matrix4x4::multiplyVertex(const Vertex& v) const {
    Vertex result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
    return result;
}

// Métodos para crear matrices de transformación
Matrix4x4 Matrix4x4::translation(float x, float y, float z) {
    Matrix4x4 result;
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

// Método estático para crear una matriz de rotación en el eje X
Matrix4x4 Matrix4x4::rotationX(float angleDegrees) {
    Matrix4x4 result;
    float radians = angleDegrees * (M_PI / 180.0f);
    result.m[1][1] = cos(radians);
    result.m[1][2] = -sin(radians);
    result.m[2][1] = sin(radians);
    result.m[2][2] = cos(radians);
    return result;
}

// Método estático para crear una matriz de rotación en el eje Y
Matrix4x4 Matrix4x4::rotationY(float angleDegrees) {
    Matrix4x4 result;
    float radians = angleDegrees * (M_PI / 180.0f);
    result.m[0][0] = cos(radians);
    result.m[0][2] = sin(radians);
    result.m[2][0] = -sin(radians);
    result.m[2][2] = cos(radians);
    return result;
}

// Método estático para crear una matriz de rotación en el eje Z
Matrix4x4 Matrix4x4::rotationZ(float angleDegrees) {
    Matrix4x4 result;
    float radians = angleDegrees * (M_PI / 180.0f);
    result.m[0][0] = cos(radians);
    result.m[0][1] = -sin(radians);
    result.m[1][0] = sin(radians);
    result.m[1][1] = cos(radians);
    return result;
}

Matrix4x4 Matrix4x4::scale(float x) {
    Matrix4x4 result;
    result.m[0][0] = x;
    result.m[1][1] = x;
    result.m[2][2] = x;
    return result;
}

Matrix4x4 Matrix4x4::lookAtMatrix(const Vertex& eye, const Vertex& target, const Vertex& up) {

    Vertex zAxis = (eye - target).normalize();
    Vertex xAxis = up.crossProduct(zAxis).normalize();
    Vertex yAxis = zAxis.crossProduct(xAxis);

    Matrix4x4 viewMatrix(0);

    viewMatrix.m[0][0] = xAxis.x;
    viewMatrix.m[0][1] = xAxis.y;
    viewMatrix.m[0][2] = xAxis.z;
    viewMatrix.m[0][3] = -xAxis.dotProduct(eye);

    viewMatrix.m[1][0] = yAxis.x;
    viewMatrix.m[1][1] = yAxis.y;
    viewMatrix.m[1][2] = yAxis.z;
    viewMatrix.m[1][3] = -yAxis.dotProduct(eye);

    viewMatrix.m[2][0] = zAxis.x;
    viewMatrix.m[2][1] = zAxis.y;
    viewMatrix.m[2][2] = zAxis.z;
    viewMatrix.m[2][3] = -zAxis.dotProduct(eye);

    viewMatrix.m[3][0] = 0.0f;
    viewMatrix.m[3][1] = 0.0f;
    viewMatrix.m[3][2] = 0.0f;
    viewMatrix.m[3][3] = 1.0f;

    return viewMatrix;

}

Matrix4x4 Matrix4x4::perspectiveMatrix(float fov, float znear, float zfar) {
    Matrix4x4 result(0);
    //int windowWidth = GetSystemMetrics(SM_CXSCREEN); // Ancho de pantalla
    //int windowHeight = GetSystemMetrics(SM_CYSCREEN); // altura de la pantalla
    //float aspect = windowHeight / static_cast<float>(windowWidth);
    float scale = 1.0 / tan(fov / 2);

    result.m[0][0] = scale;
    result.m[1][1] = scale;
    result.m[2][2] = zfar / (zfar - znear);
    result.m[2][3] = (-zfar * znear) / (zfar - znear);
    result.m[3][2] = 1.0;

    return result;
}

Matrix4x4 Matrix4x4::transpose()const {
    Matrix4x4 trans(0);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            trans.m[i][j] = m[j][i];
        }
    }
    return trans;
}

// Método para imprimir la matriz (para depuración)
void Matrix4x4::print() const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix4x4 Matrix4x4::inverseAffine() const {
    Matrix4x4 inv;

    // Copiar la submatriz 3x3 (rotación y escalado)
    float R[3][3] = {
        {m[0][0], m[0][1], m[0][2]},
        {m[1][0], m[1][1], m[1][2]},
        {m[2][0], m[2][1], m[2][2]}
    };

    // Calcular el determinante de la submatriz 3x3
    float det = R[0][0] * (R[1][1] * R[2][2] - R[1][2] * R[2][1]) -
        R[0][1] * (R[1][0] * R[2][2] - R[1][2] * R[2][0]) +
        R[0][2] * (R[1][0] * R[2][1] - R[1][1] * R[2][0]);

    if (fabs(det) < 1e-6f) {
        throw std::runtime_error("La matriz no es invertible.");
    }

    float invDet = 1.0f / det;

    // Calcular la matriz inversa de R (R_inv)
    float R_inv[3][3];
    R_inv[0][0] = (R[1][1] * R[2][2] - R[1][2] * R[2][1]) * invDet;
    R_inv[0][1] = (R[0][2] * R[2][1] - R[0][1] * R[2][2]) * invDet;
    R_inv[0][2] = (R[0][1] * R[1][2] - R[0][2] * R[1][1]) * invDet;

    R_inv[1][0] = (R[1][2] * R[2][0] - R[1][0] * R[2][2]) * invDet;
    R_inv[1][1] = (R[0][0] * R[2][2] - R[0][2] * R[2][0]) * invDet;
    R_inv[1][2] = (R[0][2] * R[1][0] - R[0][0] * R[1][2]) * invDet;

    R_inv[2][0] = (R[1][0] * R[2][1] - R[1][1] * R[2][0]) * invDet;
    R_inv[2][1] = (R[0][1] * R[2][0] - R[0][0] * R[2][1]) * invDet;
    R_inv[2][2] = (R[0][0] * R[1][1] - R[0][1] * R[1][0]) * invDet;

    // Asignar R_inv a la matriz inversa
    inv.m[0][0] = R_inv[0][0];
    inv.m[0][1] = R_inv[0][1];
    inv.m[0][2] = R_inv[0][2];
    inv.m[0][3] = 0.0f;

    inv.m[1][0] = R_inv[1][0];
    inv.m[1][1] = R_inv[1][1];
    inv.m[1][2] = R_inv[1][2];
    inv.m[1][3] = 0.0f;

    inv.m[2][0] = R_inv[2][0];
    inv.m[2][1] = R_inv[2][1];
    inv.m[2][2] = R_inv[2][2];
    inv.m[2][3] = 0.0f;

    // Calcular la traslación inversa
    inv.m[3][0] = -(R_inv[0][0] * m[3][0] + R_inv[0][1] * m[3][1] + R_inv[0][2] * m[3][2]);
    inv.m[3][1] = -(R_inv[1][0] * m[3][0] + R_inv[1][1] * m[3][1] + R_inv[1][2] * m[3][2]);
    inv.m[3][2] = -(R_inv[2][0] * m[3][0] + R_inv[2][1] * m[3][1] + R_inv[2][2] * m[3][2]);
    inv.m[3][3] = 1.0f;

    return inv;
}