#pragma once

#include "EngineWin.h"

#include <d3d11.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desc;
};

class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapterData();
private:
	static std::vector<AdapterData> adapters;
};