#include "BezierCurve.h"

#define M_PI 3.14159265358979323846
#define GRAVITY 9.80665

BezierCurve::BezierCurve(Vertex pa, Vertex pb, Vertex pc, Vertex pd) {
	p0 = pa;
	p1 = pb;
	p2 = pc;
	p3 = pd;
}

BezierCurve::BezierCurve(float f, float w, float a) {
	initForce = force = f;
	weight = w;
	initAngle = angle = a * (M_PI / 180.0f);
	velocity = rangeX = maximumHeight = totalRange = 0.0f;
}

float BezierCurve::calculateVelocity() {
	return (force / weight);
}

void BezierCurve::CalculateValues() {
	velocity = calculateVelocity();
	maximumHeight = (pow(velocity, 2) * pow(sin(angle), 2)) / (2 * GRAVITY);
	rangeX = pow(velocity, 2) * sin(2 * angle) / GRAVITY;
	assignControlPoints();
	//se va almacenando la distancia en X, porque se tiene que ir recorriendo las curvas
	totalRange += rangeX;
}

// calcular los 4 puntos de control teniendo en cuenta a la altura maxima y la distancia en X
void BezierCurve::assignControlPoints() {
	p0 = Vertex{ totalRange, 0.0f, 0.0f };
	p1 = Vertex{ totalRange + rangeX / 4, maximumHeight / 2, 0.0f };
	p2 = Vertex{ totalRange + rangeX * 3 / 4, maximumHeight, 0.0f };
	p3 = Vertex{ totalRange + rangeX, 0.0f, 0.0f };

}

void BezierCurve::resetValues() {
	force = initForce;
	angle = initAngle;
	totalRange = 0;
}

// se actualizan los valores cuando la piedra toca el piso, regresa si esta botando o el objeto
bool BezierCurve::updateValues() {
	bool flag = false;
	if (angle < 65 * (M_PI / 180.0f)) {
		if (rangeX <= 0.5 || maximumHeight <= 0.5) {
			resetValues();
		}
		else {
			angle *= 0.7;
			force *= 0.8;
			flag = true;
		}
	}
	else {
		if (force <= 6.5f) {
			resetValues();
		}
		else {
			force *= 0.5;
			flag = true;
		}
	}
	CalculateValues();
	return flag;
}

//calular los puntos de la curva cubica de bezier
Vertex BezierCurve::CalculateBezierCurve() {
	Vertex result;
	result.x = pow((1 - t), 3) * p0.x + 3 * t * pow((1 - t), 2) * p1.x + 3 * pow(t, 2) * (1 - t) * p2.x + pow(t, 3) * p3.x;
	result.y = pow((1 - t), 3) * p0.y + 3 * t * pow((1 - t), 2) * p1.y + 3 * pow(t, 2) * (1 - t) * p2.y + pow(t, 3) * p3.y;
	result.z = pow((1 - t), 3) * p0.z + 3 * t * pow((1 - t), 2) * p1.z + 3 * pow(t, 2) * (1 - t) * p2.z + pow(t, 3) * p3.z;
	return result;
}
