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
#include <chrono>

#include "RayTracer/Tracer.h"
#include "RayTracer/JsonLoader.h"
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

enum RenderMode { 
	None	= -1,
	Basic	= 0,
	Simple	= 1,
	Smooth	= 2
};

struct SettingsJSON
{
	RenderMode eMode = RenderMode::None;
	
	// Use threading in SimpleShrinking
	bool bUseThreading;
	
	// Number of spheres in SmoothScaling
	int iSphereCount;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SettingsJSON, eMode, bUseThreading, iSphereCount);

RenderMode _eMode;
bool _bUseThreading;
SettingsJSON _settings;

float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

static void ShowUsage(std::string name) {
	std::cout 
		<< "\nUsage: " << name << " [options]" 
		<< "\n/ or ?:\tdisplays this information."
		<< "\n--json:\tload setttings from JSON file (.json)" 
		<< "\n--basic:\tBasicRender"
		<< "\n--simple:\tSimpleShrinking" 
		<< "\n--smooth:\tSmoothScaling (default)" 
		<< "\n--thread:\tUse Multi-Threading"
		<< "\n\nExample:\n\t"
		<< name << " --json settings.json --basic" 
		<< std::endl;
}

bool ParseArgs(int argc, char** argv)
{
	std::string sFile;

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] == "/" ||
			argv[i] == "?")
			return false;
		
		if (argv[i] == "--json")
		{
			sFile = argv[i + 1];
			if (sFile.find_last_of(".json") < 1)
				return false;
			
			JsonLoading::LoadJson(_settings, sFile);
			_eMode = _settings.eMode;
			_bUseThreading = _settings.bUseThreading;
			
			if (_settings.eMode != RenderMode::None)
				return true;
		}

		if (argv[i] == "--basic")
			_eMode = RenderMode::Basic;

		if (argv[i] == "--simple")
			_eMode = RenderMode::Simple;

		if (argv[i] == "--smooth")
			_eMode = RenderMode::Smooth;

		_bUseThreading = argv[i] == "--thread" ? true : false;
	}

	return true;
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

	_eMode = RenderMode::Smooth;
	_bUseThreading = false;
	
	if (argc < 2) {
		std::cout
			<< "\nNo settings JSON specified." 
			<< "\nDefault to using Smooth Scaling method."
			<< std::endl;
	}
	
	if (!ParseArgs(argc, argv)) 
	{
		ShowUsage(argv[0]);
		return 1;
	}

	auto tracer = new Tracer();
	int iSphereCount = _settings.eMode != RenderMode::None
		? _settings.iSphereCount
		: 100;
	
	// This sample only allows one choice per program execution. Feel free to improve upon this
	srand(13);
	
	// Testing
	_bUseThreading = true;
	_eMode = RenderMode::Simple;
	
	std::string sMode;
	auto iStart = std::chrono::high_resolution_clock::now();

	switch (_eMode)
	{
		case Basic: 
			sMode = "BasicRender";
			tracer->BasicRender(); 
			break;
		case Simple: 
			sMode = "SimpleShrinking";
			tracer->SimpleShrinking(_bUseThreading); 
			break;
		case Smooth:
			sMode = "SmoothScaling";
			
			tracer->SmoothScaling(iSphereCount);
			break;
	}

	auto iStop = std::chrono::high_resolution_clock::now();
	std::string sThreading = _bUseThreading ? " (Multi-Threaded)" : " (Single-Threaded)";
	std::cout << std::endl
		<< "\n/-----------------\\"
		<< "\nRendering Complete!"
		<< "\n\\-----------------/"
		<< "\n\nRender Mode: " << sMode 
		<< "\nTime taken: " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(iStop - iStart).count() << "ms" 
		<< "\nThreading Mode:" << sThreading 
		<< std::endl << std::endl;

	_settings = { _eMode, _bUseThreading, iSphereCount};
	JsonLoading::SaveJson(_settings, "LastUsedSettings.json");

	return 0;
}

