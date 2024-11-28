#include <bits/stdc++.h>
#include <windows.h>
using namespace std; 

//struct proceso -> para almacenar los datos procesos
struct Proceso{
    int TME = 0, id = 0, resultado = 0;
    string nombre = "";
    char opc = 'X';
    int a = 0, b = 0;
};

//struct lote -> para almacenar los 5 procesos que tiene y su ID
struct Lote{
    Proceso arr[5];
};

int main(){
    int totaldelotes, loteAct = 0;
    cout << "Cantidad de procesos que crearas: ";
    int n; cin >> n;
    totaldelotes = (n % 5 == 0? n /5 : (n / 5) + 1);
    set<int> ids;    

    //Vector<lote> -> para guardar los lotes
    vector<Lote> lotes(totaldelotes);
    vector<Proceso> terminados;

    for(int i = 0; i < n; i++){
        system("cls");
        string s;
        int id = -1, a, b, x;
        char c;
        cout << "Ingresa nombre: "; cin >> s;
        cout << endl;
        do{
            system("cls");
            cout << "Ingresa nombre: " << s << endl;
            cout << "Ingresa ID[1 - 10^9]: "; cin>>id; cin.ignore();
        }while(id <= 0 or ids.count(id) == 1);
        cout << endl;
        bool flag = false;
        do{
            system("cls");
            cout << "Ingresa nombre: " << s << endl;
            cout << "Ingresa ID[1 - 10^9]: " << id << endl;
            cout << "Ingresa la operacion que quieras hacer (+, -, *, /, %): "; cin >> c;
            if(c != '%' and c != '/' and c != '+' and c != '-' and c != '*') flag = true;
        }while(flag);
        cout << "Ingresa el primer numero: "; cin >> a; cin.ignore();
        cout << endl;
        flag = false;
        do{
            system("cls");
            cout << "Ingresa nombre: " << s << endl;
            cout << "Ingresa ID[1 - 10^9]: " << id << endl;
            cout << "Ingresa la operacion que quieras hacer (+, -, *, /, %): " << c << endl;
            cout << "Ingresa el primer numero: " << a << endl;
            cout << "Ingresa el segundo numero: "; cin >> b; cin.ignore();
            if((c == '%' or c == '/')  and b == 0) flag = true;
        }while(flag);
        cout << endl;
        do{
            system("cls");
            cout << "Ingresa nombre: " << s << endl;
            cout << "Ingresa ID[1 - 10^9]: " << id << endl;
            cout << "Ingresa la operacion que quieras hacer (+, -, *, /, %): " << c << endl;
            cout << "Ingresa el primer numero: " << a << endl;
            cout << "Ingresa el segundo numero: " << b << endl;
            cout << "Ingresa el tiempo maximo estimado: "; cin >> x; cin.ignore();
        }while(x < 1);
        cout << endl;
        lotes[loteAct].arr[i % 5].nombre = s;
        lotes[loteAct].arr[i % 5].id = id;
        lotes[loteAct].arr[i % 5].a = a;
        lotes[loteAct].arr[i % 5].b = b;
        lotes[loteAct].arr[i % 5].opc = c;
        lotes[loteAct].arr[i % 5].TME = x;
        if(c == '+'){lotes[loteAct].arr[i % 5].resultado = a + b;}
        else if(c == '-'){lotes[loteAct].arr[i % 5].resultado = a - b;}
        else if(c == '*'){lotes[loteAct].arr[i % 5].resultado = a * b;}
        else if(c == '/'){lotes[loteAct].arr[i % 5].resultado = a / b;}
        else lotes[loteAct].arr[i % 5].resultado = a % b;
        
    }

    loteAct = 0;
    int contador = 0;
    int li = 0;
    int tt = 0;
    //id: 10
    //nombre : 15
    //operacion: 21
    //TME: 5
    //TT: 5
    //TR: 5
    int algo;

    while(n>0){
        system("cls");
        cout << "No. Lotes Pendientes: " << (totaldelotes - loteAct - 1 < 0? 0: totaldelotes - loteAct - 1) << endl;
        cout << "Contador: " << contador << endl;
        cout << "Proceso en ejecucion" << endl;
        cout << "ID        Nombre         Operacion            TME  TT   TR   \n";
        int id = lotes[loteAct].arr[li].id;
        string nm = lotes[loteAct].arr[li].nombre;
        string op = to_string(lotes[loteAct].arr[li].a);
        op += lotes[loteAct].arr[li].opc;
        op += to_string(lotes[loteAct].arr[li].b);
        int tme = lotes[loteAct].arr[li].TME;
        cout << id;  for(int i = 0; i < 10 - int(to_string(id).size()); i++) cout << " ";
        cout << nm;  for(int i = 0; i < 15 - int(nm.size()); i++) cout << " ";
        cout << op;  for(int i = 0; i < 21 - int(op.size()); i++) cout << " ";
        cout << tme; for(int i = 0; i < 5 - int(to_string(tme).size()); i++) cout << " ";
        cout << tt;  for(int i = 0; i < 5 - int(to_string(tt).size()); i++) cout << " ";
        cout << tme - tt; for(int i = 0; i < 5 - int(to_string(tme-tt).size()); i++) cout << " ";
        cout << endl << endl;

        cout << "______________________________________________________________" << endl;
        cout << "Procesos pendientes lote " << loteAct+1 << endl;
        cout << "Nombre         TME" << endl;
        for(int i = li+1; i<5; i++){
            string pdNombre = lotes[loteAct].arr[i].nombre;
            int pdTME = lotes[loteAct].arr[i].TME;
            if(pdNombre == "") continue;
            cout << pdNombre; for(int ii = 0; ii < 15 - int(pdNombre.size()); ii++) cout << " ";
            cout << pdTME; for(int ii = 0; ii < 5 - (to_string(pdTME).size()); ii++) cout << " ";
            cout << endl;
        }
        cout << endl << endl;
        cout << "______________________________________________________________" << endl;
        cout << "Procesos Terminados"<<endl;
        cout << "ID        Operacion            Resultado"<<endl;
        for(int i = 0; i<int(terminados.size()); i++){
            int trID = terminados[i].id, trRes = terminados[i].resultado;
            string op = to_string(terminados[i].a);
            op += terminados[i].opc;
            op += to_string(terminados[i].b);
            cout << trID; for(int ii = 0; ii < 10 - int(to_string(trID).size()); ii++) cout << " ";
            cout << op; for(int ii = 0; ii < 21 - int(op.size()); ii++) cout << " ";
            cout << trRes << endl;  
        }
        contador++;
        tt++;
        if(tt == tme){
            terminados.push_back(lotes[loteAct].arr[li]);
            tt = 0;
            li++;
            n--;
        }
        if(li == 5){
            li = 0;
            loteAct++;
        }
        Sleep(1000);
        
    }
    system("cls");
        cout << "No. Lotes Pendientes: " << (totaldelotes - loteAct - 1) << endl;
        cout << "Contador: " << contador << endl;
        cout << "Proceso en ejecucion" << endl;
        cout << "ID        Nombre         Operacion            TME  TT   TR   \n";
        cout << endl << endl;
        cout << "______________________________________________________________" << endl;
        cout << "No hay lotes pendientes";
        cout << endl << endl;
        cout << "______________________________________________________________" << endl;
        cout << "Procesos Terminados"<<endl;
        cout << "ID        Operacion            Resultado"<<endl;
        for(int i = 0; i<int(terminados.size()); i++){
            int trID = terminados[i].id, trRes = terminados[i].resultado;
            string op = to_string(terminados[i].a);
            op += terminados[i].opc;
            op += to_string(terminados[i].b);
            cout << trID; for(int ii = 0; ii < 10 - int(to_string(trID).size()); ii++) cout << " ";
            cout << op; for(int ii = 0; ii < 21 - int(op.size()); ii++) cout << " ";
            cout << trRes << endl;  
        }
    cin.ignore();
    return 0;
}
