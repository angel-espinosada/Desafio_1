#include <iostream>

using namespace std;

int main()
{
    {
        int opcion=1;
        while(opcion!=0)
        {
            cout<<"Seleccione una opcion"<<endl;
            cout<<"1- Problema 1"<<endl;
            cout<<"2- Problema 2"<<endl;
            cout<<"3- Problema 3"<<endl;
            cout<<"0- para salir"<<endl;
            cin>>opcion;

            switch (opcion) {

            case 1:{

            }
            break;
                break;
            default:{

                if(opcion!=0)
                    cout<<"opcion no valida"<<endl;
                break;
            }
            }
        }
    }
    return 0;
}
