#include "GUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window, UINT Message, WPARAM WideParameter, LPARAM LongParameter
);

long __stdcall WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter) {

	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) {
		return true;
	}

	switch (message) {
		case WM_SIZE: {
			if (GUI::device && wideParameter != SIZE_MINIMIZED) {
				GUI::PresentParameters.BackBufferWidth = LOWORD(longParameter);
				GUI::PresentParameters.BackBufferHeight = HIWORD(longParameter);
				GUI::ResetDevice();
			}
		}return 0;
		case WM_SYSCOMMAND: {
			if ((wideParameter && 0xfff0) == SC_KEYMENU) {
				return 0;
			}
		}break;

		case WM_DESTROY: {
			PostQuitMessage(0);
		}return 0;


		case WM_LBUTTONDOWN: {
			GUI::position = MAKEPOINTS(longParameter);
		}return 0;

		case WM_MOUSEMOVE: {
			if (wideParameter == MK_LBUTTON) {
				const auto Points = MAKEPOINTS(longParameter);
				auto rect = ::RECT{ };

				GetWindowRect(GUI::window, &rect);

				rect.left += Points.x - GUI::position.x;
				rect.top += Points.y - GUI::position.y;

				if (GUI::position.x >= 0 && GUI::position.x <= GUI::Width && GUI::position.y >= 0 && GUI::position.y <= 19) {
					SetWindowPos(GUI::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
				}
			}
		}
	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}


void GUI::CreateTheWindow(const char* WindowName, const char* ClassName) noexcept
{
	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = CS_CLASSDC;
	WindowClass.lpfnWndProc = WindowProcess;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleA(0);
	WindowClass.hIcon = 0;
	WindowClass.lpszClassName = ClassName;

	RegisterClassExA(&WindowClass);

	window = CreateWindowA(ClassName, WindowName, WS_POPUP, 100, 100, Width, Height, 0, 0, WindowClass.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

}

void GUI::DestroyTheWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
}

bool GUI::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) {
		return false;
	}

	ZeroMemory(&PresentParameters, sizeof(PresentParameters));
	PresentParameters.Windowed = TRUE;
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	PresentParameters.EnableAutoDepthStencil = TRUE;
	PresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	// come back 

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParameters, &device) < 0) {
		return false;
	}
	return true;
}

void GUI::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT Result = device->Reset(&PresentParameters);
	if (Result == D3DERR_INVALIDCALL) {
		IM_ASSERT(0);
	}
	ImGui_ImplDX9_CreateDeviceObjects();
}

void GUI::DestroyDevice() noexcept
{
	if (device) { 
		device->Release(); device = nullptr; 
	}
	if (d3d) { 
		d3d->Release(); d3d = nullptr; 
	}
}

void GUI::BeginRender() noexcept
{

	MSG msg;
	while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUI::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void GUI::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	ImVec4* clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)
		(clear_color->x * clear_color->w * 255.0f), 
		(int)(clear_color->y * clear_color->w * 255.0f), 
		(int)(clear_color->z * clear_color->w * 255.0f), 
		(int)(clear_color->w * 255.0f)
	);
	delete clear_color;
	device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);


	if (device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const HRESULT result = device->Present(0, 0, 0, 0);
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ResetDevice();
	}
}

void GUI::DestroyImGui() noexcept
{
	
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	delete windowTitle;
}

void GUI::Render() noexcept
{
	windowTitle = static_cast<const wchar_t*>(L"APIRequester");
	//wcscpy_s(windowTitle.get(),16, L"My DX9 Application");
	SetWindowText(window, (LPCSTR)windowTitle);
	
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ GUI::Width , GUI::Height });

	ImGui::End();
}
