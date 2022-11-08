#ifndef UNICODE
#define UNICODE
#endif 

#include "zad4_stworek.h"
using namespace globals;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.hCursor = NULL;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GetClientRect(hwnd, &rc);
    ID2D1PathGeometry* path_mouth = nullptr;
    ID2D1GeometrySink* path_sink_mouth = nullptr;
    D2D1_POINT_2F ellipse_center_eye1 = {};
    D2D1_POINT_2F ellipse_center_eye2 = {};
    half_y = (rc.bottom - rc.top) / 2;
    half_x = (rc.right - rc.left) / 2;
   
    switch (uMsg)
    {
    case WM_CREATE:
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &d2d_factory);

        GetClientRect(hwnd, &rc);

        d2d_factory->CreateHwndRenderTarget(
            RenderTargetProperties(),
            HwndRenderTargetProperties(hwnd,
                SizeU(static_cast<UINT32>(rc.right) -
                    static_cast<UINT32>(rc.left),
                    static_cast<UINT32>(rc.bottom) -
                    static_cast<UINT32>(rc.top))),
            &d2d_render_target);

        ellipse_center_eye1.x = half_x - 118; 
        ellipse_center_eye1.y = half_y - 95; 
        ellipse_center_eye2.x = half_x + 118;
        ellipse_center_eye2.y = half_y - 95;
        
        ellipse_center_pupil1.x = ellipse_center_eye1.x - half_x;
        ellipse_center_pupil1.y = ellipse_center_eye1.y - half_y;
        ellipse_center_pupil2.x = ellipse_center_eye2.x - half_x;
        ellipse_center_pupil2.y = ellipse_center_eye2.y - half_y;

        if (!rc_initiated_already)
        {
            initial_higth = rc.bottom - rc.top;
            initial_width = rc.right - rc.left;
        }

        return 0;
    }
    case WM_DESTROY:
    {
        if (d2d_render_target) d2d_render_target->Release();
        if (d2d_factory) d2d_factory->Release();

        PostQuitMessage(0);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        ellipse_center_eye1.x = half_x - 118;
        ellipse_center_eye1.y = half_y - 95;
        ellipse_center_eye2.x = half_x + 118;
        ellipse_center_eye2.y = half_y - 95;

        ellipse_center_pupil1.x = ellipse_center_eye1.x;
        ellipse_center_pupil1.y = ellipse_center_eye1.y;
        ellipse_center_pupil2.x = ellipse_center_eye2.x;
        ellipse_center_pupil2.y = ellipse_center_eye2.y;
        if (radius_of_middleeye_squared >= pow((LOWORD(lParam) - ellipse_center_eye1.x), 2) 
            + pow((HIWORD(lParam) - ellipse_center_eye1.y), 2 )) 
        {
            ellipse_center_pupil1.x = LOWORD(lParam);
            ellipse_center_pupil1.y = HIWORD(lParam);
        }
        else 
        {
            // lewe oko d³ugoœæ du¿ego trójk¹ta w poziomie 
            float llen_big_tr_in_x = abs(LOWORD(lParam) - ellipse_center_eye1.x);
            // lewe oko d³ugoœæ du¿ego trójk¹ta w pionie 
            float llen_big_tr_in_y = abs(HIWORD(lParam) - ellipse_center_eye1.y);
            // preciwprostok¹tna du¿ego trójk¹ta
            float llen_big_tr_across = sqrt(pow(llen_big_tr_in_x, 2) + pow(llen_big_tr_in_y, 2));
            // lewe oko d³ugoœæ ma³ego trójk¹ta w poziomie 
            float llen_small_tr_in_x = (llen_big_tr_in_x * radius_of_middleeye) / llen_big_tr_across;
            // lewe oko d³ugoœæ ma³ego trójk¹ta w pionie 
            float llen_small_tr_in_y = (llen_big_tr_in_y * radius_of_middleeye) / llen_big_tr_across;
            
            // jeœli x punktu kursora jest na prawo od œrodka oka
            if (LOWORD(lParam) > ellipse_center_eye1.x)
                //œrodek ¿renicy idzie o llen_small_tr_in_x w prawo
                ellipse_center_pupil1.x = ellipse_center_eye1.x + llen_small_tr_in_x;
            else
                //œrodek ¿renicy idzie o llen_small_tr_in_x w lewo
                ellipse_center_pupil1.x = ellipse_center_eye1.x - llen_small_tr_in_x;

            // jeœli y punktu kursora jest wy¿ej od œrodka oka
            if (HIWORD(lParam) < ellipse_center_eye1.y)
                //œrodek ¿renicy idzie o llen_small_tr_in_y w górê
                ellipse_center_pupil1.y = ellipse_center_eye1.y - llen_small_tr_in_y; 
            else
                //œrodek ¿renicy idzie o llen_small_tr_in_y w dó³
                ellipse_center_pupil1.y = ellipse_center_eye1.y + llen_small_tr_in_y;
        }
        if (radius_of_middleeye_squared >= pow((LOWORD(lParam) - ellipse_center_eye2.x), 2) 
            + pow((HIWORD(lParam) - ellipse_center_eye2.y), 2))
        {
            ellipse_center_pupil2.x = LOWORD(lParam);
            ellipse_center_pupil2.y = HIWORD(lParam);
        }
        else
        {
            // prawe oko d³ugoœæ du¿ego trójk¹ta w poziomie 
            float llen_big_tr_in_x = abs(LOWORD(lParam) - ellipse_center_eye2.x);
            // prawe oko d³ugoœæ du¿ego trójk¹ta w pionie 
            float llen_big_tr_in_y = abs(HIWORD(lParam) - ellipse_center_eye2.y);
            // preciwprostok¹tna du¿ego trójk¹ta
            float llen_big_tr_across = sqrt(pow(llen_big_tr_in_x, 2) + pow(llen_big_tr_in_y, 2));
            // prawe oko d³ugoœæ ma³ego trójk¹ta w poziomie 
            float llen_small_tr_in_x = (llen_big_tr_in_x * radius_of_middleeye) / llen_big_tr_across;
            // prawe oko d³ugoœæ ma³ego trójk¹ta w pionie 
            float llen_small_tr_in_y = (llen_big_tr_in_y * radius_of_middleeye) / llen_big_tr_across;

            // jeœli x punktu kursora jest na prawo od œrodka oka
            if (LOWORD(lParam) > ellipse_center_eye2.x)
                //œrodek ¿renicy idzie o llen_small_tr_in_x w prawo
                ellipse_center_pupil2.x = ellipse_center_eye2.x + llen_small_tr_in_x;
            else
                //œrodek ¿renicy idzie o llen_small_tr_in_x w lewo
                ellipse_center_pupil2.x = ellipse_center_eye2.x - llen_small_tr_in_x;

            // jeœli y punktu kursora jest wy¿ej od œrodka oka
            if (HIWORD(lParam) < ellipse_center_eye2.y)
                //œrodek ¿renicy idzie o llen_small_tr_in_y w górê
                ellipse_center_pupil2.y = ellipse_center_eye2.y - llen_small_tr_in_y;
            else
                //œrodek ¿renicy idzie o llen_small_tr_in_y w dó³
                ellipse_center_pupil2.y = ellipse_center_eye2.y + llen_small_tr_in_y;
        }
        ellipse_center_pupil1.x -= half_x;
        ellipse_center_pupil1.y -= half_y;
        ellipse_center_pupil2.x -= half_x;
        ellipse_center_pupil2.y -= half_y;
        return 0;
    }
    case WM_PAINT:
    {
        Matrix3x2F scale = Matrix3x2F::Scale((initial_width)/(rc.right - rc.left), 
            ((initial_higth) / (rc.bottom - rc.top)), Point2F(0,0));
        d2d_render_target->SetTransform(scale);

        ellipse_center_eye1.x = half_x - 118;
        ellipse_center_eye1.y = half_y - 95;
        ellipse_center_eye2.x = half_x + 118;
        ellipse_center_eye2.y = half_y - 95;
        ellipse_center_pupil1.x += half_x;
        ellipse_center_pupil1.y += half_y;
        ellipse_center_pupil2.x += half_x;
        ellipse_center_pupil2.y += half_y;

        ID2D1SolidColorBrush* brush = nullptr;
        ID2D1SolidColorBrush* brush1 = nullptr;

        // - Interfejsy do obs³ugi œcie¿ki
        ID2D1PathGeometry* path = nullptr;
        ID2D1GeometrySink* path_sink = nullptr;
        ID2D1PathGeometry* path_nose = nullptr;
        ID2D1GeometrySink* path_sink_nose = nullptr;

        // Sta³e z kolorami
        D2D1_COLOR_F const brush_color_white =
        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        D2D1_COLOR_F const brush_color_black =
        { .r = 0, .g = 0, .b = 0, .a = 1 };
        // Kolory dla nosa
        D2D1_COLOR_F const brush1_color =
        { .r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1.0f };

        // Utworzenie pêdzli
        d2d_render_target->CreateSolidColorBrush(brush_color_black, &brush);
        d2d_render_target->CreateSolidColorBrush(brush1_color, &brush1);

         // Utworzenie i zbudowanie geometrii œcie¿ki (cia³a)
        d2d_factory->CreatePathGeometry(&path);
        path->Open(&path_sink);

        path_sink->BeginFigure(Point2F(half_x - 180, half_y - 100), D2D1_FIGURE_BEGIN_FILLED);
        path_sink->AddBezier(BezierSegment( Point2F(half_x - 400, half_y + 310), 
            Point2F(half_x - 200, half_y + 315), Point2F(half_x, half_y + 316)));
        path_sink->AddBezier(BezierSegment( Point2F(half_x + 200, half_y + 315), 
            Point2F(half_x + 400, half_y + 310), Point2F(half_x + 180, half_y - 100)));
        path_sink->AddBezier(BezierSegment( Point2F(half_x + 350, half_y - 200), 
            Point2F(half_x + 200, half_y - 370), Point2F(half_x + 100, half_y - 230)));
        path_sink->AddQuadraticBezier(QuadraticBezierSegment(
            Point2F(half_x, half_y - 300), Point2F(half_x - 100, half_y - 230)));
        path_sink->AddBezier(BezierSegment( Point2F(half_x - 200, half_y - 370), 
            Point2F(half_x - 350, half_y - 200), Point2F(half_x - 180, half_y - 100)));
        path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
        path_sink->Close();

        // Utworzenie i zbudowanie geometrii nosa
        d2d_factory->CreatePathGeometry(&path_nose);
        path_nose->Open(&path_sink_nose);
        path_sink_nose->BeginFigure(Point2F(half_x - 75, half_y + 50), D2D1_FIGURE_BEGIN_FILLED);
        path_sink_nose->AddBezier(BezierSegment( Point2F(half_x - 100, half_y - 10), 
            Point2F(half_x + 100, half_y - 10), Point2F(half_x + 75, half_y + 50)));
        path_sink_nose->AddQuadraticBezier(QuadraticBezierSegment(
            Point2F(half_x, half_y + 175), Point2F(half_x - 75, half_y + 50)));
        path_sink_nose->EndFigure(D2D1_FIGURE_END_OPEN);
        path_sink_nose->Close();

        // Utworzenie i zbudowanie geometrii ust
        d2d_factory->CreatePathGeometry(&path_mouth);
        path_mouth->Open(&path_sink_mouth);
        path_sink_mouth->BeginFigure(Point2F(half_x - 100, half_y + 160), D2D1_FIGURE_BEGIN_HOLLOW);
        if (GetAsyncKeyState(VK_LBUTTON) >= 0)
            path_sink_mouth->AddBezier(BezierSegment(
                Point2F(half_x - 70, half_y + 125), Point2F(half_x + 70, half_y + 125), 
                    Point2F(half_x + 100, half_y + 160)));
        else 
            path_sink_mouth->AddBezier(BezierSegment(
                Point2F(half_x - 70, half_y + 200), Point2F(half_x + 70, half_y + 200), 
                    Point2F(half_x + 100, half_y + 160)));
        path_sink_mouth->EndFigure(D2D1_FIGURE_END_OPEN);
        path_sink_mouth->Close();

        // - Pêdzel - gradient promienisty
        ID2D1RadialGradientBrush* rad_brush_eye1 = nullptr;
        ID2D1RadialGradientBrush* rad_brush_eye2 = nullptr;
        ID2D1RadialGradientBrush* rad_brush_body = nullptr;
        ID2D1GradientStopCollection* rad_stops_eye1 = nullptr;
        ID2D1GradientStopCollection* rad_stops_eye2 = nullptr;
        ID2D1GradientStopCollection* rad_stops_body = nullptr;
        UINT const NUM_RAD_STOPS_EYE = 2;
        UINT const NUM_RAD_STOPS_BODY = 5;
        D2D1_GRADIENT_STOP rad_stops_data_eye[NUM_RAD_STOPS_EYE];
        D2D1_GRADIENT_STOP rad_stops_data_body[NUM_RAD_STOPS_BODY];

        // Sta³e ustawienia geometrii
        D2D1_POINT_2F const center_body = { .x = half_x, .y = half_y };
        D2D1_POINT_2F const eye_ellipse_radii = { .x = r_eye, .y = r_eye };
        D2D1_POINT_2F const pupil_ellipse_radii = { .x = r_pupil, .y = r_pupil };

        rad_stops_data_eye[0] =
        { .position = 0.75f, .color = ColorF(1.0f, 1.0f, 1.0f, 1) };
        rad_stops_data_eye[1] =
        { .position = 1.0f, .color = ColorF(0.68f, 0.68f, 0.68f, 1) };

        rad_stops_data_body[0] =
        { .position = 0.1, .color = ColorF(1.0f, 0.9f, 1.0f, 1) };
        rad_stops_data_body[1] =
        { .position = 0.55f, .color = ColorF(0.0f, 0.87f, 0.0f, 1) };
        rad_stops_data_body[2] =
        { .position = 0.70f, .color = ColorF(0.0f, 0.75f, 0.0f, 1) };
        rad_stops_data_body[3] =
        { .position = 0.8f, .color = ColorF(0.0f, 0.6f, 0.0f, 1) };
        rad_stops_data_body[4] =
        { .position = 0.95f, .color = ColorF(0.0f, 0.45f, 0.0f, 1) };

        // Utworzenie gradientu promienistego
        d2d_render_target->CreateGradientStopCollection(
            rad_stops_data_eye, NUM_RAD_STOPS_EYE, &rad_stops_eye1);
        if (rad_stops_eye1) {
            d2d_render_target->CreateRadialGradientBrush(
                RadialGradientBrushProperties(ellipse_center_eye1,
                    Point2F(0, 0), 95, 95),
                rad_stops_eye1, &rad_brush_eye1);
        }
        d2d_render_target->CreateGradientStopCollection(
            rad_stops_data_eye, NUM_RAD_STOPS_EYE, &rad_stops_eye2);
        if (rad_stops_eye2) {
            d2d_render_target->CreateRadialGradientBrush(
                RadialGradientBrushProperties(ellipse_center_eye2,
                    Point2F(0, 0), 95, 95),
                rad_stops_eye2, &rad_brush_eye2);
        }
        d2d_render_target->CreateGradientStopCollection(
            rad_stops_data_body, NUM_RAD_STOPS_BODY, &rad_stops_body);
        if (rad_stops_body) {
            d2d_render_target->CreateRadialGradientBrush(
                RadialGradientBrushProperties(center_body,
                    Point2F(0, 70), 350, 350),
                rad_stops_body, &rad_brush_body);
        }
        
        // Rysowanie
        d2d_render_target->BeginDraw();
        d2d_render_target->Clear(brush_color_white);

        float brush_body_width = 5.0f;
        float brush_eye_width = 3.0f;
        float brush_mouth_width = 9.0f;

        // Cia³o z gradientem
        d2d_render_target->FillGeometry(path, rad_brush_body);
        d2d_render_target->DrawGeometry(path, brush, brush_body_width);

        // Oczy z gradientem
        d2d_render_target->FillEllipse(
            Ellipse(ellipse_center_eye1, eye_ellipse_radii.x, eye_ellipse_radii.y),
            rad_brush_eye1);
        d2d_render_target->DrawEllipse(
            Ellipse(ellipse_center_eye1, eye_ellipse_radii.x, eye_ellipse_radii.y),
            brush, brush_eye_width);

        d2d_render_target->FillEllipse(
            Ellipse(ellipse_center_eye2, eye_ellipse_radii.x, eye_ellipse_radii.y),
            rad_brush_eye2);
        d2d_render_target->DrawEllipse(
            Ellipse(ellipse_center_eye2, eye_ellipse_radii.x, eye_ellipse_radii.y),
            brush, brush_eye_width);

        // rednice
        d2d_render_target->FillEllipse(
            Ellipse(ellipse_center_pupil1, pupil_ellipse_radii.x, pupil_ellipse_radii.y),
            brush);
        d2d_render_target->DrawEllipse(
            Ellipse(ellipse_center_pupil1, pupil_ellipse_radii.x, pupil_ellipse_radii.y),
            brush, brush_eye_width);

        d2d_render_target->FillEllipse(
            Ellipse(ellipse_center_pupil2, pupil_ellipse_radii.x, pupil_ellipse_radii.y),
            brush);
        d2d_render_target->DrawEllipse(
            Ellipse(ellipse_center_pupil2, pupil_ellipse_radii.x, pupil_ellipse_radii.y),
            brush, brush_eye_width);

        // Nos i usta
        time += 0.03f;
        float angle = 15.0f * sin(time);
        // Zachowujemy macierz transofmacji
        D2D1_MATRIX_3X2_F transformation_to_save;
        d2d_render_target->GetTransform(&transformation_to_save);
        // Wprawiamy w ruch render_target (¿eby zrobiæ ruchome nos i usta)
        Matrix3x2F transformation;
        d2d_render_target->GetTransform(&transformation);
        Matrix3x2F rotate = Matrix3x2F::Rotation(angle, Point2F(half_x, half_y + 30));
        rotate.SetProduct(rotate, transformation);
        d2d_render_target->SetTransform(rotate);
        // Rysujemy nos i usta na ruchomym render_targer
        d2d_render_target->FillGeometry(path_nose, brush1);
        d2d_render_target->DrawGeometry(path_nose, brush, brush_eye_width);
        d2d_render_target->DrawGeometry(path_mouth, brush, brush_mouth_width);
        // Przywracamy render_target do stanu nieruchomego (¿eby nic poza nosem i ustami siê nie rusza³o)
        d2d_render_target->SetTransform(transformation_to_save);
       
        d2d_render_target->EndDraw();

        if (brush) brush->Release();
        if (brush1) brush1->Release(); 
        if (path) path->Release();
        if (path_nose) path_nose->Release();
        if (path_mouth) path_mouth->Release();
        if (path_sink) path_sink->Release();
        if (path_sink_nose) path_sink_nose->Release();
        if (path_sink_mouth) path_sink_mouth->Release();
        if (rad_brush_eye1) rad_brush_eye1->Release();
        if (rad_brush_eye2) rad_brush_eye2->Release();
        if (rad_brush_body) rad_brush_body->Release();
        if (rad_stops_eye1) rad_stops_eye1->Release();
        if (rad_stops_eye2) rad_stops_eye2->Release();
        if (rad_stops_body) rad_stops_body->Release();
    }
    ellipse_center_pupil1.x -= half_x;
    ellipse_center_pupil1.y -= half_y;
    ellipse_center_pupil2.x -= half_x;
    ellipse_center_pupil2.y -= half_y;
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}