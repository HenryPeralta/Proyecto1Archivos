#ifndef COMANDO_RMDISK_H
#define COMANDO_RMDISK_H
#include "string"
using namespace std;

class comando_rmdisk
{
public:
    comando_rmdisk();
    string path;
    void borrarDisco(comando_rmdisk *disco);
};

#endif // COMANDO_RMDISK_H
