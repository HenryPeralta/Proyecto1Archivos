#ifndef COMANDO_MOUNT_H
#define COMANDO_MOUNT_H
#include "string"
using namespace std;


class comando_mount
{
public:
    comando_mount();
    string path;
    string name;
    string ppath;
    string pname;
    void creandoMount(comando_mount *disco);
};

#endif // COMANDO_MOUNT_H
