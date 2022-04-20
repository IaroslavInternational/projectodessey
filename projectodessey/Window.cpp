#include "Window.h"
#include <sstream>
#include "resource.h"
#include "WindowsThrowMacros.h"

#include "imgui/imgui_impl_win32.h"

// Window Class stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage( 
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0
	));

	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0
	));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName,GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

// Window Stuff
Window::Window(const char* name)
{
	width = GetDeviceCaps(CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL), HORZRES);
	height = GetDeviceCaps(CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL), VERTRES);

	// Создание окна
	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, WindowClass::GetName(), name, WS_POPUP | WS_MAXIMIZE,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		HWND_DESKTOP, NULL, WindowClass::GetInstance(), this
	);

	// Проверка успешности создания
	if( hWnd == nullptr )
	{
		throw ENGWND_LAST_EXCEPT();
	}

	// Созданное окно изначально спрятно, здесь его показываем
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);

#if IS_ENGINE_MODE
	// Инициализация ImGui Win32 Impl
	ImGui_ImplWin32_Init( hWnd );

	ImGui::GetIO().Fonts->AddFontFromFileTTF("Fonts\\Ubuntu-L.ttf", 15.0F, NULL,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
#endif // IS_ENGINE_MODE

	// Создание графического объекта
	pGfx = std::make_unique<Graphics>(hWnd, width, height);

	// Регистрация данных с мыши
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	
	if(RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE )
	{
		throw ENGWND_LAST_EXCEPT();
	}
}

Window::~Window()
{
#if IS_ENGINE_MODE
	ImGui_ImplWin32_Shutdown();
#endif // IS_ENGINE_MODE

	DestroyWindow(hWnd);
}

void Window::SetTitle( const std::string& title )
{
	if( SetWindowText(hWnd, title.c_str()) == 0 )
	{
		throw ENGWND_LAST_EXCEPT();
	}
}

