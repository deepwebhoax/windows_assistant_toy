// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <combaseapi.h>
#include "../headers/avatar.h"





Avatar::Avatar(HWND hwnd)
    : m_hwnd(hwnd),
      m_currentFrame(0),
      m_totalFrames(0),
      m_animationSpeed(0.1f)
{
    m_position = D2D1::Point2F(0.0f, 0.0f);
}

Avatar::~Avatar()
{
    DiscardDeviceResources();
}

bool Avatar::Initialize()
{
    // Create D2D factory
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf());
    if (FAILED(hr)) return false;

    // Create WIC factory
    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory));
    if (FAILED(hr)) return false;

    // Create device resources
    CreateDeviceResources();
    
    // Load SVG image or animation frames
    // Assuming LoadSVGImage is capable of loading multiple frames
    // Example:
    LoadSVGImage(L"avatar.svg", m_avatarBitmap.GetAddressOf());
    m_animationFrames.push_back(m_avatarBitmap);
    m_totalFrames = static_cast<UINT>(m_animationFrames.size());

    return true;
}

void Avatar::CreateDeviceResources()
{
    // Create Direct3D device and context
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &d3dDevice, nullptr, &d3dContext);

    // Convert Direct3D device to DXGI device
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    d3dDevice.As(&dxgiDevice);

    // Create Direct2D device and context
    m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);
    m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);
}

void Avatar::DiscardDeviceResources()
{
    m_d2dContext.Reset();
    m_d2dDevice.Reset();
    m_renderTarget.Reset();
}

void Avatar::Render()
{
    m_d2dContext->BeginDraw();
    m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
    m_d2dContext->DrawBitmap(m_animationFrames[m_currentFrame].Get(), D2D1::RectF(m_position.x, m_position.y, m_position.x + 100.0f, m_position.y + 100.0f));
    m_d2dContext->EndDraw();
}

void Avatar::Move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
    Render();
}

void Avatar::Animate()
{
    m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
    Render();
}

bool Avatar::LoadSVGImage(const std::wstring& filepath, ID2D1Bitmap1** bitmap)
{
    // Load SVG image using WIC and convert to Direct2D bitmap
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = m_wicFactory->CreateDecoderFromFilename(filepath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
    hr = m_wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) return false;

    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return false;

    hr = m_d2dContext->CreateBitmapFromWicBitmap(converter.Get(), nullptr, bitmap);
    return SUCCEEDED(hr);
}
