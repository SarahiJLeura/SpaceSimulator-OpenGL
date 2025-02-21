#include <GL/freeglut.h>
#include <chrono>
#include <windows.h> 
#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Camera.h"
#include "Face.h"
#include "Vertex.h"
#include "Matrix4x4.h"
#include "BezierCurve.h"
#include "MathUtils.h"
#include "Model.h"
#include "LightManagement.h"

#define M_PI 3.14159265358979323846

int frameCount = 0;
double fps = 0.0;
std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

using namespace std;

struct Key {
    int keyCode;
    bool isPressed;
};
/* Lighting management */
const int KEY_COUNT_LIGHT = 3;
Key keysLights[KEY_COUNT_LIGHT]{
    {0x31, false}, // '1'
    {0x32, false}, // '2'
    {0x33, false} // '3'
};

/* Rotate objects in the world */
const int KEY_COUNT_ROT = 3;
Key keysRotation[KEY_COUNT_ROT]{
    {0x54, false}, // 'T'
    {0x59, false}, // 'Y'
    {0x55, false} // 'U'
};

/* Camera Movements */
const int KEY_COUNT = 10;
Key keys[KEY_COUNT] = {
    {0x41, false}, // 'A'
    {0x53, false}, // 'S'
    {0x44, false}, // 'D'
    {0x57, false}, // 'W'
    {0x52, false}, // 'R'
    {0x46, false}, // 'F'
    {0x51, false}, // 'Q'
    {0x45, false}, // 'E'
    {0x5A, false}, // 'Z'
    {0x58, false} // 'X'
};

float keyX = 0.0f;
float keyY = 0.0f;
float keyZ = 0.0f;

Model modelObjects;
Camera cameralookAt = { {0.0f, 0.0f, 15.0f} };
vector<Vertex> Modelcolors;

vector<BezierCurve> allBeziers;

bool isFlatShading = false;
bool noShading = true;

LightManagement lightingWorld = { {0.8f, 0.8f, 0.8f}, 0.2 };

bool updateKey(bool& key) {
    key = !key;
    return key;
}

void updateLights() {
    static bool prevState[KEY_COUNT_LIGHT] = { false };

    for (int i = 0; i < KEY_COUNT_LIGHT; ++i) {
        bool isPressed = GetAsyncKeyState(keysLights[i].keyCode) & 0x8000;

        if (isPressed && !prevState[i]) {
            lightingWorld.updateStateLight(i);
        }
        prevState[i] = isPressed;
    }
}

void updateShading() {
    static bool prevState = false;
    static int numShading = 0;
    bool isPressed = GetAsyncKeyState(0x49) & 0x8000; // 'I'
    if (isPressed && !prevState) {
        int type = (numShading++) % 3;
        switch (type) {
        case 0:
            isFlatShading = true; noShading = false;
            break;
        case 1:
            isFlatShading = false;
            break;
        case 2:
            noShading = true;
            break;
        }
    }
    prevState = isPressed;
}

void updateCameraPosition() {
    if (keys[0].isPressed) cameralookAt.moveLeft();
    if (keys[2].isPressed) cameralookAt.moveRight();

    if (keys[1].isPressed) cameralookAt.moveBackward();
    if (keys[3].isPressed) cameralookAt.moveForward();

    if (keys[4].isPressed) cameralookAt.moveDown();
    if (keys[5].isPressed) cameralookAt.moveUp();

    if (keys[6].isPressed) cameralookAt.rotateY(0.5);
    if (keys[7].isPressed) cameralookAt.rotateY(-0.5);

    if (keys[8].isPressed) cameralookAt.rotateX(0.5);
    if (keys[9].isPressed) cameralookAt.rotateX(-0.5);
}

void updateRotation() {
    static bool prevState[KEY_COUNT_ROT] = { false };
    for (int i = 0; i < KEY_COUNT_ROT; i++) {
        bool currentState = GetAsyncKeyState(keysRotation[i].keyCode) & 0x8000;

        if (currentState && !prevState[i]) {
            updateKey(keysRotation[i].isPressed);
        }
        prevState[i] = currentState;
    }
    if (keysRotation[0].isPressed) { keyX += 0.85; }
    if (keysRotation[1].isPressed) { keyY += 0.85; }
    if (keysRotation[2].isPressed) { keyZ += 0.85; }
}

