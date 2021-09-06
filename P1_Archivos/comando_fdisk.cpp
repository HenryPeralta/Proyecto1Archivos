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
    printf("La Palabra es: %s \n",disco->psize.c_str());
    printf("La Palabra es: %s \n",disco->punit.c_str());
    printf("La Palabra es: %s \n",disco->ppath.c_str());
    printf("La Palabra es: %s \n",disco->ptype.c_str());
    printf("La Palabra es: %s \n",disco->pfit.c_str());
    printf("La Palabra es: %s \n",disco->pcoman_delete.c_str());
    printf("La Palabra es: %s \n",disco->pname.c_str());
    printf("La Palabra es: %s \n",disco->padd.c_str());

    bool bandera_type = false;
    bool bandera_size = false;
    bool bandera_delete = false;
    bool bandera_add = false;
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

        if(disco->psize == "size"){
            if(disco->size <= 0){
                std::cout<<"Los valores de size no pueden ser menores o iguales a cero \n";
                bandera_size = true;
            }
        }else{
            bandera_size = true;
        }

        if(disco->pcoman_delete == "delete"){
            if(disco->coman_delete.empty()){
                std::cout<<"Faltan valores del delete \n";
                bandera_delete = true;
            }
        }else{
            bandera_delete = true;
        }

        if(disco->ptype == "type"){
            if(disco->type.empty()){
                std::cout<<"Faltan valores del type \n";
                bandera_type = true;
            }
        }else{
            bandera_type = true;
        }

        if(disco->padd == "add"){
            if(disco->add == 0){
                std::cout<<"El valor de add no puede ser cero \n";
                bandera_add = true;
            }
        }else{
            bandera_add = true;
        }

        if(bandera_size == false){
            if(bandera_delete == true && bandera_add == true){
                crear_particones(disco->path, disco->name, disco->size, disco->fit, disco->unit, "principal");
            }else{
                std::cout<<"Se esta creando una particion, los Parametros -delete, -add no estan permitidos.\n";
            }
        }else if(bandera_add == false){
            if(bandera_delete == true && bandera_size == true && bandera_type == true){
                metodoAdd(disco->path, disco->name, disco->add, disco->unit, "principal");
            }else{
                std::cout<<"Se esta modificando el tamaño una particion, los Parametros -delete, -size no estas permitidos.\n";
            }
        }else if(bandera_delete == false){
            if(bandera_add == true && bandera_size == true || bandera_type == true){

            }else{
                std::cout<<"Se esta eliminando una particion, los Parametros -size, -add, -f, -type no estan permitidos.\n";
            }
        }
    }
}

