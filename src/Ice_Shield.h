#pragma once
#include "EntityMesh.h"
class Ice_Shield :
    public EntityMesh
{
public:
    const int Hp_MAX = 250;
    int Hp;
    int row;
    int column;
    bool reduced;
    void Init(Shader* shader);
    void update(float elapsed_time);
    void getDamage(int damage);
};

