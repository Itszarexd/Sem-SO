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
*/
int id_de_procesos = 1;
vector<char> opcs = {'+', '-', '*', '/', '%'};

struct Proceso{
    int TME, id, resultado, TT;
    char opc;
    int a, b;
    bool error;

    //Constructor 
    Proceso(): TME(min(5, rand() % 21)), id(id_de_procesos++), TT(0), error(false){
      opc = opcs[rand() % 5]; 
      a = rand() % 10000;
      b = max(1, rand() % 10000); 
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
    }
};

//struct lote -> para almacenar los 5 procesos que tiene y su ID
struct Lote{
    queue<Proceso> cola;
};

/*
El usuario solo nos dirá cuántos procesos quiere crear.
*/

int main(){
    int totaldelotes;
    cout << "Cantidad de procesos que crearas: ";
    int n; cin >> n;
    totaldelotes = (n % 5 == 0? n /5 : (n / 5) + 1);   
    int auxN = n;
    //Vector<lote> -> para guardar los lotes
    vector<Lote> lotes(totaldelotes);
    for(int i = 0; i<totaldelotes; i++){ //Recorrer los lotes
        int now = min(5, auxN);
        for(int j = 0; j<now; j++){ // Insertar los procesos a los lotes.
            lotes[i].cola.push(Proceso()); // Insertamos proceso al lote
            auxN--; //Le resto uno al numero de procesos faltantes por insertar
        }
    }
    vector<Proceso> terminados;

    int loteActual = 0;
    Proceso ejecutando;
    bool seleccionado = false;
    int tiempo = 0;
    bool activo = true;
    //Mientras N > 0, o sea, que haya procesos por procesar
    while(n > 0){
        system("cls");
        if(!seleccionado and activo){
            ejecutando = lotes[loteActual].cola.front();
            seleccionado = true;
            lotes[loteActual].cola.pop();
        }

        //Numero de procesos faltantes
        cout << "Numero de lotes pendientes: " << totaldelotes - (loteActual + 1) << endl;
        //Proceso en ejecucion: id, opc, TME, TR
        string operacion = to_string(ejecutando.a) + ejecutando.opc + to_string(ejecutando.b);
        int TR = ejecutando.TME - ejecutando.TT;
        cout << "____________________________________________________________________________" << endl;
        cout << "Proceso en ejecucion " << endl;
        cout << "id        |operacion           |TME  |TR   " << endl;
        if(seleccionado){
            cout << ejecutando.id; for(int i = 0; i<(10 - int((to_string(ejecutando.id)).size())); i++) cout << " "; cout << "|";
            cout << operacion; for(int i = 0; i<(20 - int(operacion.size())); i++) cout << " "; cout << "|";
            cout << ejecutando.TME; for(int i = 0; i < (5 - to_string(ejecutando.TME).size()); i++) cout << " "; cout <<"|";
            cout << TR; for(int i = 0; i < (5 - to_string(TR).size()); i++) cout << " ";
            cout << endl; 
        }
        else{
            cout << "NO HAY PROCESOS EN EJECUCION"<<endl;
        }
        
        cout << "____________________________________________________________________________" << endl;
        queue<Proceso> LoteEnEjecucion = lotes[loteActual].cola;
        cout << "Lote en ejecucion: " << loteActual+1 << endl;
        cout << "Procesos pendientes: "<< int(LoteEnEjecucion.size()) << endl;
        cout << "id        |TME  |TT   "<<endl;
        while(!LoteEnEjecucion.empty()){
            Proceso auxP = LoteEnEjecucion.front();
            cout << auxP.id; for(int i = 0; i<(10 - int((to_string(auxP.id)).size())); i++) cout << " "; cout << "|";
            cout << auxP.TME; for(int i = 0; i < (5 - to_string(auxP.TME).size()); i++) cout << " "; cout <<"|";
            cout << auxP.TT; for(int i = 0; i < (5 - to_string(auxP.TT).size()); i++) cout << " "; 
            cout << endl;
            LoteEnEjecucion.pop();
        }

        cout << "____________________________________________________________________________" << endl;
        cout << "Procesos Terminados" << endl;
        cout << "id        |Operacion           |Resultado "<< endl;
        for(int pr = 0; pr < terminados.size(); pr++){
            int T_id = terminados[pr].id;
            string T_operacion = to_string(terminados[pr].a) + terminados[pr].opc + to_string(terminados[pr].b);
            string T_resultado = to_string(terminados[pr].resultado);
            cout << T_id;  for(int i = 0; i<(10 - int((to_string(T_id)).size())); i++) cout << " "; cout << "|";
            cout << T_operacion; for(int i = 0; i<(20 - int(T_operacion.size())); i++) cout << " "; cout << "|";
            if(terminados[pr].error){
                cout << "ERROR     ";
            }
            else{
                cout << T_resultado; for(int i = 0; i<(10 - int(T_resultado.size())); i++) cout << " ";
            }
            cout << endl;
        }
        cout << "Contador: " << tiempo;

        if(_kbhit()){
            char tecla = _getch();
            if(tecla == 'i'){
                lotes[loteActual].cola.push(ejecutando);
                seleccionado = false;
            }
            if(tecla == 'e'){
                ejecutando.error = true;
                terminados.push_back(ejecutando);
                seleccionado = false;
                n--;
            }
            if(tecla == 'p'){
                activo = false;
            }
            if(tecla == 'c'){
                activo = true;
            }
        }

        if(activo){
            tiempo++;
            if(seleccionado){
                ejecutando.TT++;
                if(ejecutando.error == false and ejecutando.TME == ejecutando.TT){
                    terminados.push_back(ejecutando);
                    seleccionado = false;
                    n--;
                }
            }
            Sleep(1000);
        }
        if(seleccionado == false and lotes[loteActual].cola.empty()){
            loteActual++;
        }

    }
    seleccionado = false;
    system("cls");
        if(!seleccionado and activo){
            ejecutando = lotes[loteActual].cola.front();
            seleccionado = true;
            lotes[loteActual].cola.pop();
        }

        //Numero de procesos faltantes
        cout << "Numero de lotes pendientes: " << 0 << endl;
        //Proceso en ejecucion: id, opc, TME, TR
        string operacion = to_string(ejecutando.a) + ejecutando.opc + to_string(ejecutando.b);
        int TR = ejecutando.TME - ejecutando.TT;
        cout << "____________________________________________________________________________" << endl;
        cout << "Proceso en ejecucion " << endl;
        cout << "id         |operacion            |TME   |TR   " << endl;
        cout << "NO HAY PROCESOS EN EJECUCION"<<endl;
       
        
        cout << "____________________________________________________________________________" << endl;
        cout << "No hay lote pendiente"<<endl;

        cout << "____________________________________________________________________________" << endl;
        cout << "Procesos Terminados" << endl;
        cout << "id        |Operacion           |Resultado "<< endl;
        for(int pr = 0; pr < terminados.size(); pr++){
            int T_id = terminados[pr].id;
            string T_operacion = to_string(terminados[pr].a) + terminados[pr].opc + to_string(terminados[pr].b);
            string T_resultado = to_string(terminados[pr].resultado);
            cout << T_id;  for(int i = 0; i<(10 - int((to_string(T_id)).size())); i++) cout << " "; cout << "|";
            cout << T_operacion; for(int i = 0; i<(20 - int(T_operacion.size())); i++) cout << " "; cout << "|";
            if(terminados[pr].error){
                cout << "ERROR     ";
            }
            else{
                cout << T_resultado; for(int i = 0; i<(10 - int(T_resultado.size())); i++) cout << " ";
            }
            cout << endl;
        }
        cout << "Contador: " << tiempo;
        while(true){
            if(_kbhit()){
                char finalEsc = _getch();
                if(finalEsc == char(27)){
                    break;
                }
            }
        }
    return 0;
}
