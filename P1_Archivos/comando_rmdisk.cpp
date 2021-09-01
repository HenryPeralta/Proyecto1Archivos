#include "comando_rmdisk.h"
#include <iostream>
#include "iostream"
#include <QString>
#include "string"
using namespace std;

comando_rmdisk::comando_rmdisk()
{

}

void comando_rmdisk::borrarDisco(comando_rmdisk *disco){

          printf("\n\n---------DATOS----------\n\n");
          printf("La ruta del disco a borrar es %s \n",disco->path.c_str());

          QString cambio_ruta = disco->path.c_str();
          cambio_ruta.replace(QString("\""), QString(""));
          string rutax = cambio_ruta.toStdString();

          QString ruta = rutax.c_str();

          FILE *archivo;
          if((archivo=fopen(ruta.toStdString().c_str(),"r"))){
              string opcion = "";
              cout << "¿Esta seguro que desea eliminar el disco? S/N : ";
              getline(cin, opcion);
              if(opcion.compare("S") == 0 || opcion.compare("s") == 0){
                  string comando = "rm \""+ruta.toStdString()+"\"";
                  system(comando.c_str());
                  cout <<"Disco eliminado con exito" << endl;
              }else if(opcion.compare("N") == 0 || opcion.compare("n") == 0){
                  cout << "Operacion Cancelada" << endl;
              }else{
                  cout << "ERROR: No existe la opcion seleccionada" << endl;
              }
              fclose(archivo);
          }else{
              std::cout << "No Existe el disco que desea eliminar" << std:: endl;
          }
}
