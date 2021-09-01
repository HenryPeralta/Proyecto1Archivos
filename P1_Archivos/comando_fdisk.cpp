#include "comando_fdisk.h"
#include "estructuras.h"
#include "string"
#include <QString>
#include <stdio.h>
#include <string.h>
#include <iostream>

comando_fdisk::comando_fdisk()
{

}

void comando_fdisk::adminParticiones(comando_fdisk *disco){
    /*printf("\n\n---------DATOS----------\n\n");
    printf("EL tamano es %d \n",disco->size);
    printf("El tipo de unidad es %s \n",disco->unit.c_str());
    printf("La ruta es %s \n",disco->path.c_str());
    printf("El tipo es %s \n",disco->type.c_str());
    printf("El tipo de ajuste es %s \n",disco->fit.c_str());
    printf("El tipo de delete es %s \n",disco->coman_delete.c_str());
    printf("EL name es %s \n",disco->name.c_str());
    printf("EL valor de add es %d \n",disco->add);

    QString cambio_ruta = disco->path.c_str();
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

    FILE *archivo;

    int tamanoparticion = 0;
    archivo = fopen(rutax.c_str(), "rb+"); //se abre el archivo del disco en modo lectura para ver si existe

    if(archivo == NULL){
        std::cout<<"ERROR! \n El disco no existe\n";
        return;
    }

    fseek(archivo, 0, SEEK_SET); //me posiciono en el inicio del archivo
    mbr MBR; //mbr temporal que agarra el mbr que ya esta en el disco
    fread(&MBR, sizeof (mbr),1,archivo); //MBR es donde voy a guardar, sizeof es el tamaño del elemento a leer

    if(disco->unit=="K" || disco->unit=="k" || disco->unit.empty()==true){ //si viene vacia o viene en kb
        tamanoparticion = disco->size*1024;
    }else if(disco->unit=="M" || disco->unit=="m"){
        tamanoparticion = disco->size*1024*1024;
    }else if(disco->unit=="B" || disco->unit=="b"){
        tamanoparticion = disco->size;
    }

    if(disco->fit=="BF" || disco->fit=="bf"){
        MBR.mbr_partitions[0].part_fit = 'B';
    }else if(disco->fit=="FF" || disco->fit=="ff"){
        MBR.mbr_partitions[0].part_fit = 'F';
    }else if(disco->fit=="WF" || disco->fit=="wf" || disco->fit.empty()==true){
        MBR.mbr_partitions[0].part_fit = 'W';
    }

    //Asigno nombre a la particion
    strcpy(MBR.mbr_partitions[0].part_name, disco->name.c_str());

    MBR.mbr_partitions[0].part_size = tamanoparticion;
    MBR.mbr_partitions[0].part_start = sizeof(mbr);
    MBR.mbr_partitions[0].part_status = '1';

    if(disco->type == "P" || disco->type == "p" || disco->type.empty() == true){
        MBR.mbr_partitions[0].part_type = 'P';
    }else if(disco->type == "E" || disco->type == "e"){
        MBR.mbr_partitions[0].part_type = 'E';
        //Acaba de agragar esto del ebr
        ebr auxiliar; //primer ebr
        auxiliar.part_fit='-';
        auxiliar.part_name[0]='\0';
        auxiliar.part_next = -1;
        auxiliar.part_size = -1;
        auxiliar.part_start = -1;
        auxiliar.part_status='0';
        fseek(archivo, MBR.mbr_partitions[0].part_start, SEEK_SET);//Me posiciono al inicio de la particion para meter el ebr
        fwrite(&auxiliar, sizeof(ebr), 1, archivo);
    }else if(disco->type == "L" || disco->type == "l"){
        MBR.mbr_partitions[0].part_type = 'L';
    }

    fseek(archivo, 0, SEEK_SET);//queremos modificar en donde estaba el MBR original
    fwrite(&MBR, sizeof(mbr), 1, archivo);
    fclose(archivo);

    std::cout<<"\n----------DATOS DEL DISCO----------\n";
    std::cout<<"\n MBR NAME: "<< MBR.mbr_disk_signature;
    std::cout<<"\n MBR SIZE: "<< MBR.mbr_tamano;

    for(int i=0; i<4; i++){
        if(MBR.mbr_partitions[i].part_status=='1'){
            std::cout<< "\n PARTICION: "<<i;
            std::cout<< "\n PARTICION STATUS: "<<MBR.mbr_partitions[i].part_status;
            std::cout<< "\n PARTICION TYPE: "<<MBR.mbr_partitions[i].part_type;
            std::cout<< "\n PARTICION FIT: "<<MBR.mbr_partitions[i].part_fit;
            std::cout<< "\n PARTICION START: "<<MBR.mbr_partitions[i].part_start;
            std::cout<< "\n PARTICION SIZE: "<<MBR.mbr_partitions[i].part_size;
            std::cout<< "\n PARTICION NAME: "<<MBR.mbr_partitions[i].part_name;
        }
    }*/

    crearParticionPrimaria(disco->path, disco->name, disco->size, disco->fit, disco->unit, "principal");
}

