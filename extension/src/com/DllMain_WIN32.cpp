#include <windows.h>

HINSTANCE hDllInstance = 0;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Save the instance handle
		hDllInstance = hInstance;
		// initialize the COM module information
		//_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
		// initialize the common controls
		// they are used in some of the dialogs
		//InitCommonControls();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//_Module.Term();
	}

	return TRUE;    // ok
}
