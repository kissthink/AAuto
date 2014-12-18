#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501 
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include "wke.h"

extern "C" __declspec(dllexport) BOOL __cdecl WebViewWndProc(wkeWebView webView,HIMC hImmContext,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,int *pRetCode)
{
    bool handled = true;
	//HIMC hIMC = (HIMC)GetWindowLong(hWnd,IMMGWL_IMC); 
	//LPINPUTCONTEXT  lpIMC       = NULL;  
	*pRetCode = 0;
	
    switch (message)
    {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc;
        hdc = BeginPaint(hWnd, &ps);
        webView->setDirty(true);
        EndPaint(hWnd, &ps);
		handled = true;
        break;
	}
	case WM_ERASEBKGND:
		webView->setDirty(true);
		handled = true;
        break;	
   case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:

    case WM_MOUSEMOVE:
        {
            if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
            {
                SetFocus(hWnd);
                SetCapture(hWnd);
            }
            else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
            {
                ReleaseCapture();
            }

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            unsigned int flags = 0;

            if (wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            //flags = wParam;
            handled = webView->mouseEvent(message, x, y, flags);
        }
        break;
    case WM_KEYDOWN:
        {
            unsigned int virtualKeyCode = wParam;
            unsigned int flags = 0;
            if (HIWORD(lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);
            handled = webView->keyDown(virtualKeyCode, flags, false); 
        }
        break;

    case WM_KEYUP:
        {
            unsigned int virtualKeyCode = wParam;
            unsigned int flags = 0;
            if (HIWORD(lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);
            handled = webView->keyUp(virtualKeyCode, flags, false); 
        }
        break;

    case WM_CHAR:
        {
			int len = 1;
			unsigned char chr[3];
			wchar_t pUnicode[2] = {0,0};   
			static unsigned int precharcode; 
            unsigned int charCode = wParam;
            unsigned int flags = 0;
			if( precharcode ){
				chr[0] = precharcode;
				chr[1] = charCode;
				chr[2] = 0;
				len = 2;
				precharcode = 0;
			}
			else if(charCode>0x80){
				precharcode = charCode;
				handled = true;
				break;
			}
			else{
				chr[0] = charCode;
				chr[1] = 0;
				len = 1;
			} 
			::MultiByteToWideChar( CP_ACP,  0,(LPSTR)chr, len,  (LPWSTR)pUnicode, 1 );  
			
            if (HIWORD(lParam) & KF_REPEAT)
                flags |= WKE_REPEAT;
            if (HIWORD(lParam) & KF_EXTENDED)
                flags |= WKE_EXTENDED;

            //flags = HIWORD(lParam);
            handled = webView->keyPress(pUnicode[0], flags, false);
			
			
        }
        break;
 

    case WM_CONTEXTMENU:
        {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);

            if (pt.x != -1 && pt.y != -1)
                ScreenToClient(hWnd, &pt);

            unsigned int flags = 0;

            if (wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            handled = webView->contextMenuEvent(pt.x, pt.y, flags);
        }
        break;

    case WM_MOUSEWHEEL:
        {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);

            int delta = GET_WHEEL_DELTA_WPARAM(wParam);

            unsigned int flags = 0;

            if (wParam & MK_CONTROL)
                flags |= WKE_CONTROL;
            if (wParam & MK_SHIFT)
                flags |= WKE_SHIFT;

            if (wParam & MK_LBUTTON)
                flags |= WKE_LBUTTON;
            if (wParam & MK_MBUTTON)
                flags |= WKE_MBUTTON;
            if (wParam & MK_RBUTTON)
                flags |= WKE_RBUTTON;

            //flags = wParam;
            handled = webView->mouseWheel(pt.x, pt.y, delta, flags);
        }
        break;

    case WM_SETFOCUS:
        webView->focus();
        break;

    case WM_KILLFOCUS:
        webView->unfocus();
        break; 
		
    case WM_IME_STARTCOMPOSITION:
        {
            wkeRect caret = webView->getCaret();

            CANDIDATEFORM form;
            form.dwIndex = 0;
            form.dwStyle = CFS_EXCLUDE;
            form.ptCurrentPos.x = caret.x;
            form.ptCurrentPos.y = caret.y + caret.h;
            form.rcArea.top = caret.y;
            form.rcArea.bottom = caret.y + caret.h;
            form.rcArea.left = caret.x;
            form.rcArea.right = caret.x + caret.w;

            HIMC hIMC = ImmGetContext(hWnd);
            ImmSetCandidateWindow(hImmContext, &form);
			//ImmSetCompositionWindow (hImmContext, (COMPOSITIONFORM*)&form);
            ImmReleaseContext(hWnd, hIMC);
        }
        break;
	case WM_GETDLGCODE:
		*pRetCode =  DLGC_WANTALLKEYS;
		handled = true;
		break;
	case WM_SIZE:
        webView->resize(LOWORD(lParam), HIWORD(lParam));
        break;
	/*
	case WM_IME_SETCONTEXT:   
        if (wParam == 1)  
        {  
			ImmAssociateContext(hWnd, hImmContext);  
        }  
        break;  
	*/
	
	case WM_DESTROY:
		webView->destroy();
		handled = false;
    default:
        handled = false;
        break;
    }
 
    return handled;
}