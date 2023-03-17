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

#include <pthread.h>

#include "Sphere.h"

//[comment]
// This variable controls the maximum recursion depth
//[/comment]
#define MAX_RAY_DEPTH 5

class Tracer
{
public:
	void BasicRender();
	void SimpleShrinking(bool bUseThreading = false);
	void SmoothScaling();
	
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