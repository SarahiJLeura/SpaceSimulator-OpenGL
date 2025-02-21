#pragma once

#include "Vertex.h"
#include <cmath>
#include <iostream>

class BezierCurve
{
private:
	Vertex p0, p1, p2, p3;
	float angle, velocity, rangeX, maximumHeight, force, weight;
	float totalRange;
	float initAngle, initForce;
public:
	float t = 0.0f;

	BezierCurve(Vertex pa, Vertex pb, Vertex pc, Vertex pd);
	BezierCurve(float f, float w, float a);
	void CalculateValues();
	bool updateValues();
	Vertex CalculateBezierCurve();
	float calculateVelocity();
	void assignControlPoints();
	void resetValues();
};

