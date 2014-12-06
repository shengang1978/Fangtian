#pragma once

#include <cv.h>

extern "C" __declspec(dllexport) int __stdcall HikNetApi_Init();
extern "C" __declspec(dllexport) const IplImage* __stdcall HikNetApi_GetImage();