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
    string psize;
    string punit;
    string ppath;
    string ptype;
    string pfit;
    string pcoman_delete;
    string pname;
    string padd;
    void adminParticiones(comando_fdisk *disco);
    void crear_particones(string path, string name, int size, string fit, string unit, string archivo);
    void crearParticionPrimaria(string path, string name, int size, string fit, string unit, string archivo);
    bool existeParticion(string path, string name);
    void crearParticionExtendida(string path, string name, int size, string fit, string unit, string archivo);
    void crearParticionLogica(string path, string name, int size, string fit, string unit, string archivo);
    void metodoAdd(string path, string name, int add, string unit, string archivo);
    int buscarParticion_L(string path, string name);
    void eliminarParticion(string path, string name, string pcoman_delete, string archivo);
};

#endif // COMANDO_FDISK_H
