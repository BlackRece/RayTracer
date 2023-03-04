// [header]
// A very basic raytracer example.
// [/header]
// [compile]
// c++ -o raytracer -O3 -Wall raytracer.cpp
// [/compile]
// [ignore]
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// [/ignore]
#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#include "RayTracer/Tracer.h"
//#include "RayTracer/Vec3.h"
//#include "RayTracer/Sphere.h"

#include "MemoryManager\MemoryManager.h"
// Windows only
#include <algorithm>
#include <sstream>
#include <string.h>

//#if defined __linux__ || defined __APPLE__
//// "Compiled for Linux
//#else
//// Windows doesn't define these values by default, Linux does
//#define M_PI 3.141592653589793
//#define INFINITY 1e8
//#endif

float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

//[comment]
// In the main function, we will create the scene which is composed of 5 spheres
// and 1 light (which is also a sphere). Then, once the scene description is complete
// we render that scene, by calling the render() function.
//[/comment]
int main(int argc, char **argv)
{
	MemoryManager::init();
	MemoryManager::enablePooling(false);

	auto tracer = new Tracer();
	
	// This sample only allows one choice per program execution. Feel free to improve upon this
	srand(13);
	//BasicRender();
	//SimpleShrinking();
	tracer->SmoothScaling();

	return 0;
}