void keyboardInput() {
    for (int i = 0; i < KEY_COUNT; ++i) {
        if (GetAsyncKeyState(keys[i].keyCode) & 0x8000) { // 0x8000  = keyPressed
            keys[i].isPressed = true;
        }
        else {
            keys[i].isPressed = false;
        }
    }
    updateCameraPosition();
    updateLights();
    updateShading();
}

// Función para renderizar texto en 2D (FPS)
void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

//calular los frames por segundo para el mainLoop
void calculateFPS() {
    using namespace std::chrono;

    frameCount++;
    steady_clock::time_point currentTime = steady_clock::now();
    double elapsedTime = duration_cast<duration<double>>(currentTime - startTime).count();

    if (elapsedTime >= 1.0) {
        fps = frameCount / elapsedTime;
        frameCount = 0;
        startTime = currentTime;
    }
}


void initBeziers(float force, float angle) {
    float weight = 2.0f;

    for (int i = 0; i < modelObjects.objects.size(); i++) {
        BezierCurve currentBezier = BezierCurve(force, weight, angle);
        allBeziers.push_back(currentBezier);
        allBeziers[i].CalculateValues();
    }
}

void displayKeyBoardMenu() {
    renderText(-0.9f, 0.8f, "CAMERA MOVEMENTS");
    renderText(-0.9f, 0.7f, "W = Forward");
    renderText(-0.9f, 0.6f, "S = Backward");
    renderText(-0.9f, 0.5f, "A = Left");
    renderText(-0.9f, 0.4f, "D = Right");
    renderText(-0.9f, 0.3f, "R = Up");
    renderText(-0.9f, 0.2f, "F = Down");
    renderText(-0.9f, 0.1f, "Q = Rotate Left");
    renderText(-0.9f, 0.0f, "E = Rotate Right");
    renderText(-0.9f, -0.1f, "Z = Rotate Up");
    renderText(-0.9f, -0.2f, "X = Rotate Down");
    renderText(-0.9f, -0.3f, "---OTHERS---");
    renderText(-0.9f, -0.4f, "I = Shading");
    renderText(-0.9f, -0.5f, "1, 2, 3 = Lights");
    renderText(-0.9f, -0.6f, "T = Rot Obj X");
    renderText(-0.9f, -0.7f, "Y = Rot Obj Y");
    renderText(-0.9f, -0.8f, "U = Rot Obj Z");
}

void applyTransformations(Model::itemStruct& obj, const Vertex& centroide,
    float tx, float ty, float tz,
    float angleX, float angleY, float angleZ, int index) {

    Matrix4x4 bezierTransform = Matrix4x4::Matrix4x4();

    // Definir posiciones iniciales únicas para cada objeto
    Vertex initialPositions[] = {
        {0.0f, -5.0f, 0.0f},
        {2.0f, 2.0f, 3.0f},
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.5f},
        {0.0f, 1.0f, 0.0f}
    };

    if (index == 2 || index == 3 || index == 1 || index == 4) {
        // Velocidades de rotación

        float planetRotationSpeed = 0.3f; // Velocidad de rotación del planeta
        float orbitRotationSpeed = 1.0f;  // Velocidad de la órbita de la luna

        if (index == 3) {
            // Parámetros de la órbita
            float orbitRadius = 0.0f; // Radio de la órbita
            angleY += orbitRotationSpeed; // Incremento para la órbita

            // Obtener la posición actual del objeto
            Vertex object5Position = allBeziers[5].CalculateBezierCurve();

            // Transformaciones para la órbita
            Matrix4x4 orbitRotation = Matrix4x4::rotationY(angleY); // Rotación orbital
            Matrix4x4 translationToOrbit = Matrix4x4::translation(orbitRadius, 0.0f, 0.0f);


            bezierTransform =
                Matrix4x4::translation(
                    object5Position.x,
                    object5Position.y,
                    object5Position.z
                ) * orbitRotation * translationToOrbit;
        }

        // Movimiento en la curva Bézier
        else if (index == 4 || index == 2) {
            Vertex bezierPosition = allBeziers[index].CalculateBezierCurve();
            allBeziers[index].t += 0.003f;
            if (allBeziers[index].t > 1.0f) allBeziers[index].t = 0.0f;

            // Transformación para mover el objeto a la curva Bézier
            bezierTransform = Matrix4x4::translation(
                bezierPosition.x + initialPositions[index].x - centroide.x,
                bezierPosition.y + initialPositions[index].y - centroide.y,
                bezierPosition.z + initialPositions[index].z - centroide.z
            );
        }

        // Movimiento de despegue
        else if (index == 1) {
            static float currentHeight = 0.0f;
            static float ascentSpeed = 0.01f;
            float maxHeight = 10.0f;
            float acceleration = 0.002f;

            ascentSpeed += acceleration;
            currentHeight += ascentSpeed;

            if (currentHeight > maxHeight) {
                currentHeight = 0.0f;
                ascentSpeed = 0.01f;
            }

            bezierTransform = Matrix4x4::translation(
                initialPositions[index].x - centroide.x,
                currentHeight + initialPositions[index].y - centroide.y,
                initialPositions[index].z - centroide.z
            );
        }
    }

    // Transformaciones generales (traslación y rotación global)
    Matrix4x4 translationToOrigin = Matrix4x4::translation(-centroide.x, -centroide.y, -centroide.z);
    Matrix4x4 rotationX = Matrix4x4::rotationX(angleX);
    Matrix4x4 rotationY = Matrix4x4::rotationY(angleY);
    Matrix4x4 rotationZ = Matrix4x4::rotationZ(angleZ);
    Matrix4x4 translationBack = Matrix4x4::translation(centroide.x, centroide.y, centroide.z);
    Matrix4x4 additionalTranslation = Matrix4x4::translation(tx, ty, tz);

    obj.modelMatrix = bezierTransform * additionalTranslation * translationBack * rotationZ * rotationY * rotationX * translationToOrigin;
}


