#ifndef COMANDO_MOUNT_H
#define COMANDO_MOUNT_H
#include "string"
#include "estructuras.h"
#include "lista_particiones.h"
#include "tipo_particiones.h"
#include <QObject>
#include <stdio.h>
#include <QtCore>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>

using namespace std;


class comando_mount
{
public:
    comando_mount();
    lista_particiones *lista;
    string path;
    string name;
    string ppath;
    string pname;
    void creandoMount(comando_mount *disco);
    void ejecutarMount(QString path, QString name);
    int buscarParticion_P_E(QString path, QString name);
    int buscarParticion_L(QString path, QString name);
};

#endif // COMANDO_MOUNT_H