void comando_fdisk::crear_particones(string path, string name, int size, string fit, string unit, string archivo){
    if(type=="P" || type == "p" || type.empty() == true){
        crearParticionPrimaria(path, name, size, fit, unit, "principal");
    }else if(type=="E" || type == "e"){
        crearParticionExtendida(path, name, size, fit, unit, "principal");
    }
}

void comando_fdisk::crearParticionPrimaria(string path, string name, int size, string fit, string unit, string archivo){
    std::cout<< "\n PATH: "<<path;
    std::cout<< "\n NAME: "<<name;
    std::cout<< "\n SIZE: "<<size;
    std::cout<< "\n FIT: "<<fit;
    std::cout<< "\n UNIT: "<<unit;
    std::cout<< "\n ARCHIVO: "<<archivo;

    QString cambio_ruta = path.c_str();
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

    char auxFit = 0;
    char auxUnit = 0;
    string auxPath = rutax;
    int size_bytes = 1024;
    char buffer = '1';

    if(fit=="bf" || fit=="BF"){
        auxFit='B';
    }
    else if(fit=="ff" || fit=="FF"){
        auxFit='F';
    }
    else if(fit=="wf" || fit=="WF"){
        auxFit='W';
    }

    if(unit=="b" || unit == "B"){
        auxUnit='b';
        size_bytes = size;
    }
    else if(unit=="k" || unit == "K"){
        auxUnit='k';
        size_bytes = size * 1024;
    }
    else if(unit=="m" || unit == "M"){
        auxUnit='m';
        size_bytes = size*1024*1024;
    }

    FILE *Archivo;
    mbr MBR;

    if((Archivo = fopen(auxPath.c_str(), "rb+"))){
        bool flagParticion = false;//Flag para ver si hay una particion disponible
        int numParticion = 0;//Que numero de particion es
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR,sizeof(MBR),1,Archivo);
        //Verificar si existe una particion disponible
        for(int i = 0; i < 4; i++){
            if(MBR.mbr_partitions[i].part_start == -1 || (MBR.mbr_partitions[i].part_status == '1' && MBR.mbr_partitions[i].part_size>=size_bytes)){
                flagParticion = true;
                numParticion = i;
                break;
            }
        }
        if(flagParticion){
            //Verificar el espacio libre del disco
            int espacioUsado = 0;
            for(int i = 0; i < 4; i++){
                if(MBR.mbr_partitions[i].part_status!='1'){
                    espacioUsado += MBR.mbr_partitions[i].part_size;
                }
            }
            if(archivo == "principal"){
                cout << "Espacio Disponible: " << (MBR.mbr_tamano - espacioUsado) << " Bytes" << endl;
                cout << "Espacio Requerido:  " << size_bytes << " Bytes" << endl;
            }
            //Verificar que haya espacio suficiente para crear la particion
            if((MBR.mbr_tamano - espacioUsado) >= size_bytes){
                if(!existeParticion(path,name)){
                    if(MBR.disk_fit == 'F'){//FIRST FIT
                        MBR.mbr_partitions[numParticion].part_type = 'P';
                        MBR.mbr_partitions[numParticion].part_fit = auxFit;
                        //start
                        if(numParticion == 0){
                            MBR.mbr_partitions[numParticion].part_start = sizeof(MBR);
                        }else{
                            MBR.mbr_partitions[numParticion].part_start = MBR.mbr_partitions[numParticion-1].part_start + MBR.mbr_partitions[numParticion-1].part_size;
                        }
                        MBR.mbr_partitions[numParticion].part_size = size_bytes;
                        MBR.mbr_partitions[numParticion].part_status = '0';
                        strcpy(MBR.mbr_partitions[numParticion].part_name,name.c_str());
                        //Se guarda de nuevo el MBR
                        fseek(Archivo,0,SEEK_SET);
                        fwrite(&MBR,sizeof(MBR),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[numParticion].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        std::cout<< "Llego al final ";
                        if(archivo == "principal") cout << "ejecutando...\n" << "\033[96mParticion primaria creada con exito. FF\033[0m" <<  endl;
                    }else if(MBR.disk_fit == 'B'){//BEST FIT
                        int bestIndex = numParticion;
                        for(int i = 0; i < 4; i++){
                            if(MBR.mbr_partitions[i].part_start == -1 || (MBR.mbr_partitions[i].part_status == '1' && MBR.mbr_partitions[i].part_size>=size_bytes)){
                                if(i != numParticion){
                                    if(MBR.mbr_partitions[bestIndex].part_size > MBR.mbr_partitions[i].part_size){
                                        bestIndex = i;
                                        break;
                                    }
                                }
                            }
                        }
                        MBR.mbr_partitions[bestIndex].part_type = 'P';
                        MBR.mbr_partitions[bestIndex].part_fit = auxFit;
                        //start
                        if(bestIndex == 0){
                            MBR.mbr_partitions[bestIndex].part_start = sizeof(MBR);
                        }else{
                            MBR.mbr_partitions[bestIndex].part_start = MBR.mbr_partitions[bestIndex-1].part_start + MBR.mbr_partitions[bestIndex-1].part_size;
                        }
                        MBR.mbr_partitions[bestIndex].part_size = size_bytes;
                        MBR.mbr_partitions[bestIndex].part_status = '0';
                        strcpy(MBR.mbr_partitions[bestIndex].part_name,name.c_str());
                        //Se guarda de nuevo el MBR
                        fseek(Archivo,0,SEEK_SET);
                        fwrite(&MBR,sizeof(MBR),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[bestIndex].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        if(archivo == "principal") cout << "...\n" << "\033[96mParticion primaria creada con exito. BF\033[0m" <<  endl;
                    }else if(MBR.disk_fit == 'W'){//WORST FIT
                        int  worstIndex= numParticion;
                        for(int i = 0; i < 4; i++){
                            if(MBR.mbr_partitions[i].part_start == -1 || (MBR.mbr_partitions[i].part_status == '1' && MBR.mbr_partitions[i].part_size>=size_bytes)){
                                if(i != numParticion){
                                    if(MBR.mbr_partitions[worstIndex].part_size < MBR.mbr_partitions[i].part_size){
                                        worstIndex = i;
                                        break;
                                    }
                                }
                            }
                        }
                        MBR.mbr_partitions[worstIndex].part_type = 'P';
                        MBR.mbr_partitions[worstIndex].part_fit = auxFit;
                        //start
                        if(worstIndex == 0){
                            MBR.mbr_partitions[worstIndex].part_start = sizeof(MBR);
                        }else{
                            MBR.mbr_partitions[worstIndex].part_start = MBR.mbr_partitions[worstIndex-1].part_start + MBR.mbr_partitions[worstIndex-1].part_size;
                        }
                        MBR.mbr_partitions[worstIndex].part_size = size_bytes;
                        MBR.mbr_partitions[worstIndex].part_status = '0';
                        strcpy(MBR.mbr_partitions[worstIndex].part_name,name.c_str());
                        //Se guarda de nuevo el MBR
                        fseek(Archivo,0,SEEK_SET);
                        fwrite(&MBR,sizeof(MBR),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[worstIndex].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        if(archivo == "principal") cout << "...\n" << "\033[96mParticion primaria creada con exito. WF\033[0m" <<  endl;
                    }
                }else{
                    cout << "\033[31mERROR: ya existe una particion con ese nombre.\033[0m" << endl;
                }

            }else{
                cout << "\033[31mERROR: la particion a crear excede el espacio libre.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: Ya existen 4 particiones, no se puede crear otra" << endl;
            cout << "Elimine alguna para poder crear una.\033[0m" << endl;
        }
    fclose(Archivo);
    }
}


bool comando_fdisk::existeParticion(string path, string name){
    int extendida = -1;
    FILE *Archivo;
    if((Archivo = fopen(path.c_str(),"rb+"))){
        mbr MBR;
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR, sizeof(MBR),1,Archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(MBR.mbr_partitions[i].part_name,name.c_str()) == 0){
                fclose(Archivo);
                return true;
            }else if(MBR.mbr_partitions[i].part_type == 'E'){
                extendida = i;
            }
        }
        if(extendida != -1){
            fseek(Archivo, MBR.mbr_partitions[extendida].part_start,SEEK_SET);
            ebr EBR;
            while((fread(&EBR,sizeof(ebr),1,Archivo))!=0 && (ftell(Archivo) < (MBR.mbr_partitions[extendida].part_size + MBR.mbr_partitions[extendida].part_start))){
                if(strcmp(EBR.part_name,name.c_str()) == 0){
                    fclose(Archivo);
                    return true;
                }
                if(EBR.part_next == -1){
                    fclose(Archivo);
                    return false;
                }
            }
        }
    }
    fclose(Archivo);
    return false;
}
