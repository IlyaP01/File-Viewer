#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#pragma once

#include <Windows.h>
#include "../model/model.h"
#include "../error.h"

/*
 * Type for mode
 */
typedef enum
{
    RAW,
    LAYOUT,

} mode_t;

/*
 * Type for font
 */
typedef struct
{
    HFONT hfont;
    int width;
    int height;

} font_t;

/*
 * View type
 */
typedef struct
{
    const model_t* model;           ///< pointer to model
    char** lines;                   ///< array with pointers of starts of lines

    unsigned long numOfLines;       ///< number of lines
    unsigned long maxLineLength;    ///< maxumum line length

    unsigned long vScrollPos;       ///< position of vertical scroll
    unsigned long hScrollPos;       ///< position of horizontal scroll

    unsigned long vPageSize;        ///< number of lines on one vertical page
    unsigned long hPageSize;        ///< number of lines on one horizontal page

    unsigned long windowWidth;      ///< window width
    unsigned long windowHeight;     ///<window height

    mode_t mode;                    ///< mode of representation
    font_t font;                    ///< font info

    HWND hwnd;                      ///< window descriptor

} view_t;

/*
 * Initialise view
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] hwnd window descriptor
 */
void ViewInit(view_t* view, HWND hwnd);

/*
 * Bind model to view
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] model pinter to model
 * RETURN code with info about error
 */
error_t ViewBindModel(view_t* view, const model_t* model);

/*
 * Set font
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] fontName font name
 * PARAM[IN] height font height
 */
void ViewSetFont(view_t* view, TCHAR* fontName, unsigned long height);

/*
 * Set layout or raw mode
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] mode RAW or LAYOUT model
 * RETURN code with info about error
 */
error_t ViewSetMode(view_t* view, mode_t mode);

/*
 * Set window size
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] width window width
 * PARAM[IN] height window height
 * RETURN code with info about error
 */
error_t ViewSetWindowSize(view_t* view, unsigned long width, unsigned long height);

/*
 * Scroll view up
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] count number of lines to scroll
 */
void ViewUp(view_t* view, unsigned long count);

/*
 * Scroll view down
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] count number of lines to scroll
 */
void ViewDown(view_t* view, unsigned long count);

/*
 * Scroll view right
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] count number of positions to scroll
 */
void ViewRight(view_t* view, unsigned long count);

/*
 * Scroll view left
 * PARAM[IN|OUT] view pointer to view
 * PARAM[IN] count number of positions to scroll
 */
void ViewLeft(view_t* view, unsigned long count);

/*
 * Scroll view page up
 * PARAM[IN|OUT] view pointer to view
 */
void ViewPageUp(view_t* view);

/*
 * Scroll view page down
 * PARAM[IN|OUT] view pointer to view
 */
void ViewPageDown(view_t* view);

/*
 * Scroll view to the first line
 * PARAM[IN|OUT] view pointer to view
 */
void ViewHome(view_t* view);

/*
 * Scroll view to the last page
 * PARAM[IN|OUT] view pointer to view
 */
void ViewEnd(view_t* view);

/*
 * Draw view
 * PARAM[IN] view pointer to view
 */
void ViewDraw(const view_t* view);

/*
 * Clear view
 * PARAM[IN|OUT] view pointer to view
 */
void ViewClear(view_t* view);

#endif // VIEW_H_INCLUDED
