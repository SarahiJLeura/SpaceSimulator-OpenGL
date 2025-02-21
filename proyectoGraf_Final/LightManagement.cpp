#include "LightManagement.h"

void LightManagement::initLights() {
    Light light1 = { true, { 0.0f, 5.0f, 5.0f }, { 1.0f, 1.0f, 1.0f } };  // Luz arriba
    Light light2 = { true, { 3.0f, -3.0f, -5.0f }, { 1.0f, 1.0f, 1.0f } }; // Luz abajo
    Light light3 = { true, { -3.0f, -3.0f, -5.0f }, { 1.0f, 1.0f, 1.0f } };  // Luz derecha
    Light light4 = { true, {-2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } };  // Luz izquierda
    lightSources.push_back(light1);
    lightSources.push_back(light2);
    lightSources.push_back(light3);
}

Vertex LightManagement::calculateLighting(const Vertex& normal, const Vertex& position, const Matrix4x4& worldToLocal) {
    Vertex color, diffuse;

    Vertex ambient = ambientLight.color * ambientLight.intensity;

    // Luz difusa
    for (const Light& light : lightSources) {
        if (light.isOn) {
            Vertex lightDir = (light.position - position);

            Vertex localLight = worldToLocal.multiplyVertex(lightDir).normalize();
            Vertex localNormal = worldToLocal.multiplyVertex(normal).normalize();

            float diffuseFactor = max(0.0f, localNormal.dotProduct(localLight));

            diffuse = light.color * diffuseFactor;
            color = color + diffuse;
        }
    }
    color = color + ambient;

    color.x = min(1.0f, color.x);
    color.y = min(1.0f, color.y);
    color.z = min(1.0f, color.z);

    return color;
}

void LightManagement::updateStateLight(int index) {
    lightSources[index].isOn = !lightSources[index].isOn;
}