#ifndef COMANDO_MKDISK_H
#define COMANDO_MKDISK_H
#include "string"
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
    void crearDisco(comando_mkdisk *disco);
};

#endif // COMANDO_MKDISK_H
