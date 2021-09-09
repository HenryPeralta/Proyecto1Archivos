#include "comando_mount.h"
#include "estructuras.h"
#include "string"
#include <QString>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

comando_mount::comando_mount()
{

}

void comando_mount::creandoMount(comando_mount *disco){
    printf("La ruta es %s \n",disco->path.c_str());
    printf("La Palabra es: %s \n",disco->ppath.c_str());
    printf("El name es %s \n",disco->name.c_str());
    printf("El name es: %s \n",disco->pname.c_str());

    QString cambio_name = disco->name.c_str();
    cambio_name.replace(QString("\""), QString(""));
    string nuevo_name = cambio_name.toStdString();

    QString cambio_ruta = path.c_str();
    cambio_ruta.replace(QString("\""), QString(""));
    string rutax = cambio_ruta.toStdString();

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
        ejecutarMount(cambio_ruta, cambio_name);
    }
}

void comando_mount::ejecutarMount(QString path, QString name){
    int indexP = buscarParticion_P_E(path,name);
    if(indexP != -1){
        FILE *Archivo;
        if((Archivo = fopen(path.toStdString().c_str(),"rb+"))){
            mbr MBR;
            fseek(Archivo, 0, SEEK_SET);
            fread(&MBR, sizeof(mbr),1,Archivo);
            MBR.mbr_partitions[indexP].part_status = '2';
            //cout<<masterboot.mbr_partition[indexP].part_type<<endl;
            char temp_type = MBR.mbr_partitions[indexP].part_type;
            particion temp_part = MBR.mbr_partitions[indexP];
            fseek(Archivo,0,SEEK_SET);
            fwrite(&MBR,sizeof(mbr),1,Archivo);

            //std::cout<<"p1: "<<temp_type<<"\n";
            //std::cout<<"p2: "<<temp_part.part_name<<"\n";
            //std::cout<<"p3: "<<letra<<"\n";

            fclose(Archivo);
            int letra = lista->buscarLetra(path,name);
            if(letra == -1){
                cout << "\033[31mERROR: la particion ya esta montada.\033[0m" << endl;
            }else{
                int num = lista->buscarNumero(path, name);
                char auxLetra = static_cast<char>(letra);
                string id = "08";
                id += to_string(num) +auxLetra;
                tipo_particiones *n = new tipo_particiones(path,name,auxLetra,num, temp_type);
                n->particiones=temp_part;
                lista->insertarNodo(n);
                cout << "\033[96mParticion montada con exito.\033[0m" << endl;
                //lista->mostrarLista();
            }
        }else{
            cout << "\033[31mERROR: no se encuentra el disco.\033[0m" << endl;
        }
    }else{//Posiblemente logica
        int indexP = buscarParticion_L(path,name);
        if(indexP != -1){
            FILE *Archivo;
            if((Archivo = fopen(path.toStdString().c_str(), "rb+"))){
                ebr EBR;
                fseek(Archivo, indexP, SEEK_SET);
                fread(&EBR, sizeof(ebr),1,Archivo);
                EBR.part_status = '2';
                ebr temp_part = EBR;
                fseek(Archivo,indexP,SEEK_SET);
                fwrite(&EBR,sizeof(ebr),1, Archivo);
                fclose(Archivo);

                int letra = lista->buscarLetra(path,name);
                if(letra == -1){
                    cout << "\033[31mERROR: La particion ya esta montada.\033[0m" << endl;
                }else{
                    int num = lista->buscarNumero(path,name);
                    char auxLetra = static_cast<char>(letra);
                    string id = "08";
                    id += to_string(num) +auxLetra;
                    tipo_particiones *n = new tipo_particiones(path, name, auxLetra, num,'l');
                    n->particion_logica=temp_part;
                    lista->insertarNodo(n);
                    cout << "\033[96mParticion montada con exito.\033[0m" << endl;
                    //lista->mostrarLista();
                }
            }else{
                cout << "\033[31mERROR: no se encuentra el disco.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: no se encuentra la particion a montar.\033[0m" << endl;
        }
    }
}

int comando_mount::buscarParticion_P_E(QString path, QString name){
    string auxPath = path.toStdString();
    string auxName = name.toStdString();
    FILE *Archivo;
    if((Archivo = fopen(auxPath.c_str(),"rb+"))){
        mbr MBR;
        fseek(Archivo,0,SEEK_SET);
        fread(&MBR,sizeof(mbr),1,Archivo);
        for(int i = 0; i < 4; i++){
            if(MBR.mbr_partitions[i].part_status != '1'){
                if(strcmp(MBR.mbr_partitions[i].part_name,auxName.c_str()) == 0){
                    return i;
                }
            }
        }

    }
    return -1;
}

int comando_mount::buscarParticion_L(QString path, QString name){
    string auxPath = path.toStdString();
    string auxName = name.toStdString();
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
