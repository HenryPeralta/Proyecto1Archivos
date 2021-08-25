#include "comando_mkdisk.h"
#include "estructuras.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "string"
using namespace std;

comando_mkdisk::comando_mkdisk()
{

}

void comando_mkdisk::crearDisco(comando_mkdisk *disco){

    printf("\n\n---------DATOS----------\n\n");

    printf("EL tamano es %d \n",disco->size);
    printf("El tipo de unidad es %s \n",disco->unit.c_str());
    printf("La ruta es %s \n",disco->path.c_str());
    printf("El tipo de ajuste es %s \n",disco->fit.c_str());

    mbr prueba;
    FILE *archivo;
    archivo = fopen(disco->path.c_str(), "wb");
    if(archivo == NULL){
        exit(1);
    }
    //Simulando un Kb
    char buffer[1024];

    //Si son Kb
    if(disco->unit == "k"){
        //Este if es para el tamaño
        prueba.mbr_tamano = disco->size*1024;

        for(int i = 0; i<1024 ; i++){
            buffer[i] = '\0';
        }

        for(int i = 0; i<(disco->size); i++){
            fwrite(&buffer, 1024, 1, archivo);
        }
        fclose(archivo);
    }

    //Si son megas
    else if(disco->unit == "m" || disco->unit.empty() == 1){
        prueba.mbr_tamano = disco->size*1024*1024; //En bytes
        for(int i = 0; i<1024; i++){
            buffer[i] = '\0';
        }
        for(int i = 0; i<(disco ->size*1024); i++){
            fwrite(&buffer, 1024, 1, archivo);
        }
        fclose(archivo);
    }

    //Etiquetas para el disco

    //Para la fecha actual
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    disco ->fechacreacion = buf;

    //Datos para el mrb
    prueba.mbr_disk_signature = (rand()% 100); //Numero random para la etiqueta del disco
    strcpy(prueba.mbr_fecha_creacion, buf); //Se convierte a char para guardarlo en el mrb

    //Si el fit viene vacio se asigna por defecto primer ajuste
    if(disco ->fit.empty() == true){
        strcpy(prueba.disk_fit, "F"); //Se convierte a char para guardarlo en el mrb
    }else{
        strcpy(prueba.disk_fit, disco->fit.c_str());//Se convierte a char para guardarlo en el mrb
    }

    printf("\n FECHA DE CREACION: %s\n", prueba.mbr_fecha_creacion);

    particion vacia;
    vacia.part_status = '0'; // Estatus inactivo
    vacia .part_type = '-';
    vacia.part_fit = '-';
    vacia.part_start = -1;
    vacia.part_size = -1;
    vacia.part_name[0] = '\0';
    for(int i = 0; i<4; i++){
        prueba.mbr_partitions[i] = vacia;
    }

    //Agregamos el mrb al disco
    archivo = fopen(disco->path.c_str(), "rb+"); //Modo de escritura mixto permite actualizar un fichero sin borrar el contenido
    if(archivo == NULL){
        fseek(archivo, 0, SEEK_SET);
        fwrite(&prueba, sizeof(mbr), 1, archivo);
        fclose(archivo);
        printf("DISCO CREADO CORRECTAMENTE \n SE AGREGO EL MBR DE MANERA CORRECTA\n");
    }
}

/*void comando_mkdisk::crearDisco(comando_mkdisk *disco){

          printf("\n\n---------DATOS----------\n\n");

          printf("EL tamano es %d \n",disco->size);
          printf("El tipo de unidad es %s \n",disco->unit.c_str());
          printf("La ruta es %s \n",disco->path.c_str());
          printf("El tipo de ajuste es %s \n",disco->fit.c_str());

}*/
