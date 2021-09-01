#ifndef COMANDO_FDISK_H
#define COMANDO_FDISK_H
#include "string"
using namespace std;


class comando_fdisk
{
public:
    comando_fdisk();
    int size;
    string unit;
    string path;
    string type;
    string fit;
    string coman_delete;
    string name;
    int add;
    void adminParticiones(comando_fdisk *disco);
    void crear_particones(string path, string name, int size, string fit, string unit, string archivo);
    void crearParticionPrimaria(string path, string name, int size, string fit, string unit, string archivo);
    bool existeParticion(string path, string name);
    void crearParticionExtendida(string path, string name, int size, string fit, string unit, string archivo);
};

#endif // COMANDO_FDISK_H