void initColors() {
    for (int i = 0; i < modelObjects.objects.size(); i++) {
        Vertex currentColor;
        switch (i) {
        case 0:
            currentColor = { 0.2, 0.5, 0.3 };
            Modelcolors.push_back(currentColor);
            break;
        case 1:
            currentColor = { 0.25, 0.25, 0.25 };
            Modelcolors.push_back(currentColor);
            break;
        case 2:
            currentColor = { 0.8f, 0.5f, 0.2 };
            Modelcolors.push_back(currentColor);
            break;
        case 3:
            currentColor = { 0.7f, 0.7f, 0.7 };
            Modelcolors.push_back(currentColor);
            break;
        case 4:
            currentColor = { 0.94, 0.94, 0.94 };
            Modelcolors.push_back(currentColor);
            break;
        case 5:
            currentColor = { 0.13, 0.55, 0.13 };
            Modelcolors.push_back(currentColor);
            break;
        default:
            currentColor = { 0.95f, 0.56f, 0.64 };
            Modelcolors.push_back(currentColor);
            break;
        }
    }
}

float rtX, rtY, rtZ = 0.0f;

// Función para dibujar en la pantalla
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    keyboardInput();
    updateRotation();

    //crear matriz de proyeccion
    float fovFactor = 100 * M_PI / 180.0;
    float zNear = 0.1, zFar = 20.0;
    Matrix4x4 projectionMatrix = Matrix4x4::perspectiveMatrix(fovFactor, zNear, zFar);

    //crear lookat
    Vertex pos = cameralookAt.getPos();
    Vertex tar = pos + cameralookAt.getTarget();

    Vertex eye(pos.x, pos.y, pos.z);
    Vertex target(tar.x, tar.y, tar.z);
    Vertex up(0.0f, 1.0f, 0.0f);

    Matrix4x4 view = Matrix4x4::lookAtMatrix(eye, tar, up);

    // Dibujar el objeto
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < modelObjects.objects.size(); i++) {

        Model::itemStruct& obj = modelObjects.objects[i];

        Vertex centroide = calcularCentroideFigura(modelObjects.objects[i].faces);

        // Aplicar transformaciones adicionales 
        applyTransformations(modelObjects.objects[i], centroide, 0.0f, 0.0f, 0.0f, keyX + 0.0f, keyY + rtY, keyZ + 0.0f, i);

        Matrix4x4 finalM;

        finalM = projectionMatrix * view * obj.modelMatrix;
        Matrix4x4 inverseTranspose = finalM.inverseAffine().transpose();

        // Obtener color según índice
        float r, g, b;
        r = Modelcolors[i].x;
        g = Modelcolors[i].y;
        b = Modelcolors[i].z;

        for (int j = 0; j < modelObjects.objects[i].faces.size(); j++) {
            int index1 = modelObjects.objects[i].faces[j].v1 - 1;
            int index2 = modelObjects.objects[i].faces[j].v2 - 1;
            int index3 = modelObjects.objects[i].faces[j].v3 - 1;

            // Obtener los vértices correspondientes
            Vertex v1 = modelObjects.vertixes[index1];
            Vertex v2 = modelObjects.vertixes[index2];
            Vertex v3 = modelObjects.vertixes[index3];

            // Aplicar las transformaciones
            Vertex vt1 = finalM.multiplyVertex(v1);
            Vertex vt2 = finalM.multiplyVertex(v2);
            Vertex vt3 = finalM.multiplyVertex(v3);

            //Flat shading
            if (isFlatShading && !noShading) {

                Vertex faceNormal = calculateNormal(vt1, vt2, vt3);
                Vertex centerFace = centroideTriangulo(vt1, vt2, vt3);
                Vertex faceColor = lightingWorld.calculateLighting(faceNormal, centerFace, inverseTranspose);

                glColor3f(r * faceColor.x, g * faceColor.y, b * faceColor.z);
                glVertex3f(vt1.x, vt1.y, vt1.z);
                glVertex3f(vt2.x, vt2.y, vt2.z);
                glVertex3f(vt3.x, vt3.y, vt3.z);
            }
            else if (!isFlatShading && !noShading) {
                Vertex n1 = modelObjects.normals[index1];
                Vertex n2 = modelObjects.normals[index2];
                Vertex n3 = modelObjects.normals[index3];

                Vertex tn1 = inverseTranspose.multiplyVertex(n1);
                Vertex tn2 = inverseTranspose.multiplyVertex(n2);
                Vertex tn3 = inverseTranspose.multiplyVertex(n3);

                Vertex color1 = lightingWorld.calculateLighting(tn1, vt1, inverseTranspose);
                Vertex color2 = lightingWorld.calculateLighting(tn2, vt2, inverseTranspose);
                Vertex color3 = lightingWorld.calculateLighting(tn3, vt3, inverseTranspose);

                glColor3f(r * color1.x, g * color1.y, b * color1.z);
                glVertex3f(vt1.x, vt1.y, vt1.z);

                glColor3f(r * color2.x, g * color2.y, b * color2.z);
                glVertex3f(vt2.x, vt2.y, vt2.z);

                glColor3f(r * color3.x, g * color3.y, b * color3.z);
                glVertex3f(vt3.x, vt3.y, vt3.z);

            }
            else if (noShading) {
                glColor3f(r, g, b);
                glVertex3f(vt1.x, vt1.y, vt1.z);
                glVertex3f(vt2.x, vt2.y, vt2.z);
                glVertex3f(vt3.x, vt3.y, vt3.z);
            }
        }
    }
    glEnd();
    rtX += 0.85;
    rtY += 0.85;
    // Dibujar el texto de los FPS y numero de botes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);  // Modo ortogonal para dibujar en 2D
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // Restablecer la matriz de vista a la identidad

    glColor3f(1.0f, 1.0f, 1.0f);  // Color blanco
    //renderizar FPS
    stringstream ssFPS;
    ssFPS << "FPS: " << fps;
    renderText(-0.9f, 0.9f, ssFPS.str());

    displayKeyBoardMenu();

    // Volver a la proyección 3D original
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  // Reiniciar la proyección de nuevo
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);  // Volver a la proyección 3D
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
    glFlush();
}

void init(void) {
    /*  select clearing (background) color       */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    /*  initialize viewing values  */
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    */

    glEnable(GL_DEPTH_TEST);

}

/* Simula la funcion de glutMainLoop  */
void mainLoop() {
    using namespace std::chrono;

    while (true) {
        display();

        calculateFPS();

        glutMainLoopEvent();

        //control de FPS (aprox 60 FPS)
        std::this_thread::sleep_for(milliseconds(4));
    }
}

/*
 *  Declare initial window size, position, and display mode
 *  (single buffer and RGBA).  Open window with "hello"
 *  in its title bar.  Call initialization routines.
 *  Register callback function to display graphics.
 *  Enter main loop and process events.
 */

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simulation");

    init();
    //std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    string filename = "simulation5.obj";
    modelObjects.loadObject(filename);

    initBeziers(30, 60);
    initColors();
    lightingWorld.initLights();
    calculateVertexNormals();

    glutDisplayFunc(display);

    mainLoop();
    return 0;
}