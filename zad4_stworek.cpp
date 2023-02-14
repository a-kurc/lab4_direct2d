#ifndef UNICODE
#define UNICODE
#endif 

#include "zad4_stworek.h"
using namespace globals;


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
    D2D1_RECT_U sourceRect = D2D1::RectU(0 + digit * 108, 0, 108 + digit * 108, 192);
    pCroppedBitmap->CopyFromBitmap(NULL, digits_bitmap, &sourceRect);

    // Specify the destination rectangle for the cropped bitmap
    D2D1_RECT_F destRect = D2D1::RectF(
        half_x - (size.width) / 2 + offset_x + position * 108 + position * 10 + 50 * (position == 2 || position == 3),
        half_y - (size.height) / 2 + offset_y,
        half_x - (size.width) / 2 + 108 + offset_x + position * 108 + position * 10 + 50 * (position == 2 || position == 3),
        half_y - (size.height) / 2 + 192 + offset_y);

    // Draw the cropped bitmap at the specified location
    d2d_render_target->DrawBitmap(pCroppedBitmap, destRect, 0.7);
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
    d2d_render_target->DrawBitmap(pCroppedBitmap, destRect, 0.7);
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


void increase_time()
{
    if (fourth_digit < 9)
    {
        fourth_digit++;
        return;
    }
    if (third_digit < 5)
    {
        fourth_digit = 0;
        third_digit++;
        return;
    }
    if (second_digit < 9 && first_digit < 2) //np. 18, 04, 12
    {
        fourth_digit = 0;
        third_digit = 0;
        second_digit++;
        return;
    }
    if (first_digit < 2) // 19
    {
        fourth_digit = 0;
        third_digit = 0;
        second_digit = 0;
        first_digit++;
        return;
    }
    if (second_digit < 3 && first_digit == 2) //np. 21, 23
    {
        fourth_digit = 0;
        third_digit = 0;
        second_digit++;
        return;
    }
    fourth_digit = 0;
    third_digit = 0;
    second_digit = 0;
    first_digit = 0;
    return;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GetClientRect(hwnd, &rc);
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

        if (!rc_initiated_already)
        {
            initial_higth = rc.bottom - rc.top;
            initial_width = rc.right - rc.left;
        }

        load_bitmaps(hwnd);

        first_digit = random_digit_in_range(2);
        second_digit = random_digit_in_range(9);
        third_digit = random_digit_in_range(5);
        fourth_digit = random_digit_in_range(9);

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

            increase_time();
        }

        InvalidateRect(hwnd, nullptr, true);
        return 0;
    }
    case WM_PAINT:
    {
        Matrix3x2F scale = Matrix3x2F::Scale((initial_width)/(rc.right - rc.left), 
            ((initial_higth) / (rc.bottom - rc.top)), Point2F(0,0));
        d2d_render_target->SetTransform(scale);

        ID2D1SolidColorBrush* brush = nullptr;

        // Sta³e z kolorami
        D2D1_COLOR_F const brush_color_violet =
        { .r = 0.8f, .g = 0.4f, .b = 0.8f, .a = 1.0f };

        // Utworzenie pêdzli
        d2d_render_target->CreateSolidColorBrush(brush_color_violet, &brush);
        
        // Rysowanie
        d2d_render_target->BeginDraw();
        d2d_render_target->Clear(brush_color_violet);

        d2d_render_target->FillRectangle(D2D1::RectF(0, 0, rc.right, rc.bottom), brush);
        // Obracamy zegar 
        float angle = -8.0f;
        // Zachowujemy macierz transofmacji
        D2D1_MATRIX_3X2_F transformation_to_save;
        d2d_render_target->GetTransform(&transformation_to_save);
        Matrix3x2F transformation;
        d2d_render_target->GetTransform(&transformation);
        Matrix3x2F rotate = Matrix3x2F::Rotation(angle, Point2F(half_x, half_y));
        rotate.SetProduct(rotate, transformation);
        d2d_render_target->SetTransform(rotate);
        D2D1_SIZE_F size_of_watch = watch_bitmap->GetSize();

        d2d_render_target->DrawBitmap(watch_bitmap, D2D1::RectF(half_x - (size_of_watch.width) / 2, 
            half_y - (size_of_watch.height) / 2,
            half_x + (size_of_watch.width) / 2, 
            half_y + (size_of_watch.height) / 2));

        draw_digit(first_digit, 0, size_of_watch);
        draw_digit(second_digit, 1, size_of_watch);
        draw_digit(third_digit, 2, size_of_watch);
        draw_digit(fourth_digit, 3, size_of_watch);
        if ((time_counter/10)%2 == 0)
            draw_dots(size_of_watch);

        d2d_render_target->SetTransform(transformation_to_save);
       
        d2d_render_target->EndDraw();

        if (brush) brush->Release();

        ValidateRect(hwnd, nullptr);
    }

    return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}