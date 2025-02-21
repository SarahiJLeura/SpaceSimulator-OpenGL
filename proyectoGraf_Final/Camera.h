#pragma once

#include "Vertex.h"
#include "Matrix4x4.h"

class Camera
{
public:
    Camera(Vertex pos);
    void setPosition(float x, float y, float z);
	void apply();
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void updateCameraVectors();
    void updateRightVector();
    void rotateX(float angle);
    void rotateY(float angle);

    Vertex getPos() { return position; }
    Vertex getTarget() { return target; }
    Vertex getUp() { return up; }

private:
    Vertex position;
    Vertex target;
    Vertex up;
    Vertex right;
    Vertex rotation;
    Vertex worldUp;

    float speed;
    float Yaw, Pitch;
};

