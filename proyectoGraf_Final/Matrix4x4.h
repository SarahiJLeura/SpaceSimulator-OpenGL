#pragma once
#include "Vertex.h"
#include <iostream>
#include <cmath>

class Matrix4x4
{
private:
    float m[4][4];
public:
    Matrix4x4();
    Matrix4x4(int x);

    Matrix4x4 operator*(const Matrix4x4& other) const;
    Vertex multiplyVertex(const Vertex& v) const;
    static Matrix4x4 translation(float x, float y, float z);
    static Matrix4x4 rotationX(float angleDegrees);
    static Matrix4x4 rotationY(float angleDegrees);
    static Matrix4x4 rotationZ(float angleDegrees);
    static Matrix4x4 scale(float x);
    static Matrix4x4 lookAtMatrix(const Vertex& eye, const Vertex& target, const Vertex& up);
    static Matrix4x4 perspectiveMatrix(float fov, float znear, float zfar);
    Matrix4x4 transpose() const;
    Matrix4x4 inverseAffine() const;

    void print() const;
};

