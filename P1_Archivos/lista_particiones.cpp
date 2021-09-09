#include "lista_particiones.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

lista_particiones::lista_particiones()
{
    primero = nullptr;
}

void lista_particiones::insertarNodo(tipo_particiones *nuevo){
    tipo_particiones *aux = primero;
    if(primero == nullptr){
        primero = nuevo;
    }else{
        while(aux->siguiente!=nullptr){
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }

}

int lista_particiones::eliminarNodo(QString ID){
    tipo_particiones *aux = primero;
    QString tempID = "08";
    tempID += QString::number(aux->num)+aux->letra;
    if(ID == tempID){
        primero = aux->siguiente;
        free(aux);
        return 1;
    }else{
        tipo_particiones *aux2 = nullptr;
        while(aux!=nullptr){
            tempID = "08";
            tempID += QString::number(aux->num) + aux->letra;
            if(ID == tempID){
                aux2->siguiente = aux->siguiente;
                return 1;
            }
            aux2 = aux;
            aux = aux->siguiente;
        }
    }
    return 0;
}

int lista_particiones::buscarLetra(QString path, QString nombre){
    tipo_particiones *aux = primero;
    int retorno = 'a';
    cout << "HOLA" << endl;
    while(aux!=NULL){
        cout << "HOLA2" << endl;
        cout << path.toStdString() << endl;
        cout << nombre.toStdString() << endl;
        cout << "HOLIS" << endl;
        cout << aux->path.toStdString().c_str() << endl;
        cout << aux->nombre.toStdString().c_str() << endl;
        if((path == aux->path) && (nombre == aux->nombre)){
            cout << "HOLA3" << endl;
            return -1;
        }else{
            cout << "HOLA4" << endl;
            if(path == aux->path){
                cout << "HOLA5" << endl;
                return aux->letra;
            }else if(retorno <= aux->letra){
                cout << "HOLA6" << endl;
                retorno++;
            }
        }
        aux = aux->siguiente;
    }
    return retorno;
}

int lista_particiones::buscarNumero(QString path, QString nombre){
    int retorno = 1;
    tipo_particiones *aux = primero;
    while(aux!=nullptr){
        if((path == aux->path) && (retorno == aux->num)){
            retorno++;
        }
        aux = aux->siguiente;
    }
    return retorno;
}

QString lista_particiones::getDireccion(QString id){
    tipo_particiones *aux = primero;
    while(aux!=nullptr){
        QString tempID = "08";
        tempID +=  QString::number(aux->num) + aux->letra;
        if(id == tempID){
            return aux->path;
        }
        aux = aux->siguiente;
    }
    return "null";
}

bool lista_particiones::buscarNodo(QString path, QString nombre){
    tipo_particiones *aux = primero;
    while(aux!=nullptr){
        if((aux->path == path) && (aux->nombre == nombre)){
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

/* Metodo para desplegar las particiones montadas
*/
void lista_particiones::mostrarLista(){
    cout << "*********************************" << endl;
    cout << "*       LISTA DE PARTICIONES    *" << endl;
    cout << "*********************************" << endl;
    cout << "*      NOMBRE    |    ID        *" << endl;
    cout << "*********************************" << endl;
    tipo_particiones *aux = primero;
    while(aux!=nullptr){
        cout << "     "<< aux->nombre.toStdString() << "          " << "08"<<aux->num<<aux->letra << endl;
        cout << "*********************************" << endl;
        aux = aux->siguiente;
    }
}

tipo_particiones lista_particiones::getParticionMontada(QString pid){
    tipo_particiones *aux = primero;
    while(aux!=nullptr){
        QString ntemp= "08";
        ntemp.append(QString::number(aux->num));
        ntemp.append(aux->letra);
        if(ntemp==pid){
            return *aux;
        }
        aux = aux->siguiente;
    }
    tipo_particiones *temp = new tipo_particiones();
    return *temp;
}

bool lista_particiones::estavacio(){
    if(primero==nullptr){
        return true;
    }
    return false;
}
