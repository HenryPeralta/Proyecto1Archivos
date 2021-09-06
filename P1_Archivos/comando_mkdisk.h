#ifndef COMANDO_MKDISK_H
#define COMANDO_MKDISK_H
#include "string"
#include <QString>
using namespace std;

class comando_mkdisk
{
public:
    comando_mkdisk();
    int size;
    string fechacreacion;
    string unit;
    string path;
    string fit;
    string psize;
    string punit;
    string ppath;
    string pfit;
    void crearDisco(comando_mkdisk *disco);
    void ejecutarDisco(int size, string fechacreacion, string unit, QString path, string fit);
    void creandoRuta(QString path);
    QString obtener_path(QString);
};

#endif // COMANDO_MKDISK_H
