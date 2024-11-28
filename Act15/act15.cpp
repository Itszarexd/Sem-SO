#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <queue>
#include <algorithm>
#include <cstdlib> //Para utilizar función rand()
using namespace std; 

//rand() -> nos da un número random entre 0 y MAX_INT  

//struct proceso -> para almacenar los datos procesos
/*
Proceso debe de tener:
ID: Generado aleatoriamente (puede ser consecutivo)
TME: Tiempo maximo de espera generado aleatoriamente entre 5 y 20
Operacion: Generada aleatoriamente y que sea valida
TT: Tiempo Transcurrido
TL
*/
int id_de_procesos = 1;
vector<char> opcs = {'+', '-', '*', '/', '%'};

struct Proceso{
    int id, resultado;
    char opc;
    int a, b;
    bool error;
    string estado;
    //Tiempos en los procesos
    int TME; // Tiempo Maximo de Estimado
    int TT; // Tiempo Transcurrido
    int TL; // Tiempo de Llegada
    int TF; // Tiempo de Finalización
    int Tr; // Tiempo de respuesta
    int TE; // Tiempo de espera 
    int TA; // Tiempo de atención
    int tam; // Tamaño de memoria

    //Constructor 
    Proceso(): TME(min(10, rand() % 20 + 1)), id(id_de_procesos++), TT(0), error(false),TL(-1), TF(-1), Tr(-1), TE(-1), TA(-1){
      estado = "nuevo";
      opc = opcs[rand() % 5]; //Selecciona de manera aleatoria una operacion
      tam = max(6, rand() % 27); // Generamos el tamaño de manera aleatoria entre 6 y 26
      a = rand() % 10000;   // Selecciona de manera aleatoria el primer operando
      b = max(1, rand() % 10000); //Se selecciona de manera aleatoria el segundo operando
      if(opc == '+'){ resultado = a + b;}
      else if(opc == '-'){resultado = a - b;}
      else if(opc == '*'){resultado = a * b;}
      else if(opc == '/'){resultado = a / b;}
      else if(opc == '%'){resultado = a % b;}
    }

    //Sobrecarga de operador
    //Significa que le dire que debe de hacer en caso de que haya una operacion
    //De tipo "Proceso = Proceso"
    void operator = (const Proceso aux){
        TME = aux.TME;
        id = aux.id;
        resultado = aux.resultado;
        TT = aux.TT;
        estado = aux.estado;
        opc = aux.opc;
        a = aux.a;
        b = aux.b;
        error = aux.error;
        TL = aux.TL;
        TF = aux.TF;
        Tr = aux.Tr;
        TE = aux.TE;
        TA = aux.TA;
        tam = aux.tam;
    }

    bool operator < (const Proceso aux){
        return id < aux.id;
    }
};

/*
El usuario nos dirá cuantos procesos quiere y cual será el quantum.
*/

vector<int> frame(42, -1); //Frames en memoria
vector<int> ocupado(42, 0); // Espacio ocupado del frame
int sinusar = 41; //Cuantos Frames de memoria tenemos libres

void printFrames(){
    frame[41] = 10000;
    cout << "Frames de memoria\n";
    cout << "|Frame|Ocupado|Proceso|   |Frame|Ocupado|Proceso|   |Frame|Ocupado|Proceso|   |Frame|Ocupado|Proceso|\n";
    int fr = 0;
    for(int i = 0; i<12; i++){
        for(int j = 0; j<4 and fr < 46; j++, fr++){
            string frm = to_string(fr);
            if(fr >= 41){
                cout << "|" << frm; for(int k = 0; k < 5 - frm.size(); k++) cout << " ";
                cout << "|5/5    |SO     |   ";  
                continue;
            }
            string ocup = to_string(ocupado[fr]) + "/5";
            string Proce = (frame[fr] == -1? "libre":to_string(frame[fr]));
            cout << "|" << frm; for(int k = 0; k < 5 - frm.size(); k++) cout << " ";
            cout << "|" << ocup; for(int k = 0; k < 7 - ocup.size(); k++) cout << " ";
            cout << "|" << Proce; for(int k = 0; k < 7 - Proce.size(); k++) cout << " ";
            cout << "|   ";
        }
        cout << "\n";
    }
}

