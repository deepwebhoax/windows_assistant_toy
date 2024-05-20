// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#pragma once

#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <DXGI1_2.h>
#include <vector>
#include <string>


class BaseAvatar
{
public:
    virtual ~BaseAvatar() = default;
    virtual bool Initialize() = 0;
    virtual void Render() = 0;
    virtual void Move(float dx, float dy) = 0;
    virtual void Animate() = 0;
};


class Avatar
{
public:
    Avatar(HWND hwnd);
    ~Avatar();

    bool Initialize();
    void Render();
    void Move(float dx, float dy);
    void Animate();

private:
    bool LoadSVGImage(const std::wstring& filepath, ID2D1Bitmap1** bitmap);
    void CreateDeviceResources();
    void DiscardDeviceResources();

    HWND m_hwnd;
    Microsoft::WRL::ComPtr<ID2D1Factory1> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext;
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_avatarBitmap;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
    Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicFactory;
    std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_animationFrames;
    D2D1_POINT_2F m_position;
    UINT m_currentFrame;
    UINT m_totalFrames;
    float m_animationSpeed;
};
