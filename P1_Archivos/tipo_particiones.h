#ifndef TIPO_PARTICIONES_H
#define TIPO_PARTICIONES_H
#include <QString>
#include "string"
#include "estructuras.h"
#include <QObject>
using namespace std;

class tipo_particiones
{
public:
    tipo_particiones();
    tipo_particiones(QString path, QString nombre, char, int, char part_type);
    QString path;
    QString nombre;
    char letra;
    int num;
    tipo_particiones *siguiente;
    char part_type; //Tipo de particion: Primaria(p), Extendida(e) o Logica(l)

    particion particiones; //La partición se guardará aquí, si es de tipo 'p' o 'e'
    ebr particion_logica; //La partición se guardará aquí, si es de tipo 'l'
};

#endif // TIPO_PARTICIONES_H
