#include "Vertex.h"
#include <cmath>

// Método para normalizar el vector
Vertex Vertex::normalize() const
{
    float magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude == 0) return Vertex(0, 0, 0);
    return Vertex(x / magnitude, y / magnitude, z / magnitude);
}

Vertex Vertex::crossProduct(const Vertex& other) const
{
    return Vertex(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Vertex::dotProduct(const Vertex& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vertex Vertex::operator-(const Vertex& other) const
{
    return Vertex(x - other.x, y - other.y, z - other.z);
}

Vertex Vertex::operator+(const Vertex& other) const
{
    return Vertex(x + other.x, y + other.y, z + other.z);
}

Vertex Vertex::operator*(const Vertex& other) const
{
    return Vertex(x * other.x, y * other.y, z * other.z);
}

Vertex Vertex::operator*(float scalar) const
{
    return Vertex(x * scalar, y * scalar, z * scalar);
}

Vertex Vertex::operator/(float scalar) const
{
    if (scalar == 0) return Vertex(0, 0, 0);
    return Vertex(x / scalar, y / scalar, z / scalar);
}
