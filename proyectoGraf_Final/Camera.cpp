#include "Camera.h"

#include <GL/freeglut.h>
#include <windows.h> 
#include <cmath>

#define M_PI 3.14159265358979323846

Camera::Camera(Vertex pos) {
    position = pos;
    target = { 0.0f, 0.0f, -1.0f };
    up = { 0.0f, 1.0f, 0.0f };
    worldUp = up;
    Pitch = 0.0f;
    Yaw = -90.0f;

    speed = 0.3f;

    updateRightVector();
}

void Camera::updateRightVector() {
    right = target.crossProduct(up).normalize();
}

void Camera::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

float radians(float degrees) {
    return degrees * (M_PI / 180);
}

void Camera::updateCameraVectors()
{
    // Calcular el nuevo vector de destino (target)
    Vertex newTarget;
    newTarget.x = cos(radians(Yaw)) * cos(radians(Pitch));
    newTarget.y = sin(radians(Pitch));
    newTarget.z = sin(radians(Yaw)) * cos(radians(Pitch));
    target = newTarget.normalize();

    // Recalcular el vector Right y Up
    right = target.crossProduct(worldUp).normalize();  // Vector a la derecha (horizontal)
    up = right.crossProduct(target).normalize();
}

void Camera::moveForward() {
    position = position + (target * speed);
}

void Camera::moveBackward() {
    position = position - (target * speed);
}

void Camera::moveRight() {
    position = position + (right * speed);
}

void Camera::moveLeft() {
    position = position - (right * speed);
}

void Camera::moveUp() {
    position.y -= speed;
}

void Camera::moveDown() {
    position.y += speed;
}

void Camera::rotateX(float angle) {
    //rotation.x = (rotation.x += angle);

    float radAngle = radians(angle);

    Vertex tar = target * cos(radAngle) + up * sin(radAngle);

    target = tar.normalize();
    up = target.crossProduct(right) * -1;
}

void Camera::rotateY(float angle) {
    //rotation.y = resetRotation(rotation.y += angle);

    float radAngle = radians(angle);

    Vertex tar = target * cos(radAngle) - right * sin(radAngle);

    target = tar.normalize();
    updateRightVector();
}

void Camera::apply() {
	gluLookAt(position.x, position.y, position.z, position.x + target.x, position.y + target.y, position.y + target.z, up.x, up.y, up.z);
}