#include "tipo_particiones.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
#include "string"

tipo_particiones::tipo_particiones()
{
    path="";
    nombre="";
    letra=0;
    num=0;
    siguiente=nullptr;
    part_type=0;
}

tipo_particiones::tipo_particiones(QString path, QString nombre, char letra, int num, char part_type)
{
    this->path = path;
    this->nombre = nombre;
    this->letra = letra;
    this->num = num;
    this->siguiente = nullptr;
    this->part_type=part_type;
    cout << path.toStdString() << endl;
}

