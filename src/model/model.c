#include "model.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void ModelInit(model_t* model)
{
    assert(model);
    model->data = NULL;
    model->size = 0;
}

error_t ModelBuild(model_t* model, const char* fileName)
{
    assert(model);

    FILE* file = fopen(fileName, "rb");
    if (!file)
        return OPEN_FILE_ERR;

    fseek(file, 0, SEEK_END);
    model->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    free(model->data);
    model->data = malloc(sizeof(char) * (model->size + 1));
    if (!model->data)
    {
        fclose(file);
        return MEMORY_ERR;
    }

    fread(model->data, sizeof(char), model->size, file);
    model->data[model->size] = '\0';

    fclose(file);

    return SUCCESS;
}

void ModelClear(model_t* model)
{
    assert(model);

    free(model->data);
    model->size = 0;
}
