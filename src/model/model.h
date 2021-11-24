#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#pragma once

#include "../error.h"

typedef struct
{
  char* data;
  unsigned long size;
} model_t;

void ModelInit(model_t* model);
error_t ModelBuild(model_t* model, const char* fileName);
void ModelClear(model_t* model);

#endif // MODEL_H_INCLUDED
