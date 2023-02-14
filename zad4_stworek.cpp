#ifndef UNICODE
#define UNICODE
#endif 


#include "zad4_stworek.h"
using namespace globals;
ID2D1Bitmap* watch_bitmap = nullptr;
ID2D1Bitmap* digits_bitmap = nullptr;
IWICImagingFactory* pWICFactory = NULL;
#define TIMER_ID 1
int time_counter = 0;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID2D1Bitmap* load_bitmap(HWND hwnd, HRESULT hr, const LPCWSTR name, ID2D1Bitmap* lbitmap, IWICImagingFactory* pWICFactory)
{
    // Load bitmap from file
    IWICBitmapDecoder* pDecoder = NULL;
    
    hr = pWICFactory->CreateDecoderFromFilename(name, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error loading bitmap from file.", L"Error", MB_ICONERROR);
    }

    IWICBitmapFrameDecode* pFrame = NULL;

    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error getting frame from bitmap decoder.", L"Error", MB_ICONERROR);
    }
    IWICFormatConverter* pConverter = NULL;
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error creating format converter.", L"Error", MB_ICONERROR);
    }
    hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error initializing format converter.", L"Error", MB_ICONERROR);
    }

    hr = d2d_render_target->CreateBitmapFromWicBitmap(pConverter, NULL, &lbitmap);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error creating Direct2D bitmap from WIC bitmap.", L"Error", MB_ICONERROR);
    }

    d2d_render_target->SetDpi(96.0f, 96.0f);

    return lbitmap;
}

int load_bitmaps(HWND hwnd)
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Error creating WIC imaging factory.", L"Error", MB_ICONERROR);
        return -1;
    }

    LPCWSTR name = L"Watch.png";
    watch_bitmap = load_bitmap(hwnd, hr, name, watch_bitmap, pWICFactory);

    name = L"Digits.png";
    digits_bitmap = load_bitmap(hwnd, hr, name, digits_bitmap, pWICFactory);

    /*name = L"saturn_pink_3.png";
    planet_saturn_pink.bitmap = load_bitmap(hwnd, hr, name, planet_saturn_pink.bitmap, pWICFactory);

    name = L"clouds1.png";
    clouds.bitmap = load_bitmap(hwnd, hr, name, clouds.bitmap, pWICFactory);*/
    return 0;
}

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


void draw_digit(int digit, int position, D2D1_SIZE_F size)
{
    int offset_x = 126;
    int offset_y = 102;

    // Create a new bitmap to hold the cropped portion of the original bitmap
    ID2D1Bitmap* pCroppedBitmap;
    d2d_render_target->CreateBitmap(
        D2D1::SizeU(108, 192),
        NULL,
        0,
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &pCroppedBitmap
    );

    // Copy the portion of the original bitmap that you want to crop
    D2D1_RECT_U sourceRect = D2D1::RectU(0 + digit*108, 0, 108 + digit*108, 192);
    pCroppedBitmap->CopyFromBitmap(NULL, digits_bitmap, &sourceRect);

    // Specify the destination rectangle for the cropped bitmap
    D2D1_RECT_F destRect = D2D1::RectF(
        half_x - (size.width) / 2 + offset_x + position*108 + position*10 + 50*(position == 2 || position == 3), 
        half_y - (size.height) / 2 + offset_y,
        half_x - (size.width) / 2 + 108 + offset_x + position*108 + position*10 + 50 * (position == 2 || position == 3),
        half_y - (size.height) / 2 + 192 + offset_y);

    // Draw the cropped bitmap at the specified location
    d2d_render_target->DrawBitmap(pCroppedBitmap, destRect);
}

int random_digit_in_range(int range)
{
    srand(time(NULL));

    int my_rand = rand() % range;

    return my_rand;
}

void draw_dots(D2D1_SIZE_F size)
{
    int offset_x = 126;
    int offset_y = 102;

    // Create a new bitmap to hold the cropped portion of the original bitmap
    ID2D1Bitmap* pCroppedBitmap;
    d2d_render_target->CreateBitmap(
        D2D1::SizeU(100, 192),
        NULL,
        0,
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &pCroppedBitmap
    );

    // Copy the portion of the original bitmap that you want to crop
    D2D1_RECT_U sourceRect = D2D1::RectU(0 + 10 * 108, 0, 100 + 10 * 108, 192);
    pCroppedBitmap->CopyFromBitmap(NULL, digits_bitmap, &sourceRect);

    // Specify the destination rectangle for the cropped bitmap
    D2D1_RECT_F destRect = D2D1::RectF(
        half_x - (size.width) / 2 + offset_x + 2 * 108 + 10 - 25,
        half_y - (size.height) / 2 + offset_y,
        half_x - (size.width) / 2 + 100 + offset_x + +2 * 108 + 10,
        half_y - (size.height) / 2 + 192 + offset_y);

    // Draw the cropped bitmap at the specified location
    d2d_render_target->DrawBitmap(pCroppedBitmap, destRect);
}


int first_digit = random_digit_in_range(2);
int second_digit = random_digit_in_range(9);
int third_digit = random_digit_in_range(5);
int fourth_digit = random_digit_in_range(9);


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

        load_bitmaps(hwnd);

        SetTimer(hwnd, 1, 10, NULL);

        return 0;
    }
    case WM_DESTROY:
    {
        if (d2d_render_target) d2d_render_target->Release();
        if (d2d_factory) d2d_factory->Release();

        KillTimer(hwnd, 1);
        
        PostQuitMessage(0);
        return 0;
    }
    case WM_TIMER:
    {
        time_counter++;
        if (time_counter >= 1200) {
            time_counter = 0;
            first_digit++;
        }

        InvalidateRect(hwnd, nullptr, true);
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
        /*d2d_render_target->FillGeometry(path, rad_brush_body);
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
        timev += 0.03f;
        float angle = 15.0f * sin(timev);
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
        d2d_render_target->SetTransform(transformation_to_save);*/
       
        D2D1_SIZE_F size_of_watch = watch_bitmap->GetSize();

        d2d_render_target->DrawBitmap(watch_bitmap, D2D1::RectF(half_x - (size_of_watch.width)/2, half_y - (size_of_watch.height) / 2,
            half_x + (size_of_watch.width) / 2, half_y + (size_of_watch.height) / 2));


        draw_digit(first_digit, 0, size_of_watch);
        draw_digit(second_digit, 1, size_of_watch);
        draw_digit(third_digit, 2, size_of_watch);
        draw_digit(fourth_digit, 3, size_of_watch);
        draw_dots(size_of_watch);


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

        ValidateRect(hwnd, nullptr);
    }
    ellipse_center_pupil1.x -= half_x;
    ellipse_center_pupil1.y -= half_y;
    ellipse_center_pupil2.x -= half_x;
    ellipse_center_pupil2.y -= half_y;
    return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}