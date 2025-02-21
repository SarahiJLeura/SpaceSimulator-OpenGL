#pragma once

#include <vector>
#include "Vertex.h"
#include "Matrix4x4.h"

using namespace std;

class LightManagement
{
private:
	struct Light {
		bool isOn;
		Vertex position;
		Vertex color;
	};

	struct AmbientLight {
		Vertex color;
		float intensity;
	};

	vector<Light> lightSources;
	AmbientLight ambientLight;

public:
	LightManagement(Vertex ambientColor, float ambientIntensity) {
		ambientLight.color = ambientColor;
		ambientLight.intensity = ambientIntensity;
	}
	void initLights();
	Vertex calculateLighting(const Vertex& normal, const Vertex& position, const Matrix4x4& world);
	
	bool getStateLight(int index) { return lightSources[index].isOn; }
	void updateStateLight(int index);
};

