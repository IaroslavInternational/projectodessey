#include "AdapterData.h"
#include <wrl\client.h>

std::vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;

	HRESULT hr = pAdapter->GetDesc(&this->desc);

	if (FAILED(hr))
	{
		exit(-1);
	}
}

std::vector<AdapterData> AdapterReader::GetAdapterData()
{
	if (adapters.size() > 0)
	{
		return adapters;
	}

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(hr))
	{
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}

	return adapters;
}
