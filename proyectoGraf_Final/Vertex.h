#pragma once
class Vertex
{
public:
	float x, y, z;

	Vertex() : x(0.0f), y(0.0f), z(0.0f) {}
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

	Vertex normalize() const;
	float dotProduct(const Vertex& other) const;
	Vertex crossProduct(const Vertex& other) const;
	Vertex operator-(const Vertex& other) const;
	Vertex operator+(const Vertex& other) const;
	Vertex operator*(const Vertex& other) const;
	Vertex operator*(float scalar) const;
	Vertex operator/(float scalar) const;
};

