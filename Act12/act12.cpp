#include <iostream>
#include <windows.h>    // time
#include <cstdlib>      // rand()
#include <conio.h>      // _kbhit()
#include <vector>
#include <string>
using namespace std; 

//varible para saber si el programa está activo
bool activo = true;
int elementos;

//variables del productor
int p_pos = 0;
bool p_estado = false;


//variables del cosumidor
int c_pos = 0;
bool c_estado = false;

//Contendor 
string contenedor(40, '_');
int libres = 20;


void print(){
    system("cls");
    cout << "Productor: " << (p_estado? "Trabajando": "Dormido") << "\n";
    cout << "Consumidor: " << (c_estado? "Trabajando": "Dormido") << "\n";
    if(p_estado){
        cout << "Posicion de Productor: " << p_pos+1 << "\n";
        cout << "Restantes: " << elementos << "\n";
    }
    else if(c_estado){
        cout << "Posicion de Consumidor: " << c_pos+1 << "\n";
        cout << "Restantes: " << elementos << "\n";
    }
    else{
        cout << "\n\n";
    } 
    cout << contenedor << "\n";
}

bool terminado(){
    if(_kbhit()){
        char tecla = _getch();
        if(int(tecla) == 27){
            activo = false;
            return true;
        }
    }
    return false;
}

void work_consumidor(){
    while(elementos){
        contenedor[2 * c_pos] = '_';
        c_pos = (c_pos + 1) % 20;
        elementos--;
        libres++;
        print();
        Sleep(1000);
        if(terminado()){
            return;
        }
    }
    c_estado = false;
}

void work_productor(){
    while(elementos){
        contenedor[2 * p_pos] = 'X';
        p_pos = (p_pos + 1) % 20;
        elementos--;
        libres--;
        print();
        Sleep(1000);
        if(terminado()) return;
    }
    p_estado = false;
}


int main(){
    for(int i = 1; i<40; i+=2) contenedor[i] = ' ';
    while(activo){
        print();
        Sleep(1000);
        int turno = rand() % 2;
        if(turno and libres > 0){
            p_estado = true;
            elementos = min(max(rand() % 6, 2), libres);
            work_productor();
        }
        else if(!turno and libres < 20){
            c_estado = true;
            elementos = min(max(rand()%6, 2), 20 - libres);
            work_consumidor();
        }
        terminado();
    }
    
    return 0;
}
