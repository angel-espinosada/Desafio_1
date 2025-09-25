#include <iostream>

using namespace std;

void convertir_num_atexto(int numero, char resultado[]);
void compresion_rle(char texto[], char resultado[] );
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
            char texto[20];
            char resultado[100];
            cout<<"Ingrese el texto de prueba"<<endl;
            cin>>texto;
            compresion_rle(texto,resultado);
            cout<<texto<<"Codificado"<<resultado<<endl;

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


void convertir_num_atexto(int numero, char resultado[]){
    int i = 0;
    int temporal = numero;

    // convierte la cadena a texto
    for(; temporal > 0; temporal /= 10) {
        int digito = temporal % 10;
        resultado[i] = digito + '0';
        i++;
    }
    resultado[i] = '\0';

    // El texto sale invertido
    int inicio = 0;
    int fin = i - 1;
    while(inicio < fin) {
        char temp = resultado[inicio];
        resultado[inicio] = resultado[fin];
        resultado[fin] = temp;
        inicio++;
        fin--;
    }
}

void compresion_rle(char texto[], char resultado[] ){
    int i=0;
    int contado_resul=0;

    while(texto[i]!='\0'){
        char actual_pos=texto[i];
        int contador =1;
        while(texto[i+1]!='\0'&&texto[i+1]==actual_pos){
            contador=contador +1;
            i=i+1;

        }
       //Convierto el resultado a texto para concatenar
        char numero_texto[10];
        convertir_num_atexto(contador,numero_texto);
        int j=0;
        while(numero_texto[j]!='\0'){

            resultado[contado_resul]=numero_texto[j];
            contado_resul=contado_resul+1;
            j=j+1;

        }
        resultado[contado_resul]=actual_pos;
        contado_resul=contado_resul+1;

        i=i+1;

    }
    resultado[contado_resul]='\0';
}
