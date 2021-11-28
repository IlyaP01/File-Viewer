#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#pragma once

#include "../error.h"

typedef struct
{
  char* data;           ///<< array of symbols from file
  unsigned long size;   ///<< size of data
} model_t;

/*
 * Initialise model
 * PAARAM[IN|OUT] model pointer to model
 */
void ModelInit(model_t* model);

/*
 * Open file and create model for data
 * PARAM[IN|OUT] model model to create
 * PARAM[IN] fileName name of file with data
 * RETURN error code
 */
error_t ModelBuild(model_t* model, const char* fileName);

/*
 * Clear model and free memory
 * PARAM[IN|OUT] model model to clear
 */
void ModelClear(model_t* model);

#endif // MODEL_H_INCLUDED
