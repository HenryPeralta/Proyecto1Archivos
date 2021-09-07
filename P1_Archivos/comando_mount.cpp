#include "comando_mount.h"
#include "estructuras.h"
#include "string"
#include <QString>
#include <stdio.h>
#include <string.h>
#include <iostream>

comando_mount::comando_mount()
{

}

void comando_mount::creandoMount(comando_mount *disco){
    printf("La ruta es %s \n",disco->path.c_str());
    printf("La Palabra es: %s \n",disco->ppath.c_str());
    printf("El name es %s \n",disco->name.c_str());
    printf("El name es: %s \n",disco->pname.c_str());

    bool validacion = false;

    if(disco->ppath == "path"){
        if(disco->path.empty()){
            std::cout<<"Faltan valores del path \n";
            validacion = true;
        }
    }else{
        std::cout<<"El parametro -path es obligatorio \n";
        validacion = true;
    }

    if(disco->pname == "name"){
        if(disco->name.empty()){
            std::cout<<"Faltan valores del name \n";
            validacion = true;
        }
    }else{
        std::cout<<"El parametro -name es obligatorio \n";
        validacion = true;
    }

    if(validacion == false){
        std::cout<<"HOLA \n";
    }
}
