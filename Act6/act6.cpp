 #include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <queue>
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
    //Tiempos en los procesos
    int TME; // Tiempo Maximo de Estimado
    int TT; // Tiempo Transcurrido
    int TL; // Tiempo de Llegada
    int TF; // Tiempo de Finalización
    int Tr; // Tiempo de respuesta
    int TE; // Tiempo de espera 
    int TA; // Tiempo de atención

    //Constructor 
    Proceso(): TME(min(10, rand() % 20 + 1)), id(id_de_procesos++), TT(0), error(false),TL(-1), TF(-1), Tr(-1), TE(-1), TA(-1){
      opc = opcs[rand() % 5]; //Selecciona de manera aleatoria una operacion
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
        opc = aux.opc;
        a = aux.a;
        b = aux.b;
        error = aux.error;
        TL = aux.TL;
        TF = aux.TF;
        Tr = aux.Tr;
        TE = aux.TE;
        TA = aux.TA;
    }
};

/*
El usuario solo nos dirá cuántos procesos quiere crear.
*/

//Cola de procesos

int main(){
    bool pausa = false;
    queue<Proceso> listos, nuevos, bloqueados; 
    queue<int> liberar;
    vector<Proceso> terminados;
    cout << "Numero de procesos inicial: " << endl;
    int n; cin >> n;
    for(int i =0; i<n; i++){
        nuevos.push(Proceso());
    }
    cout << "numero de procesos nuevos: "; cout << nuevos.size() << endl;
    int cnt = 0, enMemoria = 0;
    bool seleccionado = false;
    Proceso actual;
    system("cls");
    while(!nuevos.empty() or enMemoria > 0){
        while(!nuevos.empty() and enMemoria < 5){
            Proceso tmp = nuevos.front();
            nuevos.pop();
            
            //marco hora de llegada al sistema
            tmp.TL = cnt;

            listos.push(tmp);
            enMemoria++;
        }
        if(!seleccionado and !listos.empty()){
            actual = listos.front();
            listos.pop();
            seleccionado = true;
            if(actual.TA == -1){ // Es la primera vez que entra al procesador
                actual.TA = cnt;
            }
        }
        // Detectar si una tecla fue presionada
        if(_kbhit()){
            char tecla = _getch();
            if(tecla == 'i'){
                if(!seleccionado) continue;
                //Si es la primera vez que sale del procesador,  
                //calcular tiempo de respuesta para el proceso en ejecución
                if(actual.Tr == -1){
                    actual.Tr = actual.TA - actual.TL;
                }

                bloqueados.push(actual);
                liberar.push(cnt + 7);
                seleccionado = false;
            }
            if(tecla == 'e'){
                if(!seleccionado) continue;
                if(actual.Tr == -1){
                    actual.Tr = actual.TA - actual.TL;
                }
                actual.TF = cnt;
                actual.TE = (actual.TF - actual.TL) - actual.TT;
                actual.error = true;
                terminados.push_back(actual);
                seleccionado = false;
                enMemoria--;
            }
            if(tecla == 'p'){
                pausa = true;
            }
            if(tecla == 'c'){
                pausa = false;
            }  
        }
        if(pausa){
            continue;
        }
        else system("cls");

        // Imprimir en pantalla
        cout << "Numero de procesos en estado Nuevo: " << nuevos.size() << endl;
        cout << "========================================================================"<<endl;
        cout << "Cola de listos" << endl;
        cout << "|ID |TME|TT |" << endl;
        queue<Proceso> colaTmp = listos;
        while(!colaTmp.empty()){
            Proceso tmp = colaTmp.front();
            colaTmp.pop();
            cout << '|' << tmp.id; for(int i = 0; i<3 - (to_string(tmp.id)).size(); i++) cout << ' ';
            cout << '|' << tmp.TME; for(int i = 0; i<3 - (to_string(tmp.TME)).size(); i++) cout << ' ';
            cout << '|' << tmp.TT; for(int i = 0; i<3 - (to_string(tmp.TT)).size(); i++) cout << ' '; cout <<'|'<< endl;
        }
        cout << "========================================================================"<<endl;
        string operacion = to_string(actual.a) + actual.opc + to_string(actual.b);
        int tr = actual.TME - actual.TT;
        cout << "Proceso en Ejecucion"<<endl;
        cout << "|ID |Operacion           |TME|TT |TRes|"<<endl;
        if(seleccionado){
            cout << '|' << actual.id; for(int i = 0; i<3 - (to_string(actual.id)).size(); i++) cout << ' ';
            cout << '|' << operacion ; for(int i = 0; i<20 - (operacion.size()); i++) cout << " ";
            cout << '|' << actual.TME; for(int i = 0; i<3 - (to_string(actual.TME)).size(); i++) cout << " ";
            cout << '|' << actual.TT; for(int i = 0; i < 3 - (to_string(actual.TT)).size(); i++) cout << " ";
            cout << '|' << tr; for(int i = 0; i < 3 - (to_string(tr)).size(); i++) cout << " "; cout << '|' <<endl;
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
            listos.push(bloqueados.front());
            bloqueados.pop();
            liberar.pop();
        }
        cnt++;
        if(seleccionado){
            actual.TT++;
            //Ver si termino de ejecutar
            if(actual.TT == actual.TME){ // Termino de ejecutarse el proceso
                if(actual.Tr == -1){ //Calculamos el tiempo de respuesta, si aún no se ha calculado
                    actual.Tr = actual.TA - actual.TL;
                }
                actual.TF = cnt;
                actual.TE = (actual.TF - actual.TL)- actual.TT;
                seleccionado = false;
                enMemoria--;
                terminados.push_back(actual);
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
        cout << "|ID |Operacion           |Resultado           |TL |TF |TR |Tr |TE |TS |"<<endl;
        for(int i = 0; i < terminados.size(); i++){
            Proceso tmp = terminados[i];
            int retorno = tmp.TF - tmp.TL;
            string opc2 = to_string(tmp.a) + tmp.opc + to_string(tmp.b);
            string res = (tmp.error? "ERROR": to_string(tmp.resultado));
            cout << '|' << tmp.id; for(int i = 0; i < 3 - (to_string(tmp.id)).size(); i++) cout << " ";
            cout << '|' << opc2; for(int i = 0; i < 20 - (opc2.size()); i++) cout << " ";
            cout << '|' << res; for(int i = 0; i < 20 - ((res).size()); i++) cout << " "; 
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