void comando_fdisk::crear_particones(string path, string name, int size, string fit, string unit, string archivo){
    if(type=="P" || type == "p" || type.empty() == true){
        crearParticionPrimaria(path, name, size, fit, unit, "principal");
    }else if(type=="E" || type == "e"){
        crearParticionExtendida(path, name, size, fit, unit, "principal");
    }else if(type=="L" || type == "l"){
        crearParticionLogica(path, name, size, fit, unit, "principal");
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
    else if(fit=="wf" || fit=="WF" || fit.empty()){
        auxFit='W';
    }

    if(unit=="b" || unit == "B"){
        auxUnit='b';
        size_bytes = size;
    }
    else if(unit=="k" || unit == "K" || unit.empty()){
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
        fread(&MBR,sizeof(mbr),1,Archivo);
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
                        fwrite(&MBR,sizeof(mbr),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[numParticion].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        if(archivo == "principal") cout << "ejecutando...\n" << "\033[32mPARTICION PRIMARIA CREADA CON EXITO.\033[0m" <<  endl;
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
                        fwrite(&MBR,sizeof(mbr),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[bestIndex].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        if(archivo == "principal") cout << "...\n" << "\033[32mPARTICION PRIMARIA CREADA CON EXITO..\033[0m" <<  endl;
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
                        fwrite(&MBR,sizeof(mbr),1,Archivo);
                        //Se guardan los bytes de la particion
                        fseek(Archivo,MBR.mbr_partitions[worstIndex].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,Archivo);
                        }
                        if(archivo == "principal") cout << "...\n" << "\033[32mPARTICION PRIMARIA CREADA CON EXITO..\033[0m" <<  endl;
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

void comando_fdisk::crearParticionExtendida(string path, string name, int size, string fit, string unit, string archivo){
    QString cambio_ruta = path.c_str();
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

    char auxFit = 0;
    char auxUnit = 0;
    string auxPath = rutax;
    int size_bytes = 1024;
    char buffer = '1';

    if(fit == "BF" || fit == "bf"){
        auxFit='B';
    }
    else if(fit == "FF" || fit == "ff"){
        auxFit='F';
    }
    else if(fit == "WF" || fit == "wf" || fit.empty()){
        auxFit='W';
    }

    if(unit == "B" || unit == "b"){
        auxUnit='b';
        size_bytes = size;
    }
    else if(unit == "K" || unit == "k" || unit.empty()){
        auxUnit='k';
        size_bytes = size * 1024;
    }
    else if(unit == "M" || unit == "m"){
        auxUnit='m';
        size_bytes = size*1024*1024;
    }

    FILE *Archivo;
    mbr MBR;
    if((Archivo = fopen(auxPath.c_str(), "rb+"))){
        bool flagParticion = false;//Flag para ver si hay una particion disponible
        bool flagExtendida = false;//Flag para ver si ya hay una particion extendida
        int numParticion = 0;//Que numero de particion es
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR,sizeof(mbr),1,Archivo);
        for(int i = 0; i < 4; i++){
            if (MBR.mbr_partitions[i].part_type == 'E'){
                flagExtendida = true;
                break;
            }
        }
        if(!flagExtendida){
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
                    cout << "Espacio disponible: " << (MBR.mbr_tamano - espacioUsado) <<" Bytes"<< endl;
                    cout << "Espacio Requerido:  " << size_bytes << " Bytes" << endl;
                }
                //Verificar que haya espacio suficiente para crear la particion
                if((MBR.mbr_tamano - espacioUsado) >= size_bytes){
                    if(!(existeParticion(path,name))){
                        if(MBR.disk_fit == 'F'){
                            MBR.mbr_partitions[numParticion].part_type = 'E';
                            MBR.mbr_partitions[numParticion].part_fit = auxFit;
                            //start
                            if(numParticion == 0){
                                MBR.mbr_partitions[numParticion].part_start = sizeof(MBR);
                            }else{
                                MBR.mbr_partitions[numParticion].part_start =  MBR.mbr_partitions[numParticion-1].part_start + MBR.mbr_partitions[numParticion-1].part_size;
                            }
                            MBR.mbr_partitions[numParticion].part_size = size_bytes;
                            MBR.mbr_partitions[numParticion].part_status = '0';
                            strcpy(MBR.mbr_partitions[numParticion].part_name,name.c_str());
                            //Se guarda de nuevo el MBR
                            fseek(Archivo,0,SEEK_SET);
                            fwrite(&MBR,sizeof(mbr),1,Archivo);
                            //Se guarda la particion extendida
                            fseek(Archivo, MBR.mbr_partitions[numParticion].part_start,SEEK_SET);
                            ebr EBR;
                            EBR.part_fit = auxFit;
                            EBR.part_status = '0';
                            EBR.part_start = MBR.mbr_partitions[numParticion].part_start;
                            EBR.part_size = 0;
                            EBR.part_next = -1;
                            strcpy(EBR.part_name, "");
                            fwrite(&EBR,sizeof (ebr),1,Archivo);
                            for(int i = 0; i < (size_bytes - (int)sizeof(ebr)); i++){
                                fwrite(&buffer,1,1,Archivo);
                            }
                            if(archivo == "principal") cout << "Ejecutando...\n" << "\033[32mPARTICION EXTENDIDA CREADA CON EXITO.\033[0m"<< endl;
                        }else if(MBR.disk_fit == 'B'){
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
                            MBR.mbr_partitions[bestIndex].part_type = 'E';
                            MBR.mbr_partitions[bestIndex].part_fit = auxFit;
                            //start
                            if(bestIndex == 0){
                                MBR.mbr_partitions[bestIndex].part_start = sizeof(MBR);
                            }else{
                                MBR.mbr_partitions[bestIndex].part_start =  MBR.mbr_partitions[bestIndex-1].part_start + MBR.mbr_partitions[bestIndex-1].part_size;
                            }
                            MBR.mbr_partitions[bestIndex].part_size = size_bytes;
                            MBR.mbr_partitions[bestIndex].part_status = '0';
                            strcpy(MBR.mbr_partitions[bestIndex].part_name,name.c_str());
                            //Se guarda de nuevo el MBR
                            fseek(Archivo,0,SEEK_SET);
                            fwrite(&MBR,sizeof(mbr),1,Archivo);
                            //Se guarda la particion extendida
                            fseek(Archivo, MBR.mbr_partitions[bestIndex].part_start,SEEK_SET);
                            ebr EBR;
                            EBR.part_fit = auxFit;
                            EBR.part_status = '0';
                            EBR.part_start = MBR.mbr_partitions[bestIndex].part_start;
                            EBR.part_size = 0;
                            EBR.part_next = -1;
                            strcpy(EBR.part_name, "");
                            fwrite(&EBR,sizeof (ebr),1,Archivo);
                            for(int i = 0; i < (size_bytes - (int)sizeof(ebr)); i++){
                                fwrite(&buffer,1,1,Archivo);
                            }
                            if(archivo == "principal") cout << "Ejecutando...\n" << "\033[32mPARTICION EXTENDIDA CREADA CON EXITO.\033[0m"<< endl;
                        }else if(MBR.disk_fit == 'W'){
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
                            MBR.mbr_partitions[worstIndex].part_type = 'E';
                            MBR.mbr_partitions[worstIndex].part_fit = auxFit;
                            //start
                            if(worstIndex == 0){
                                MBR.mbr_partitions[worstIndex].part_start = sizeof(MBR);
                            }else{
                                MBR.mbr_partitions[worstIndex].part_start =  MBR.mbr_partitions[worstIndex-1].part_start + MBR.mbr_partitions[worstIndex-1].part_size;
                            }
                            MBR.mbr_partitions[worstIndex].part_size = size_bytes;
                            MBR.mbr_partitions[worstIndex].part_status = '0';
                            strcpy(MBR.mbr_partitions[worstIndex].part_name, name.c_str());
                            //Se guarda de nuevo el MBR
                            fseek(Archivo,0,SEEK_SET);
                            fwrite(&MBR,sizeof(mbr),1,Archivo);
                            //Se guarda la particion extendida
                            fseek(Archivo, MBR.mbr_partitions[worstIndex].part_start,SEEK_SET);
                            ebr EBR;
                            EBR.part_fit = auxFit;
                            EBR.part_status = '0';
                            EBR.part_start = MBR.mbr_partitions[worstIndex].part_start;
                            EBR.part_size = 0;
                            EBR.part_next = -1;
                            strcpy(EBR.part_name, "");
                            fwrite(&EBR,sizeof (ebr),1,Archivo);
                            for(int i = 0; i < (size_bytes - (int)sizeof(ebr)); i++){
                                fwrite(&buffer,1,1,Archivo);
                            }
                            if(archivo == "principal") cout << "Ejecutando...\n" << "\033[32mPARTICION EXTENDIDA CREADA CON EXITO.\033[0m"<< endl;
                        }
                    }else{
                        cout << "\033[31mERROR: ya existe una particion con ese nombre.\033[0m" << endl;
                    }
                }else{
                    cout << "\033[31mERROR: la particion a crear excede el tamano libre.\033[0m" << endl;
                }
            }else{
                cout << "\033[31mERROR: Ya existen 4 particiones, no se puede crear otra." << endl;
                cout << "Elimine alguna para poder crear una.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: ya existe una particion extendida en este disco.\033[0m" << endl;
        }
    fclose(Archivo);
    }else{
        cout << "\033[31mERROR: no existe el disco.\033[0m" << endl;
    }
}

void comando_fdisk::crearParticionLogica(string path, string name, int size, string fit, string unit, string archivo){
    QString cambio_ruta = path.c_str();
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

    char auxFit = 0;
    char auxUnit = 0;
    string auxPath = rutax;
    int size_bytes = 1024;
    char buffer = '1';

    if(fit == "BF" || fit == "bf"){
        auxFit='B';
    }
    else if(fit == "FF" || fit == "ff"){
        auxFit='F';
    }
    else if(fit == "WF" || fit=="wf" || fit.empty()){
        auxFit='W';
    }

    if(unit == "B" || unit == "b"){
        auxUnit='b';
        size_bytes = size;
    }
    else if(unit == "K" || unit == "k" || unit.empty()){
        auxUnit='k';
        size_bytes = size * 1024;
    }
    else if(unit == "M" || unit == "m"){
        auxUnit='m';
        size_bytes = size*1024*1024;
    }

    FILE *Archivo;
    mbr MBR;
    if((Archivo = fopen(auxPath.c_str(), "rb+"))){
        int numExtendida = -1;
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR,sizeof(mbr),1,Archivo);
        //Verificar si existe una particion extendida
        for(int i = 0; i < 4; i++){
            if(MBR.mbr_partitions[i].part_type == 'E'){
                numExtendida = i;
                break;
            }
        }
        if(!existeParticion(path,name)){
            if(numExtendida != -1){
                ebr EBR;
                int cont = MBR.mbr_partitions[numExtendida].part_start;
                fseek(Archivo,cont,SEEK_SET);
                fread(&EBR, sizeof(ebr),1,Archivo);
                if(EBR.part_size == 0){//Si es la primera
                    if(MBR.mbr_partitions[numExtendida].part_size < size_bytes){
                        if(archivo == "principal") cout << "\033[31mERROR: la particion logica a crear excede el espacio disponible de la particion extendida.\033[0m" << endl;
                    }else{
                        EBR.part_status = '0';
                        EBR.part_fit = auxFit;
                        EBR.part_start = ftell(Archivo) - sizeof(ebr); //Para regresar al inicio de la extendida
                        EBR.part_size = size_bytes;
                        EBR.part_next = -1;
                        strcpy(EBR.part_name, name.c_str());
                        fseek(Archivo, MBR.mbr_partitions[numExtendida].part_start ,SEEK_SET);
                        fwrite(&EBR,sizeof(ebr),1,Archivo);
                        if(archivo == "principal") cout << "Ejecutando...\n\033[32mPARTICION LOGICA CREADA CON EXITO.\033[0m"<< endl;
                    }
                }else{
                    while((EBR.part_next != -1) && (ftell(Archivo) < (MBR.mbr_partitions[numExtendida].part_size + MBR.mbr_partitions[numExtendida].part_start))){
                        fseek(Archivo,EBR.part_next,SEEK_SET);
                        fread(&EBR,sizeof(ebr),1,Archivo);
                    }
                    int espacioNecesario = EBR.part_start + EBR.part_size + size_bytes;
                    if(espacioNecesario <= (MBR.mbr_partitions[numExtendida].part_size + MBR.mbr_partitions[numExtendida].part_start)){
                        EBR.part_next = EBR.part_start + EBR.part_size;
                        //Escribimos el next del ultimo EBR
                        fseek(Archivo,ftell(Archivo) - sizeof (ebr),SEEK_SET);
                        fwrite(&EBR, sizeof(ebr),1 ,Archivo);
                        //Escribimos el nuevo EBR
                        fseek(Archivo,EBR.part_start + EBR.part_size, SEEK_SET);
                        EBR.part_status = 0;
                        EBR.part_fit = auxFit;
                        EBR.part_start = ftell(Archivo);
                        EBR.part_size = size_bytes;
                        EBR.part_next = -1;
                        strcpy(EBR.part_name,name.c_str());
                        fwrite(&EBR,sizeof(ebr),1,Archivo);
                        if(archivo == "principal") cout << "\033[32mPARTICION LOGICA CREADA CON EXITO.\033[0m"<< endl;
                    }else{
                        cout << "\033[31mERROR: la particion logica a crear excede el" << endl;
                        cout << "espacio disponible de la particion extendida.\033[0m" << endl;
                    }
                }
            }else{
                cout << "\033[31mERROR: se necesita una particion extendida donde guardar la logica.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: ya existe una particion con ese nombre.\033[0m" << endl;
        }
    fclose(Archivo);
    }else{
        cout << "\033[31mERROR: no existe el disco.\033[0m" << endl;
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

void comando_fdisk::metodoAdd(string path, string name, int add, string unit, string archivo){
    string auxPath = path;
    string auxNombre = name;
    int size_Bytes = 0;
    QString tipo = "";

    if(add > 0){
        tipo = "add";
    }

    if(tipo != "add"){
        add = add*(-1);
    }

    if(unit == "M" || unit == "m"){
        size_Bytes = add * 1024 * 1024;
    }else if(unit == "K" || unit == "k"){
        size_Bytes = add * 1024;
    }else{
        size_Bytes = add;
    }

    FILE *Archivo;
    if((Archivo = fopen(auxPath.c_str(), "rb+"))){
        //Verificar que la particion no este montada
        //bool mount = lista->buscarNodo(direccion,nombre);
        //if(!mount){
            mbr MBR;
            fseek(Archivo,0,SEEK_SET);
            fread(&MBR,sizeof(mbr),1,Archivo);
            int index = -1;
            int index_Extendida = -1;
            bool flagExtendida = false;
            //Buscamos la particion primaria/extendida
            for(int i = 0; i < 4; i++){
                if((strcmp(MBR.mbr_partitions[i].part_name, auxNombre.c_str())) == 0){
                    index = i;
                    if(MBR.mbr_partitions[i].part_type == 'E')
                        flagExtendida = true;
                    break;
                }else if(MBR.mbr_partitions[i].part_type == 'E'){
                    index_Extendida = i;
                }
            }
            if(index != -1){//Si se encontro en las principales
                if(!flagExtendida){//Primaria
                    if(tipo == "add"){//Agregar
                        //Verificar que exista espacio libre a la derecha
                        if(index!=3){
                            int p1 = MBR.mbr_partitions[index].part_start + MBR.mbr_partitions[index].part_size;
                            int p2 = MBR.mbr_partitions[index+1].part_start;
                            if((p2 - p1) != 0){//Hay fragmentacion
                                int fragmentacion = p2-p1;
                                if(fragmentacion >= size_Bytes){
                                    MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                    fseek(Archivo,0,SEEK_SET);
                                    fwrite(&MBR,sizeof(mbr),1,Archivo);
                                    if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                }else{
                                    cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                }
                            }else{//Espacio no usado
                                if(MBR.mbr_partitions[index + 1].part_status == '1'){
                                    if(MBR.mbr_partitions[index + 1].part_size >= size_Bytes){
                                        MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                        MBR.mbr_partitions[index + 1].part_size = (MBR.mbr_partitions[index + 1].part_size - size_Bytes);
                                        MBR.mbr_partitions[index + 1].part_start = MBR.mbr_partitions[index + 1].part_start + size_Bytes;
                                        fseek(Archivo,0,SEEK_SET);
                                        fwrite(&MBR,sizeof(mbr),1,Archivo);
                                        if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                    }else{
                                        cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                    }
                                }
                            }
                        }else{
                            int p = MBR.mbr_partitions[index].part_start + MBR.mbr_partitions[index].part_size;
                            int total = MBR.mbr_tamano + (int)sizeof(mbr);
                            if((total-p) != 0){
                                int fragmentacion = total - p;
                                if(fragmentacion >= size_Bytes){
                                    MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                    fseek(Archivo,0,SEEK_SET);
                                    fwrite(&MBR,sizeof(mbr),1,Archivo);
                                    if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                }else{
                                    cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                }
                            }else{
                                cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay espacio disponible a su derecha.\033[0m" << endl;
                            }
                        }
                    }else{//Quitar espacio
                        //Que no borre la particion
                        if(size_Bytes >= MBR.mbr_partitions[index].part_size){
                            cout << "\033[31mERROR: no es posible quitarle esta cantidad de espacio a la particion porque la borraria.\033[0m" << endl;
                        }else{
                            MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size - size_Bytes;
                            fseek(Archivo,0,SEEK_SET);
                            fwrite(&MBR,sizeof(mbr),1,Archivo);
                            if(archivo == "principal") cout << "\033[96mSe quito espacio a la particion de manera exitosa.\033[0m" << endl;
                        }
                    }
                }else{//Extendida
                    if(tipo == "add"){//Agregar
                        //Verificar que exista espacio libre a la derecha
                        if(index!=3){
                            int p1 = MBR.mbr_partitions[index].part_start + MBR.mbr_partitions[index].part_size;
                            int p2 = MBR.mbr_partitions[index+1].part_start;
                            if((p2 - p1) != 0){//Hay fragmentacion
                                int fragmentacion = p2-p1;
                                if(fragmentacion >= size_Bytes){
                                    MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                    fseek(Archivo,0,SEEK_SET);
                                    fwrite(&MBR,sizeof(mbr),1,Archivo);
                                    if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                }else{
                                    cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                }
                            }else{
                                if(MBR.mbr_partitions[index + 1].part_status == '1'){
                                    if(MBR.mbr_partitions[index + 1].part_size >= size_Bytes){
                                        MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                        MBR.mbr_partitions[index + 1].part_size = (MBR.mbr_partitions[index + 1].part_size - size_Bytes);
                                        MBR.mbr_partitions[index + 1].part_start = MBR.mbr_partitions[index + 1].part_start + size_Bytes;
                                        fseek(Archivo,0,SEEK_SET);
                                        fwrite(&MBR,sizeof(mbr),1,Archivo);
                                        if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                    }else{
                                        cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                    }
                                }
                            }
                        }else{
                            int p = MBR.mbr_partitions[index].part_start + MBR.mbr_partitions[index].part_size;
                            int total = MBR.mbr_tamano + (int)sizeof(mbr);
                            if((total-p) != 0){//Hay fragmentacion
                                int fragmentacion = total - p;
                                if(fragmentacion >= size_Bytes){
                                    MBR.mbr_partitions[index].part_size = MBR.mbr_partitions[index].part_size + size_Bytes;
                                    fseek(Archivo,0,SEEK_SET);
                                    fwrite(&MBR,sizeof(mbr),1,Archivo);
                                    if(archivo == "principal") cout << "\033[96mSe agrego espacio a la particion de manera exitosa.\033[0m" << endl;
                                }else{
                                    cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha.\033[0m" << endl;
                                }
                            }else{
                                cout << "\033[31mERROR: no es posible agregar espacio a la particion porque no hay espacio disponible a su derecha.\033[0m" << endl;
                            }
                        }
                    }else{//Quitar espacio
                        //Que no borre la particion
                        if(size_Bytes >= MBR.mbr_partitions[index_Extendida].part_size){
                            cout << "\033[31mERROR: no es posible quitarle esta cantidad de espacio a la particion porque la borraria.\033[0m" << endl;
                        }else{
                            ebr EBR;
                            fseek(Archivo, MBR.mbr_partitions[index_Extendida].part_start,SEEK_SET);
                            fread(&EBR,sizeof(ebr),1,Archivo);
                            while((EBR.part_next != -1) && (ftell(Archivo) < (MBR.mbr_partitions[index_Extendida].part_size + MBR.mbr_partitions[index_Extendida].part_start))){
                                fseek(Archivo,EBR.part_next,SEEK_SET);
                                fread(&EBR,sizeof(ebr),1,Archivo);
                            }
                            int ultimaLogica = EBR.part_start + EBR.part_size;
                            int aux = (MBR.mbr_partitions[index_Extendida].part_start + MBR.mbr_partitions[index_Extendida].part_size) - size_Bytes;
                            if(aux > ultimaLogica){//No toca ninguna logica
                                MBR.mbr_partitions[index_Extendida].part_size = MBR.mbr_partitions[index_Extendida].part_size - size_Bytes;
                                fseek(Archivo,0,SEEK_SET);
                                fwrite(&MBR,sizeof(mbr),1,Archivo);
                                if(archivo == "principal") cout << "\033[96mSe quito espacio a la particion de manera exitosa.\033[0m" << endl;
                            }else{
                                cout << "\033[31mERROR: si quita ese espacio danaria una logica.\033[0m" << endl;
                            }
                        }
                    }
                }
            }else{//Posiblemente logica
                if(index_Extendida != -1){
                    int logica = buscarParticion_L(path, name);
                    if(logica != -1){
                        if(tipo == "add"){
                            //Verificar que exista espacio libre a su derecha
                            ebr EBR;
                            fseek(Archivo,logica,SEEK_SET);
                            fread(&EBR,sizeof(ebr),1,Archivo);

                        }else{//Quitar
                            //Verificar que no la elimine
                            ebr EBR;
                            fseek(Archivo,logica,SEEK_SET);
                            fread(&EBR,sizeof(ebr),1,Archivo);
                            if(size_Bytes >= EBR.part_size){
                                cout << "\033[31mERROR: si quita ese espacio eliminaria la logica.\033[0m" << endl;
                            }else{
                                EBR.part_size = EBR.part_size - size_Bytes;
                                fseek(Archivo,logica,SEEK_SET);
                                fwrite(&EBR,sizeof(ebr),1,Archivo);
                                if(archivo == "principal") cout << "\033[96mSe quito espacio a la particion de manera exitosa.\033[0m" << endl;
                            }
                        }
                    }else{
                        cout << "\033[31mERROR: no se encuentra la particion.\033[0m" << endl;
                    }
                }else{
                    cout << "\033[31mERROR: no se encuentra la particion a redimensionar.\033[0m" << endl;
                }
            }
        //}else{
             //cout << "\033[31mERROR: desmonte primero la particion para poder redimensionar.\033[0m" << endl;
        //}
    fclose(Archivo);
    }else{
        cout << "\033[31mERROR: el disco donde se desea agregar/quitar unidades no existe.\033[0m" << endl;
    }
}

int comando_fdisk::buscarParticion_L(string path, string name){
    string auxPath = path;
    string auxName = name;
    FILE *Archivo;
    if((Archivo = fopen(auxPath.c_str(),"rb+"))){
        int extendida = -1;
        mbr MBR;
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR,sizeof(mbr),1,Archivo);
        for(int i = 0; i < 4; i++){
            if(MBR.mbr_partitions[i].part_type == 'E'){
                extendida = i;
                break;
            }
        }
        if(extendida != -1){
            ebr EBR;
            fseek(Archivo, MBR.mbr_partitions[extendida].part_start,SEEK_SET);
            while(fread(&EBR,sizeof(ebr),1,Archivo)!=0 && (ftell(Archivo) < MBR.mbr_partitions[extendida].part_start + MBR.mbr_partitions[extendida].part_size)){
                if(strcmp(EBR.part_name, auxName.c_str()) == 0){
                    return (ftell(Archivo) - sizeof(ebr));
                }
            }
        }
        fclose(Archivo);
    }
    return -1;
}
