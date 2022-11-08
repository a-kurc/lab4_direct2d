#pragma once

#ifndef ZAD4STWOREK_H
#define ZAD4STWOREK_H

#include <windows.h>
#include <d2d1_3.h>
#include <d2d1.h>
#include <math.h>
#include <stdio.h>
#include <windowsx.h>
#include <cmath>
#include <cstdlib>
#pragma comment(lib, "d2d1")

using D2D1::RenderTargetProperties;
using D2D1::HwndRenderTargetProperties;
using D2D1::SizeU;
using D2D1::Point2F;
using D2D1::StrokeStyleProperties;
using D2D1::ColorF;
using D2D1::BezierSegment;
using D2D1::QuadraticBezierSegment;
using D2D1::Matrix3x2F;
using D2D1::LinearGradientBrushProperties;
using D2D1::RadialGradientBrushProperties;
using D2D1::RectF;
using D2D1::Ellipse;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace globals
{
	ID2D1Factory7* d2d_factory = nullptr;
	ID2D1HwndRenderTarget* d2d_render_target = nullptr;
	RECT rc{};
	bool rc_initiated_already = false;
	float half_y;
	float half_x;
	float r_eye = 95;
	float r_pupil = 35;
	D2D1_POINT_2F ellipse_center_pupil1 = { 0, 0 };
	D2D1_POINT_2F ellipse_center_pupil2 = { 0, 0 };
	float radius_of_middleeye_squared = pow((r_eye - r_pupil), 2);
	float radius_of_middleeye = r_eye - r_pupil;
	float initial_width = 0;
	float initial_higth = 0;
	float time = 0;
}

#endif