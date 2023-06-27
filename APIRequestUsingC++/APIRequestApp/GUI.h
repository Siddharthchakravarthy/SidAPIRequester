#pragma once
#include <d3d9.h>
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx9.h"
#include "../Imgui/imgui_impl_win32.h"
#include <memory>
#include <wchar.h>
namespace GUI
{
	const int Width = 800;
	const int Height = 600;

	inline bool exit = true;

	inline HWND window = nullptr;
	inline WNDCLASSEXA WindowClass;
	inline POINTS position;

	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS PresentParameters = { };
	
	void CreateTheWindow(const char* WindowName, const char* ClassName) noexcept;
	void DestroyTheWindow() noexcept;


	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;


	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
	inline const wchar_t* windowTitle;
	//std::unique_ptr<wchar_t[]> windowTitle = std::make_unique<wchar_t[]>(sizeof(char));
};
//HWND GUI::window = nullptr;
//LPDIRECT3DDEVICE9 GUI::device = nullptr;
//PDIRECT3D9 GUI::d3d = nullptr;
//bool GUI::exit = true;
//
//ImVec4 GUI::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);