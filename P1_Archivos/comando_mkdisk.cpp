#include "comando_mkdisk.h"
#include "estructuras.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "string"
#include <QString>
using namespace std;

comando_mkdisk::comando_mkdisk()
{

}

void comando_mkdisk::crearDisco(comando_mkdisk *disco){
    bool validacion = false;

    if(disco->psize == "size"){
        if(disco->size <= 0){
            std::cout<<"Los valores de size no pueden ser menores o iguales a cero \n";
            validacion = true;
        }
    }else{
        std::cout<<"El parametro -size es obligatorio \n";
        validacion = true;
    }

    if(disco->ppath == "path"){
        if(disco->path.empty()){
            std::cout<<"Faltan valores del path \n";
            validacion = true;
        }
    }else{
        std::cout<<"El parametro -path es obligatorio \n";
        validacion = true;
    }

    if(validacion == false){
        ejecutarDisco(disco->size, disco->fechacreacion, disco->unit, disco->path.c_str(), disco->fit);
    }
}

void comando_mkdisk::ejecutarDisco(int size, string fechacreacion, string unit, QString path, string fit){
    printf("\n\n---------DATOS----------\n\n");

    printf("EL tamano es %d \n",size);
    printf("El tipo de unidad es %s \n",unit.c_str());
    printf("La ruta es %s \n",path.toStdString().c_str());
    printf("El tipo de ajuste es %s \n",fit.c_str());

    QString cambio_ruta = path;
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

    printf("La ruta nueva es: %s \n",rutax.c_str());

    mbr prueba;
    creandoRuta(cambio_ruta);

    FILE *archivo;
    archivo = fopen(rutax.c_str(), "rb+");

    //Simulando un Kb
    char buffer[1024];

    //Si son Kb
    if(unit == "k" || unit == "K"){
        //Este if es para el tamaño
        prueba.mbr_tamano = size*1024;

        for(int i = 0; i<1024 ; i++){
            buffer[i] = '\0';
        }

        for(int i = 0; i<(size); i++){
            fwrite(&buffer, 1024, 1, archivo);
        }
        fclose(archivo);
    }

    //Si son megas
    else if(unit == "m" || unit == "M" || unit.empty() == 1){
        prueba.mbr_tamano = size*1024*1024; //En bytes

        for(int i = 0; i<1024; i++){
            buffer[i] = '\0';
        }
        for(int i = 0; i<(size*1024); i++){
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
    fechacreacion = buf;

    //Datos para el mrb
    prueba.mbr_disk_signature = (rand()% 100); //Numero random para la etiqueta del disco
    strcpy(prueba.mbr_fecha_creacion, buf); //Se convierte a char para guardarlo en el mrb

    //Si el fit viene vacio se asigna por defecto primer ajuste
    char auxfit = 0;
    if(fit == "BF" || fit == "bf"){
        //strcpy(prueba.disk_fit, "F"); //Se convierte a char para guardarlo en el mrb
        auxfit = 'B';
    }else if(fit == "FF" || fit == "ff" || fit.empty()==true){
        //strcpy(prueba.disk_fit, disco->fit.c_str());//Se convierte a char para guardarlo en el mrb
        auxfit = 'F';
    }else if(fit == "WF" || fit == "wf"){
        auxfit = 'W';
    }

    prueba.disk_fit = auxfit;

    printf("\nFECHA DE CREACION: %s\n", prueba.mbr_fecha_creacion);

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
    archivo = fopen(rutax.c_str(), "rb+"); //Modo de escritura mixto permite actualizar un fichero sin borrar el contenido
    fseek(archivo, 0, SEEK_SET);
    fwrite(&prueba, sizeof(mbr), 1, archivo);
    fclose(archivo);
    printf("DISCO CREADO CORRECTAMENTE \nSE AGREGO EL MBR DE MANERA CORRECTA\n");
}

void comando_mkdisk::creandoRuta(QString path){
    QString aux = obtener_path(path);
    string comando = "sudo mkdir -p \'"+aux.toStdString()+"\'";
    system(comando.c_str());
    string comando2 = "sudo chmod -R 777 \'"+aux.toStdString()+"\'";
    system(comando2.c_str());
    string arch = path.toStdString();
    FILE *archivo = fopen(arch.c_str(),"wb");
    if((archivo = fopen(arch.c_str(),"wb")))
        fclose(archivo);
    else
        cout << "\033[31mERROR: Al crear el Disco.\033[0m" << endl;
}

QString comando_mkdisk::obtener_path(QString path){
    /*QString cambio_ruta = path;
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();*/

    string aux = path.toStdString();
    string delimiter = "/";
    size_t pos = 0;
    string res = "";
    while((pos = aux.find(delimiter))!=string::npos){
        res += aux.substr(0,pos)+"/";
        aux.erase(0,pos + delimiter.length());
    }
    return QString::fromStdString(res);
}
