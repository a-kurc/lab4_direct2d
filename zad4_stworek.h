#pragma once

#ifndef ZAD3ZEGAR_H
#define ZAD3ZEGAR_H

#include <windows.h>
#include <d2d1_3.h>
#include <d2d1.h>
#include <math.h>
#include <stdio.h>
#include <windowsx.h>
#include <cmath>
#include <cstdlib>
#include <array>
#include <wincodec.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <dwrite.h>
#include <cstdio>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs")
#pragma comment(lib, "Dwrite")

using D2D1::RectF;
using D2D1::BitmapProperties;
using D2D1::PixelFormat;
using D2D1::RenderTargetProperties;
using D2D1::HwndRenderTargetProperties;
using D2D1::SizeU;
using D2D1::Point2F;
using D2D1::StrokeStyleProperties;
using D2D1::ColorF;
using D2D1::Matrix3x2F;
using D2D1::RectF;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID2D1Bitmap* load_bitmap(HWND hwnd, HRESULT hr, const LPCWSTR name, ID2D1Bitmap* lbitmap, IWICImagingFactory* pWICFactory);

int load_bitmaps(HWND hwnd);

void draw_digit(int digit, int position, D2D1_SIZE_F size);

int random_digit_in_range(int range);

void draw_dots(D2D1_SIZE_F size);

void increase_time();


namespace globals
{
	ID2D1Factory7* d2d_factory = nullptr;
	ID2D1HwndRenderTarget* d2d_render_target = nullptr;
	RECT rc{};
	bool rc_initiated_already = false;
	float half_y;
	float half_x;
	float initial_width = 0;
	float initial_higth = 0;
	ID2D1Bitmap* watch_bitmap = nullptr;
	ID2D1Bitmap* digits_bitmap = nullptr;
	IWICImagingFactory* pWICFactory = NULL;
	int time_counter = 0;
	int first_digit = 0;
	int second_digit = 0;
	int third_digit = 0;
	int fourth_digit = 0;
}

#endif