#ifndef LISTA_PARTICIONES_H
#define LISTA_PARTICIONES_H
#include "tipo_particiones.h"
#include <QObject>
#include <iostream>
using namespace std;

class lista_particiones
{
public:
    lista_particiones();
    tipo_particiones *primero;
    int buscarLetra(QString path, QString nombre);
    int buscarNumero(QString path, QString nombre);
    void mostrarLista();
    void insertarNodo(tipo_particiones*);
    int eliminarNodo(QString ID);
    bool buscarNodo(QString path, QString nombre);
    QString getDireccion(QString id);

    tipo_particiones getParticionMontada(QString pid);
    bool estavacio();
};

#endif // LISTA_PARTICIONES_H