void Window::EnableCursor() noexcept
{
	cursorEnabled = true;

	ShowCursor();

#if IS_ENGINE_MODE
	EnableImGuiMouse();
#endif // IS_ENGINE_MODE

	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	cursorEnabled = false;

	HideCursor();

#if IS_ENGINE_MODE
	DisableImGuiMouse();
#endif // IS_ENGINE_MODE

	ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;

	// Пока в очереди есть сообщения, удаляем и отправляем их (но не блокируем в пустой очереди)
	while( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Проверяем выход, потому что peekmessage не сигнализирует об этом через return val
		if( msg.message == WM_QUIT )
		{
			// Возвращаем optional wrapping int (аргумент PostQuitMessage в wparam) сигнал
			return (int)msg.wParam;
		}

		// TranslateMessage будет публиковать вспомогательные сообщения WM_CHAR от ключевых msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Возращение пустого optional
	return {};
}

Graphics& Window::Gfx()
{
	if(!pGfx)
	{
		throw ENGWND_NOGFX_EXCEPT();
	}

	return *pGfx;
}

void Window::ConfineCursor() noexcept
{
	RECT rect; 
	
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
	while(::ShowCursor(FALSE) >= 0 );
}

void Window::ShowCursor() noexcept
{
	while(::ShowCursor(TRUE) < 0 );
}

#if IS_ENGINE_MODE
void Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}
#endif // IS_ENGINE_MODE

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Использование параметра create, переданный из CreateWindow(),
	// чтобы сохранить указатель класса окна на стороне WinAPI
	if( msg == WM_NCCREATE )
	{
		// Извлечение указателя на класс окна из данных создания
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// Устанавка WinAPI-managed пользовательских данных для хранения указателя на window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Установка message proc в не установленный обработчик, когда установка окончена
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		// Прямое сообщение в window instance обработчик
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// Елси имеем сообщение до WM_NCCREATE message, обрабатываем стандартно
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Получение указателя на экземпляр окна
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Прямое сообщение в window instance обработчик
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
#if IS_ENGINE_MODE
	if( ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	const auto& imio = ImGui::GetIO();
#endif // IS_ENGINE_MODE

	switch( msg )
	{
	// Мы не хотим DefProc обрабатывать это сообщение, потому что
	// мы хотим, чтобы наш деструктор закрыл окно, возвращаем 0 вместо break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	// Очищение keystate, когда окно теряет фокус, чтобы предотвратить "застревание" ввода
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_ACTIVATE:
		// Ограничение / освобождение курсора в окне передним планом / фоном, если курсор отключен
		if( !cursorEnabled )
		{
			if( wParam & WA_ACTIVE )
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}

		break;

	/*********** Сообщения клавиатуры ***********/
	case WM_KEYDOWN:
	// Команды syskey должны быть обработаны для отслеживания клавиши ALT (VK_MENU) и F10
	case WM_SYSKEYDOWN:
#if IS_ENGINE_MODE
		// Подавление этого сообщения клавиатуры, если imgui хочет захватить его
		if(imio.WantCaptureKeyboard)
		{
			break;
		}
#endif // IS_ENGINE_MODE
		if(!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // Фильтр автоповтора
		{
			kbd.OnKeyPressed( static_cast<unsigned char>(wParam) );
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
#if IS_ENGINE_MODE
		// Подавление этого сообщения клавиатуры, если imgui хочет захватить его
		if(imio.WantCaptureKeyboard)
		{
			break;
		}
#endif // IS_ENGINE_MODE

		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		
		break;
	case WM_CHAR:
#if IS_ENGINE_MODE
		// Подавление этого сообщения клавиатуры, если imgui хочет захватить его
		if(imio.WantCaptureKeyboard)
		{
			break;
		}
#endif // IS_ENGINE_MODE

		kbd.OnChar(static_cast<unsigned char>(wParam));
		
		break;
	/*********** Конец сообщений клавиатуры ***********/

	/************* Сообщения мышки ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		if(!cursorEnabled)
		{
			if(!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
				HideCursor();
			}

			break;
		}

#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if(imio.WantCaptureMouse)
		{
			break;
		}
#endif // IS_ENGINE_MODE

		if(pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);

			if(!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if(wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if(!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}

#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if( imio.WantCaptureMouse )
		{
			break;
		}
#endif // IS_ENGINE_MODE

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		
		break;
	}
	case WM_RBUTTONDOWN:
	{
#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if( imio.WantCaptureMouse )
		{
			break;
		}
#endif // IS_ENGINE_MODE

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);

		break;
	}
	case WM_LBUTTONUP:
	{
#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if(imio.WantCaptureMouse)
		{
			break;
		}
#endif // IS_ENGINE_MODE

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);

		// Освобождение мыши за пределами окна
		if(pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}

		break;
	}
	case WM_RBUTTONUP:
	{
#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if( imio.WantCaptureMouse )
		{
			break;
		}
#endif // IS_ENGINE_MODE
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);

		// Освобождение мыши за пределами окна
		if(pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}

		break;
	}
	case WM_MOUSEWHEEL:
	{
#if IS_ENGINE_MODE
		// Подавление этого сообщения мыши, если imgui хочет захватить её
		if( imio.WantCaptureMouse )
		{
			break;
		}
#endif // IS_ENGINE_MODE

		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
	
		mouse.OnWheelDelta(pt.x, pt.y, delta);

		break;
	}
	/************** Конец сообщений мыши **************/
	
	/************** Необработанные сообщения мыши **************/
	case WM_INPUT:
	{
		if(!mouse.RawEnabled())
		{
			break;
		}

		UINT size;

		// Получение размера входных данных
		if( GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			break;
		}

		rawBuffer.resize( size );
		
		// Чтение входных данных
		if( GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size )
		{
			break;
		}

		// Обработка необработанных входных данных
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

		if(ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}

		break;
	}
	/************** Конец необработанных сообщений мыши **************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception stuff
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;

	// Windows выделит память для строки и создаст указатель на указатель
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	// Проверка на ошибку
	if( nMsgLen == 0 )
	{
		return "Неизвестный код ошибки";
	}

	// Копирование строки из буфера в std::string
	std::string errorString = pMsgBuf;

	// Освобождение windows буфера
	LocalFree(pMsgBuf);

	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Код ошибки] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Описание] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();

	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Ошибка Window класса";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode( hr );
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Ошибка Window класса [Нет графики]";
}