#include "view.h"
#include <stdlib.h>
#include <assert.h>

void ViewInit(view_t* view, HWND hwnd)
{
    assert(view);

    view->hwnd = hwnd;
    view->model = NULL;
    view->lines = NULL;
    view->mode = RAW;
    view->numOfLines = 0;
    view->maxLineLength = 0;

    RECT winRect;
    GetWindowRect(hwnd, &winRect);
    view->windowWidth = winRect.right - winRect.left;
    view->windowHeight = winRect.bottom - winRect.top;

    HDC hdc = GetDC(hwnd);
    SIZE size;
    GetTextExtentPoint32(hdc,"W", 1, &size);
    view->font.hfont = NULL;
    view->font.height = size.cy;
    view->font.width = size.cx;
    view->font.hfont = NULL;

    view->vScrollPos = 0;
    view->hScrollPos = 0;
    view->vPageSize = view->windowHeight / view->font.height;
    view->hPageSize = view->windowWidth / view->font.width;
}

static void _recalcVScrollPos(view_t* view, char* prevPtr)
{
    assert(view);

    if(!prevPtr)
    {
        view->vScrollPos = 0;
        return;
    }

    unsigned long newPos = 0;

    while (view->lines[newPos + 1] <= prevPtr)
        ++newPos;

    view->vScrollPos = newPos;
}

static error_t _buildRaw(view_t* view)
{
    assert(view);
    assert(view->model);

    view->numOfLines = 1;
    for (int i = 0; i < view->model->size; ++i)
        if (view->model->data[i] == '\n')
            ++view->numOfLines;

    view->lines = malloc(sizeof(char*) * (view->numOfLines + 1));
    if (!view->lines)
        return MEMORY_ERR;

    int j = 0;
    view->maxLineLength = 0;
    view->lines[j++] = view->model->data;
    for (int i = 0; i < view->model->size; ++i)
    {
        if (view->model->data[i] == '\n')
        {
            view->lines[j++] = view->model->data + i + 1;
            unsigned long lineLength = view->lines[j - 1] - view->lines[j - 2];
            view->maxLineLength = max(lineLength, view->maxLineLength);
        }
    }

    view->lines[j] = view->model->data + view->model->size;

    return SUCCESS;
}

static error_t _buildLayout(view_t* view)
{
    assert(view);
    assert(view->model);

    view->maxLineLength = view->hPageSize;
    view->hScrollPos = 0;

    view->numOfLines = 1;
    int symbolCounter = 0;
    for (int i = 0; i < view->model->size; ++i)
    {
        ++symbolCounter;
        if (view->model->data[i] == '\n' || symbolCounter == view->maxLineLength)
        {
            ++view->numOfLines;
            symbolCounter = 0;
        }
    }

    view->lines = malloc(sizeof(char*) * (view->numOfLines + 1));
    if (!view->lines)
        return MEMORY_ERR;

    int j = 0;
    view->lines[j++] = view->model->data;
    symbolCounter = 0;
    for (int i = 0; i < view->model->size; ++i)
    {
        ++symbolCounter;
        if (view->model->data[i] == '\n' || symbolCounter == view->maxLineLength)
        {
            view->lines[j++] = view->model->data + i + 1;
            symbolCounter = 0;
        }
    }

    view->lines[j] = view->model->data + view->model->size;

    return SUCCESS;
}

static error_t _build(view_t* view)
{
    assert(view);

    if (!view->model)
        return SUCCESS;

    char* prevPosPtr = NULL;
    if (view->lines)
        prevPosPtr = view->lines[view->vScrollPos];

    view->numOfLines = 0;
    free(view->lines);

    int err = view->mode == RAW ? _buildRaw(view) : _buildLayout(view);

    _recalcVScrollPos(view, prevPosPtr);

    return err;
}

error_t ViewBindModel(view_t* view, const model_t* model)
{
    assert(view);
    assert(model);

    view->model = model;
    view->vScrollPos = 0;
    view->hScrollPos = 0;

    return _build(view);
}

error_t ViewSetMode(view_t* view, mode_t mode)
{
    assert(view);

    if (mode == view->mode)
        return SUCCESS;

    view->mode = mode;

    return _build(view);
}

