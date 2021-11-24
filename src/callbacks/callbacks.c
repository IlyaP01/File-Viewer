#include "callbacks.h"
#include "../menuDefs.h"
#include "../error.h"
#include <tchar.h>

#define SCROLL_MAX  65535

static void _handleError(error_t err)
{
    if(err == MEMORY_ERR)
    {
       MessageBox(NULL, (LPCSTR)L"Not enough memory", (LPCSTR)L"Error", MB_OK | MB_ICONERROR);
    }
    else if(err == OPEN_FILE_ERR)
    {
       MessageBox(NULL, (LPCSTR)L"Could not open file", (LPCSTR)L"Error", MB_OK | MB_ICONERROR);
    }
}

void _updateVScroll(HWND hwnd, const view_t* view)
{
    double vScale = (double)SCROLL_MAX / (view->numOfLines - view->vPageSize);
    SetScrollPos(hwnd, SB_VERT, view->vScrollPos * vScale, TRUE);
    if (view->windowHeight / view->font.height > view->numOfLines)
        ShowScrollBar(hwnd, SB_VERT, FALSE);
    else
        ShowScrollBar(hwnd, SB_VERT, TRUE);
}

void _updateHScroll(HWND hwnd, const view_t* view)
{
    double hScale = (double)SCROLL_MAX / (view->maxLineLength - view->hPageSize);
    SetScrollPos(hwnd, SB_HORZ, view->hScrollPos * hScale, TRUE);
    if (view->mode == LAYOUT || view->maxLineLength < view->windowWidth / view->font.width)
        ShowScrollBar(hwnd, SB_HORZ, FALSE);
    else
        ShowScrollBar(hwnd, SB_HORZ, TRUE);

}

static error_t _openFile(HWND hwnd, model_t* model, view_t* view)
{
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    error_t err = NO_ERROR;
    if (GetOpenFileName(&ofn) == TRUE)
    {
        err = ModelBuild(model, ofn.lpstrFile);
        if (err == NO_ERROR)
            err = ViewBindModel(view, model);
    }

    return err;
}

void OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    ModelInit(model);
    ViewInit(view, hwnd);
    ViewSetFont(view, "Consolas", 18);

    SetScrollRange(hwnd, SB_HORZ, 0, SCROLL_MAX, FALSE);
    SetScrollRange(hwnd, SB_VERT, 0, SCROLL_MAX, FALSE);

    ViewSetMode(view, RAW);
    CheckMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_UNCHECKED);
    CheckMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_CHECKED);
    EnableMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_ENABLED);
    EnableMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_GRAYED);
}

void OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    ModelClear(model);
    ViewClear(view);
    PostQuitMessage (0);
}

void OnSize(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    int err = ViewSetWindowSize(view, LOWORD(lParam), HIWORD(lParam));
    if (err != SUCCESS)
    {
        _handleError(err);
        SendMessage(hwnd, WM_CLOSE, (unsigned)NULL, (long int)NULL);
        return;
    }

    _updateHScroll(hwnd, view);
    _updateVScroll(hwnd, view);
}

void OnMouseWheel(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
    {
        SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)NULL);
        SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)NULL);
    }
    else
    {
        SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)NULL);
        SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)NULL);
    }
}

void OnKeyDown(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    switch (wParam)
    {
        case VK_DOWN:
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, (long int)NULL);
            break;
        case VK_UP:
            SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, (long int)NULL);
            break;
        case VK_RIGHT:
            SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, (long int)NULL);
            break;
        case VK_LEFT:
            SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, (long int)NULL);
            break;
        case VK_ESCAPE:
            SendMessage(hwnd, WM_CLOSE, (unsigned)NULL, (long int)NULL);
            break;
        case VK_PRIOR:
            SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, (long int)NULL);
            break;
        case VK_NEXT:
            SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, (long int)NULL);
            break;
        case VK_HOME:
            SendMessage(hwnd, WM_VSCROLL, SB_TOP, (long int)NULL);
            break;
        case VK_END:
            SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, (long int)NULL);
            break;
        case 0x4D: // M or m key (mode)
            SendMessage(hwnd,
                        WM_COMMAND,
                        MAKEWPARAM(view->mode == LAYOUT ? MENU_RAW_MODE : MENU_LAYOUT_MODE, 0),
                        (LPARAM)NULL);
            break;
    }
}

void OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    error_t err = NO_ERROR;
    switch (LOWORD(wParam))
    {
        case MENU_OPEN_FILE:
            err = _openFile(hwnd, model, view);
            break;

        case MENU_RAW_MODE:
            CheckMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_UNCHECKED);
            CheckMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_CHECKED);
            EnableMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_ENABLED);
            EnableMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_GRAYED);
            if (view->mode == RAW)
                return;
            err = ViewSetMode(view, RAW);
            break;

        case MENU_LAYOUT_MODE:
            CheckMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_UNCHECKED);
            CheckMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_CHECKED);
            EnableMenuItem(GetMenu(hwnd), MENU_RAW_MODE, MF_ENABLED);
            EnableMenuItem(GetMenu(hwnd), MENU_LAYOUT_MODE, MF_GRAYED);
            if (view->mode == LAYOUT)
                return;
            err = ViewSetMode(view, LAYOUT);
            break;

        case MENU_EXIT:
            SendMessage(hwnd, WM_CLOSE, (unsigned)NULL, (long int)NULL);
            return;
    }
    if (err != NO_ERROR)
        _handleError(err);

    _updateHScroll(hwnd, view);
    _updateVScroll(hwnd, view);

    InvalidateRect(hwnd, 0, TRUE);
    UpdateWindow(hwnd);
}

void OnVScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    switch (LOWORD (wParam))
    {
        case SB_TOP:
            ViewHome(view);
            break;

        case SB_BOTTOM:
            ViewEnd(view);
            break;

        case SB_LINEUP:
            ViewUp(view, 1);
            break;

        case SB_LINEDOWN:
            ViewDown(view, 1);
            break;

        case SB_PAGEUP:
            ViewPageUp(view);
            break;

        case SB_PAGEDOWN:
            ViewPageDown(view);
            break;

        case SB_THUMBTRACK:
            {
                double vScale = (double)SCROLL_MAX / (view->numOfLines - view->vPageSize);
                unsigned long pos = HIWORD(wParam) / vScale;
                if (pos > view->vScrollPos)
                    ViewDown(view, pos - view->vScrollPos);
                else
                    ViewUp(view, view->vScrollPos - pos);
            }
            break;
    }

    _updateVScroll(hwnd, view);
    InvalidateRect(hwnd, 0, TRUE);
    UpdateWindow(hwnd);
}

void OnHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view)
{
    switch (LOWORD(wParam)) {
        case SB_LINERIGHT:
            ViewRight(view, 1);
            break;

        case SB_LINELEFT:
            ViewLeft(view, 1);
            break;

        case SB_THUMBTRACK:
            {
                double hScale = (double)SCROLL_MAX / (view->maxLineLength - view->hPageSize);
                unsigned long pos = HIWORD(wParam) / hScale;
                if (pos > view->hScrollPos)
                    ViewRight(view, pos - view->hScrollPos);
                else
                    ViewLeft(view, view->hScrollPos - pos);
            }
            break;
    }

    _updateHScroll(hwnd, view);
    InvalidateRect(hwnd, 0, TRUE);
    UpdateWindow(hwnd);
}
