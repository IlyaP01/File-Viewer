#ifndef CALLBACKS_INCLUDED
#define CALLBACKS_INCLUDED

#include "../model/model.h"
#include "../view/view.h"

/*
 * Handle WM_CREATE message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN|OUT] model pointer to model
 * PARAM[IN|OUT] view pointer to view
 */
void OnCreate(HWND hwnd, model_t* model, view_t* view);

/*
 * Handle WM_DESTROY message
 * PARAM[IN|OUT] model pointer to model
 * PARAM[IN|OUT] view pointer to view
 */
void OnDestroy(model_t* model, view_t* view);

/*
 * Handle WM_SIZE message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] lParam lParam from WindowProcedure
 * PARAM[IN|OUT] view pointer to view
 */
void OnSize(HWND hwnd, LPARAM lParam, view_t* view);

/*
 * Handle WM_MOUSEWHEEL message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] wParam wParam from WindowProcedure
 */
void OnMouseWheel(HWND hwnd, WPARAM wParam);

/*
 * Handle WM_KEYDOWN message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] wParam wParam from WindowProcedure
 * PARAM[IN|OUT] view pointer to view
 */
void OnKeyDown(HWND hwnd, WPARAM wParam, view_t* view);

/*
 * Handle WM_COMMAND message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] wParam wParam from WindowProcedure
 * PARAM[IN|OUT] model pointer to model
 * PARAM[IN|OUT] view pointer to view
 */
void OnCommand(HWND hwnd, WPARAM wParam, model_t* model, view_t* view);

/*
 * Handle WM_VSCROLL message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] wParam wParam from WindowProcedure
 * PARAM[IN|OUT] view pointer to view
 */
void OnVScroll(HWND hwnd, WPARAM wParam, view_t* view);

/*
 * Handle WM_HSCROLL message
 * PARAM[IN] hwnd window descriptor
 * PARAM[IN] wParam wParam from WindowProcedure
 * PARAM[IN|OUT] view pointer to view
 */
void OnHScroll(HWND hwnd, WPARAM wParam, view_t* view);

#endif // CALLBACKS_INCLUDED
