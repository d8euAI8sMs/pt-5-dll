// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Resource.h"

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//					 )
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}

LONG WINAPI CPlApplet(HWND hWnd, UINT uMsg, LPARAM param1, LPARAM param2)
{
    CPLINFO *i;
    switch (uMsg)
    {
    case CPL_INIT:
        return TRUE;
    case CPL_GETCOUNT:
        return 2;
    case CPL_INQUIRE:
        i = (CPLINFO*) param2;
        switch ((int) param1)
        {
        case 0:
            i->idIcon = IDI_ICON1;
            i->idInfo = IDS_ITEM1_INFO;
            i->idName = IDS_ITEM1_NAME;
            i->lData = NULL;
            return TRUE;
        case 1:
            i->idIcon = IDI_ICON2;
            i->idInfo = IDS_ITEM2_INFO;
            i->idName = IDS_ITEM2_NAME;
            i->lData = NULL;
            return TRUE;
        default:
            return FALSE;
        }
        break;
    case CPL_DBLCLK:
        switch ((int) param1)
        {
        case 0:
            AfxMessageBox(_T("Some message from 1-st item"));
            return TRUE;
        case 1:
            AfxMessageBox(_T("Some message from 2-st item"), MB_HELP);
            return TRUE;
        default:
            return FALSE;
        }
    case CPL_STOP:
        return TRUE;
    case CPL_EXIT:
        return TRUE;
    }
    return FALSE;
}