error_t ViewSetWindowSize(view_t* view, unsigned long width, unsigned long height)
{
    assert(view);

    unsigned long prevWidth = view->windowWidth;

    view->windowWidth = width;
    view->windowHeight = height;
    view->vPageSize = height / view->font.height;
    view->hPageSize = width / view->font.width;

    if (view->vPageSize <= view->numOfLines)
        view->vScrollPos = min(view->vScrollPos, view->numOfLines - view->vPageSize);

    if (view->hPageSize <= view->maxLineLength)
        view->hScrollPos = min(view->hScrollPos, view->maxLineLength - view->hPageSize);

    return view->mode == RAW || width == prevWidth ? SUCCESS : _build(view);
}

void ViewUp(view_t* view, unsigned long count)
{
    assert(view);

    if (view->vScrollPos < count)
        view->vScrollPos = 0;
    else
        view->vScrollPos -= count;
}

void ViewDown(view_t* view, unsigned long count)
{
    assert(view);

    if (view->vPageSize < view->numOfLines)
    {
        unsigned long lastLine = view->numOfLines - view->vPageSize;
        if (view->vScrollPos + count < lastLine)
            view->vScrollPos += count;
        else
            view->vScrollPos = lastLine;
    }
}

void ViewPageUp(view_t* view)
{
    assert(view);
    ViewUp(view, view->vPageSize);
}

void ViewPageDown(view_t* view)
{
    assert(view);
    ViewDown(view, view->vPageSize);
}

void ViewHome(view_t* view)
{
    assert(view);
    view->vScrollPos = 0;
}

void ViewEnd(view_t* view)
{
    assert(view);
    if (view->numOfLines < view->vPageSize)
        view->vScrollPos = 0;
    else
        view->vScrollPos = view->numOfLines - view->vPageSize;
}

void ViewRight(view_t* view, unsigned long count)
{
    assert(view);

    if (view->hPageSize < view->maxLineLength)
    {
        unsigned long lastPosInLine = view->maxLineLength - view->hPageSize;
        if (view->hScrollPos + count < lastPosInLine)
            view->hScrollPos += count;
        else
            view->hScrollPos = lastPosInLine;
    }
}

void ViewLeft(view_t* view, unsigned long count)
{
    assert(view);

    if (view->mode == RAW && view->hScrollPos > count)
        view->hScrollPos -= count;
    else
        view->hScrollPos = 0;
}

void ViewSetFont(view_t* view, TCHAR* fontName, unsigned long height)
{
    assert(view);

    DeleteObject(view->font.hfont);
    view->font.height = height;

    view->font.hfont = CreateFont(view->font.height, 0, 0, 0, 0, 0, 0,
                                   0, RUSSIAN_CHARSET, 0, 0, 0, FIXED_PITCH, fontName);

    HDC hdc = GetDC(view->hwnd);
    SelectObject(hdc, view->font.hfont);
    SetBkMode(hdc, TRANSPARENT);

    SIZE size;
    GetTextExtentPoint32(hdc, "W", 1, &size);
    view->font.height = size.cy;
    view->font.width = size.cx;

    view->hPageSize = view->windowWidth / view->font.width;
    view->vPageSize = view->windowHeight / view->font.height;
}

void ViewDraw(const view_t* view)
{
    assert(view);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(view->hwnd, &ps);

    RECT winRect;

    GetClientRect(view->hwnd, &winRect);

    unsigned long lineLength;
    unsigned long hScrollPos;
    unsigned long symWindowWidth = view->hPageSize + 1;
    int yCoord = winRect.top;
    for (int i = view->vScrollPos; i < view->vScrollPos + view->vPageSize + 1 && i < view->numOfLines; ++i)
    {
        if (view->lines[i + 1] - view->lines[i] > view->hScrollPos)
        {
            lineLength = min(symWindowWidth, view->lines[i + 1] - view->lines[i] - view->hScrollPos);
            if (*(view->lines[i + 1] - 1) == '\n')
                --lineLength;
            if (*(view->lines[i + 1] - 2) == '\r')
                --lineLength;
            hScrollPos = min(view->hScrollPos, view->lines[i + 1] - view->lines[i] - 1);
            TextOut(hdc, winRect.top, yCoord, view->lines[i] + hScrollPos, lineLength);
        }
        yCoord += view->font.height;
    }

     EndPaint(view->hwnd, &ps);

}

void ViewClear(view_t* view)
{
    assert(view);

    free(view->lines);
    view->model = NULL;
    view->lines = NULL;
    view->numOfLines = 0;
    view->mode = RAW;
    view->windowWidth = 0;
    DeleteObject(view->font.hfont);
}
