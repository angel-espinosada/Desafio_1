#include <iostream>

using namespace std;

int main()
{
    cout << "Bienvenidos al Desafio 1" << endl;
    cout << "Compresion y decodificacion de texto" << endl;
    int opcion=1;

    while(opcion!=0)
    {
        cout<<"Seleccione una opcion"<<endl;
        cout<<"1- Encriptar"<<endl;
        cout<<"2- Desencriptar"<<endl;
        cout<<"3- Comprimir"<<endl;
        cout<<"4-Descomprimir"<<endl;
        cout<<"0- para salir"<<endl;
        cin>>opcion;
        switch (opcion) {
        case 1:{


        }
        break;

        case 2:{


        }
        break;

        case 3:{


        }
        break;
        default:{

            if(opcion!=0)
                cout<<"opcion no valida"<<endl;
            break;
        }
        }
    }

     system("pause");

    return 0;
}
