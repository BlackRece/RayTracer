#pragma once
#ifndef TRACER_H
#define TRACER_H

#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#include "Sphere.h"

// Windows only
#include <algorithm>
#include <sstream>
#include <string.h>

#ifndef __linux__ || __APPLE__
#include <thread>
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#else

#endif // !__linux__ || __APPLE__

//[comment]
// This variable controls the maximum recursion depth
//[/comment]
#define MAX_RAY_DEPTH 5

class Tracer
{
public:
	void BasicRender();
	void SimpleShrinking(bool bUseThreading = false);
	void SmoothScaling(int sphereCount = 100);
	
private:
	Vec3f trace(
		const Vec3f& rayorig,
		const Vec3f& raydir,
		const std::vector<Sphere>& spheres,
		const int& depth
	);
	void render(const std::vector<Sphere>& spheres, int iteration);
	inline float mix(const float& a, const float& b, const float& mix) { return b * mix + a * (1 - mix); }
};

#endif // !TRACER_H