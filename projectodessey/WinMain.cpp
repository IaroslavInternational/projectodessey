#include "App.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	try
	{
		return App{ lpCmdLine }.Go();
	}
	catch( const EngineException& e )
	{
		MessageBox( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION );
	}
	catch( const std::exception& e )
	{
		MessageBox( nullptr,e.what(),"??????????? ??????",MB_OK | MB_ICONEXCLAMATION );
	}
	catch( ... )
	{
		MessageBox( nullptr,"?????? ?????? ?? ????","??????????? ??????",MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}