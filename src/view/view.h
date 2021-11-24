#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#pragma once

#include <Windows.h>
#include "../model/model.h"
#include "../error.h"

typedef enum
{
    RAW,
    LAYOUT,

} mode_t;

typedef struct
{
    HFONT hfont;
    int width;
    int height;

} font_t;

typedef struct
{
    const model_t* model;
    char** lines;

    unsigned long numOfLines;
    unsigned long maxLineLength;

    unsigned long vScrollPos;
    unsigned long hScrollPos;

    unsigned long vPageSize;
    unsigned long hPageSize;

    unsigned long windowWidth;
    unsigned long windowHeight;

    mode_t mode;
    font_t font;

    HWND hwnd;

} view_t;


void ViewInit(view_t* view, HWND hwnd);

error_t ViewBindModel(view_t* view, const model_t* model);
void ViewSetFont(view_t* view, TCHAR* fontName, unsigned long height);
error_t ViewSetMode(view_t* view, mode_t mode);
error_t ViewSetWindowSize(view_t* view, unsigned long width, unsigned long height);


void ViewUp(view_t* view, unsigned long count);
void ViewDown(view_t* view, unsigned long count);
void ViewRight(view_t* view, unsigned long count);
void ViewLeft(view_t* view, unsigned long count);
void ViewPageUp(view_t* view);
void ViewPageDown(view_t* view);
void ViewHome(view_t* view);
void ViewEnd(view_t* view);

void ViewDraw(const view_t* view);

void ViewClear(view_t* view);

#endif // VIEW_H_INCLUDED
