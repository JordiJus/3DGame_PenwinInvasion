#pragma once
#include "EntityMesh.h"
class snowflake :
    public EntityMesh
{
public:
    int value;
    void Init(Shader* shader);
    void update(float elapsed_time);
};

