#ifndef CALLBACKS_INCLUDED
#define CALLBACKS_INCLUDED

#include "../model/model.h"
#include "../view/view.h"

void OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnSize(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnMouseWheel(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnKeyDown(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnVScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);
void OnHScroll(HWND hwnd, WPARAM wParam, LPARAM lParam, model_t* model, view_t* view);

#endif // CALLBACKS_INCLUDED