int main(){
    bool pausa = false;
    queue<Proceso> listos, nuevos, bloqueados, suspendidos; 
    queue<int> liberar;
    vector<Proceso> terminados;
    cout << "Dime el quantum de Round Robin:" << endl;
    int quantum; 
    cin >> quantum;
    cout << "Numero de procesos inicial: " << endl;
    int n; 
    cin >> n;
    for(int i =0; i<n; i++){
        nuevos.push(Proceso());
    }
    cout << "numero de procesos nuevos: "; cout << nuevos.size() << endl;
    int cnt = 0, enMemoria = 0, tmEjecucion = 0;
    bool seleccionado = false;
    Proceso actual;
    system("cls");
    while(!nuevos.empty() or enMemoria > 0 or !suspendidos.empty()){
        while(!nuevos.empty() and sinusar*5 >= (nuevos.front()).tam){
            Proceso tmp = nuevos.front();
            nuevos.pop();
            
            //marco hora de llegada al sistema
            tmp.TL = cnt;
            //marcar en estado de listo
            tmp.estado = "listo";
            listos.push(tmp);
            enMemoria++;
            int tmpTam = tmp.tam;
            for(int i = 0; i<42; i++){
                if(frame[i] == -1){
                    frame[i] = tmp.id;
                    ocupado[i] = min(5, tmpTam);
                    tmpTam -= min(5, tmpTam);
                    sinusar--;                    
                }
                if(tmpTam == 0) break;
            }
        }
        if(!seleccionado and !listos.empty()){
            actual = listos.front();
            listos.pop();
            actual.estado = "ejecucion";
            seleccionado = true;
            tmEjecucion = 0;
            if(actual.TA == -1){ // Es la primera vez que entra al procesador
                actual.TA = cnt;
            }
        }
        // Detectar si una tecla fue presionada
        if(_kbhit()){
            char tecla = _getch();
            if(tecla == 'e'){
                if(!seleccionado) continue;
                //Si es la primera vez que sale del procesador,  
                //calcular tiempo de respuesta para el proceso en ejecución
                if(actual.Tr == -1){
                    actual.Tr = actual.TA - actual.TL;
                }
                actual.estado = "bloqueado";
                bloqueados.push(actual);
                liberar.push(cnt + 7);
                seleccionado = false;
            }
            if(tecla == 'w'){
                if(!seleccionado) continue;
                if(actual.Tr == -1){
                    actual.Tr = actual.TA - actual.TL;
                }
                actual.TF = cnt;
                actual.TE = (actual.TF - actual.TL) - actual.TT;
                actual.error = true;
                actual.estado = "terminado";
                terminados.push_back(actual);
                seleccionado = false;
                enMemoria--;
                for(int i = 0; i<42; i++){
                    if(frame[i] == actual.id){
                        frame[i] = -1;
                        ocupado[i] = 0;
                        sinusar++;
                    }
                }
            }
            if(tecla == 'p'){
                pausa = true;
            }
            if(tecla == 'c'){
                pausa = false;
            }
            if(tecla == 'n'){
                //Agregar un proceso nuevo
                nuevos.push(Proceso());
            }
            if(tecla == 'b'){
                //Tabla de procesos
                system("cls");
                vector<Proceso> tabla;
                queue<Proceso> tablaq = nuevos;
                while(!tablaq.empty()){
                    tabla.push_back(tablaq.front());
                    tablaq.pop();
                }
                tablaq = listos;
                while(!tablaq.empty()){
                    tabla.push_back(tablaq.front());
                    tablaq.pop();
                }
                tablaq = bloqueados;
                while(!tablaq.empty()){
                    tabla.push_back(tablaq.front());
                    tablaq.pop();
                }
                for(int i = 0; i<int(terminados.size()); i++) tabla.push_back(terminados[i]);
                sort(tabla.begin(), tabla.end());
                Proceso tmp;
                cout << "Tabla de procesos" << endl;
                cout << "|ID |Estado    |Operacion           |Resultado           |TME|TL |TF |TR |Tr |TE |TS |"<<endl;
                for(int i = 0; i < tabla.size(); i++){
                    Proceso tmp = tabla[i];
                    string retorno = (tmp.TF == -1 or tmp.TL == -1? "NA":to_string(tmp.TF - tmp.TL));
                    string opc2 = to_string(tmp.a) + tmp.opc + to_string(tmp.b);
                    string res = (tmp.TF == -1? "NA":(tmp.error? "ERROR": to_string(tmp.resultado)));
                    string TmL = (tmp.TL == -1? "NA":to_string(tmp.TL));
                    string TmF = (tmp.TF == -1? "NA":to_string(tmp.TF));
                    string TmRes = (tmp.Tr == -1? "NA": to_string(tmp.Tr));
                    string TeE = (tmp.TL == -1? "NA": to_string(cnt - tmp.TL - tmp.TT));
                    if(tmp.TME == tmp.TT){
                        TeE = to_string(tmp.TE);
                    }
                    if(tmp.TT == 0){
                        TeE = "NA";
                    }
                    string TS = (tmp.TT == -1? "NA": to_string(tmp.TT));
                    string TmmE = to_string(tmp.TME);
                    cout << '|' << tmp.id; for(int i = 0; i < 3 - (to_string(tmp.id)).size(); i++) cout << " ";
                    cout << '|' << tmp.estado; for(int i = 0; i < 10 - int(tmp.estado.size()); i++) cout << " ";
                    cout << '|' << opc2; for(int i = 0; i < 20 - (opc2.size()); i++) cout << " ";
                    cout << '|' << res; for(int i = 0; i < 20 - ((res).size()); i++) cout << " "; 
                    cout << '|' << TmmE; for(int i = 0; i < 3 -(TmmE.size()); i++) cout << " ";
                    cout << '|' << TmL; for(int i = 0; i < 3 -(TmL.size()); i++) cout << " ";
                    cout << '|' << TmF; for(int i = 0; i < 3 - (TmF.size()); i++) cout << " ";
                    cout << '|' << retorno; for(int i = 0; i < 3 - (retorno).size(); i++) cout << " ";
                    cout << "|" << TmRes; for(int i = 0; i < 3 - (TmRes.size()); i++){ cout << " ";}
                    cout << '|' << TeE; for(int i = 0; i < 3 - TeE.size(); i++) cout << " "; 
                    cout << '|' << TS; for(int i = 0; i < 3 - TS.size(); i++) cout << " "; cout << '|'; 
                    cout << endl;
                }
                pausa = true;
            }
            if(tecla == 's'){
                if(!bloqueados.empty()){
                    bloqueados.front().estado = "suspendido";
                    suspendidos.push(bloqueados.front());
                    for(int i = 0; i<41; i++){
                        if(frame[i] == (bloqueados.front()).id){
                            frame[i] = -1;
                            ocupado[i] = 0;
                            sinusar++;
                        }
                    }
                    bloqueados.pop();
                    liberar.pop();
                }
            }  
            if(tecla == 'r'){
                if(!suspendidos.empty() and sinusar*5 >= (suspendidos.front()).tam){
                    int tmpttt = suspendidos.front().tam;
                    for(int i = 0; i < 41; i++){
                        if(frame[i] == -1){
                            frame[i] = suspendidos.front().id;
                            ocupado[i] = min(5, tmpttt);
                            tmpttt -= min(5, tmpttt);
                        }
                    }
                    suspendidos.front().estado = "listo";
                    listos.push(suspendidos.front());
                    suspendidos.pop();
                }
            }
        }
        if(pausa){
            continue;
        }
        else system("cls");

        // Imprimir en pantalla
        printFrames();
        cout << "========================================================================"<<endl;
        cout << "Numero de procesos en estado Nuevo: " << nuevos.size() << endl;
        cout << "========================================================================"<<endl;
        cout << "Cola de listos" << endl;
        cout << "|ID |TME|TT |tam|" << endl;
        queue<Proceso> colaTmp = listos;
        while(!colaTmp.empty()){
            Proceso tmp = colaTmp.front();
            colaTmp.pop();
            cout << '|' << tmp.id; for(int i = 0; i<3 - (to_string(tmp.id)).size(); i++) cout << ' ';
            cout << '|' << tmp.TME; for(int i = 0; i<3 - (to_string(tmp.TME)).size(); i++) cout << ' ';
            cout << '|' << tmp.TT; for(int i = 0; i<3 - (to_string(tmp.TT)).size(); i++) cout << ' '; 
            cout << '|' << tmp.tam; for(int i = 0; i<3 - (to_string(tmp.tam)).size(); i++) cout << ' ';
            cout <<'|'<< endl;
        }
        cout << "========================================================================"<<endl;
        string operacion = to_string(actual.a) + actual.opc + to_string(actual.b);
        int tr = actual.TME - actual.TT;
        string tamaniodelproceso = to_string(actual.tam);
        cout << "Proceso en Ejecucion"<<endl;
        cout << "|ID |Operacion           |TME|TT |TRes|tamanio|"<<endl;
        if(seleccionado){
            cout << '|' << actual.id; for(int i = 0; i<3 - (to_string(actual.id)).size(); i++) cout << ' ';
            cout << '|' << operacion ; for(int i = 0; i<20 - (operacion.size()); i++) cout << " ";
            cout << '|' << actual.TME; for(int i = 0; i<3 - (to_string(actual.TME)).size(); i++) cout << " ";
            cout << '|' << actual.TT; for(int i = 0; i < 3 - (to_string(actual.TT)).size(); i++) cout << " ";
            cout << '|' << tr; for(int i = 0; i < 3 - (to_string(tr)).size(); i++) cout << " "; 
            cout << '|' << tamaniodelproceso; for(int i = 0; i < 7-tamaniodelproceso.size(); i++) cout << " ";
            cout << '|' <<endl;
        }
        cout << "========================================================================"<<endl;
        colaTmp = bloqueados;
        queue<int> lb = liberar;
        cout << "Procesos bloqueados" << endl;
        cout << "|ID |TT en bloqueo|"<<endl;
        while(!colaTmp.empty()){
            Proceso tmp = colaTmp.front();
            int entro = lb.front() - 7;
            lb.pop();
            colaTmp.pop();
            cout << '|' << tmp.id; for(int i = 0; i<3 - (to_string(tmp.id)).size(); i++) cout << ' ';
            cout << '|' << cnt - entro; for(int i = 0; i<13 - (to_string(cnt-entro)).size(); i++) cout << ' '; cout << '|' << endl;
        }
        cout << "========================================================================"<<endl;
        cout << "Numero de procesos suspendidos: " << suspendidos.size() << endl;
        cout << "|ID |tamanio|"<<endl;
        colaTmp = suspendidos;
        while(!colaTmp.empty()){
            Proceso tmp = colaTmp.front();
            colaTmp.pop();
            cout << '|' << tmp.id; for(int i = 0; i<3 - (to_string(tmp.id)).size(); i++) cout << ' ';
            cout << '|' << tmp.tam; for(int i = 0; i<7 - (to_string(tmp.tam)).size(); i++) cout << ' '; cout << '|' << endl;
        }
        cout << "========================================================================"<<endl;
        cout << "Procesos Terminados" << endl;
        cout << "|ID |Operacion           |Resultado           |"<<endl;
        for(int i = 0; i < terminados.size(); i++){
            Proceso tmp = terminados[i];
            string opc2 = to_string(tmp.a) + tmp.opc + to_string(tmp.b);
            string res = (tmp.error? "ERROR": to_string(tmp.resultado));
            cout << '|' << tmp.id; for(int i = 0; i < 3 - (to_string(tmp.id)).size(); i++) cout << " "; 
            cout << '|' << opc2; for(int i = 0; i<20 - (opc2.size()); i++) cout << " "; 
            cout << '|' << res; for(int i = 0; i < 20 - ((res).size()); i++) cout << " "; cout << '|'<<endl;
        } 
        cout << "========================================================================"<<endl;
        cout << "Reloj: "<<cnt;

        Sleep(1000);
        if(!liberar.empty() and liberar.front() == cnt){
            Proceso lib = bloqueados.front();
            bloqueados.pop();
            lib.estado = "listo";
            listos.push(lib);
            liberar.pop();
        }
        cnt++;
        if(seleccionado){
            actual.TT++;
            tmEjecucion++;
            //Ver si termino de ejecutar
            if(actual.TT == actual.TME){ // Termino de ejecutarse el proceso
                if(actual.Tr == -1){ //Calculamos el tiempo de respuesta, si aún no se ha calculado
                    actual.Tr = actual.TA - actual.TL;
                }
                actual.TF = cnt;
                actual.TE = (actual.TF - actual.TL)- actual.TT;
                actual.estado = "terminado";
                seleccionado = false;
                enMemoria--;
                terminados.push_back(actual);
                for(int i = 0; i<42; i++){
                    if(frame[i] == actual.id){
                        frame[i] = -1;
                        ocupado[i] = 0;
                        sinusar++;
                    }
                }
            }
            else if(tmEjecucion == quantum){
                seleccionado = false;
                listos.push(actual);
                tmEjecucion = 0;
            }
        }
    }
    
    system("cls");
    cout << "Numero de procesos en estado Nuevo: " << nuevos.size() << endl;
        cout << "========================================================================"<<endl;
        cout << "Cola de listos" << endl;
        cout << "|ID |TME|TT |" << endl;
        cout << endl;
        cout << "========================================================================"<<endl;
        cout << "Proceso en Ejecucion"<<endl;
        cout << endl;
        cout << "========================================================================"<<endl;
        cout << "Procesos Terminados" << endl;
        cout << "|ID |Operacion           |Resultado           |TME|TL |TF |TR |Tr |TE |TS |"<<endl;
        for(int i = 0; i < terminados.size(); i++){
            Proceso tmp = terminados[i];
            int retorno = tmp.TF - tmp.TL;
            string opc2 = to_string(tmp.a) + tmp.opc + to_string(tmp.b);
            string res = (tmp.error? "ERROR": to_string(tmp.resultado));
            string TmmE = to_string(tmp.TME);
            cout << '|' << tmp.id; for(int i = 0; i < 3 - (to_string(tmp.id)).size(); i++) cout << " ";
            cout << '|' << opc2; for(int i = 0; i < 20 - (opc2.size()); i++) cout << " ";
            cout << '|' << res; for(int i = 0; i < 20 - ((res).size()); i++) cout << " "; 
            cout << '|' << TmmE; for(int i = 0; i < 3 -(TmmE.size()); i++) cout << " ";
            cout << '|' << tmp.TL; for(int i = 0; i < 3 -(to_string(tmp.TL).size()); i++) cout << " ";
            cout << '|' << tmp.TF; for(int i = 0; i < 3 - (to_string(tmp.TF)).size(); i++) cout << " ";
            cout << '|' << retorno; for(int i = 0; i < 3 - (to_string(retorno)).size(); i++) cout << " ";
            cout << '|' << tmp.Tr; for(int i = 0; i < 3 - (to_string(tmp.Tr)).size(); i++){ cout << " ";}
            cout << '|' << tmp.TE; for(int i = 0; i < 3 - (to_string(tmp.TE)).size(); i++) cout << " "; 
            cout << '|' << tmp.TT; for(int i = 0; i < 3 - (to_string(tmp.TT)).size(); i++) cout << " "; cout << '|' << endl;
        }
        cout << "========================================================================"<<endl;
        cout << "Reloj: "<<cnt;
        while(true){
            if(_kbhit()){
                char x = _getch();
                if(int(x) == 27){
                    break;
                }
            }
        }
    return 0;
}