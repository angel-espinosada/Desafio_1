#include <iostream>

using namespace std;

void convertir_num_atexto(int numero, char resultado[]);
void compresion_rle(char texto[], char *&resultado, int &tamano );
void descompresion_rle(char *resultado, int tamano, char *&texto);
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

        case 3: {
            char texto[100];
            char *resultado = nullptr;
            int tamano = 0;

            cout << "Ingrese el texto de prueba: " << endl;
            cin >> texto;

            compresion_rle(texto, resultado, tamano);

            cout << "Texto original: " << texto << endl;
            cout << "Codificado (en bytes): ";

            for (int i = 0; i < tamano; i++) {
                printf("%02X ", (unsigned char)resultado[i]);
            }
            cout << endl;

            delete[] resultado;
        }
        break;
            case 4: {
                char texto[100];
                char *resultado = nullptr;
                char *recuperado = nullptr;
                int tamano = 0;

                cout << "Ingrese el texto: ";
                cin >> texto;

                compresion_rle(texto, resultado, tamano);

                cout << "Codificado: ";
                for (int i = 0; i < tamano; i++) {
                    printf("%02X ", (unsigned char)resultado[i]);
                }
                cout << endl;

                descompresion_rle(resultado, tamano, recuperado);

                cout << "Texto recuperado: " << recuperado << endl;

                delete[] resultado;
                delete[] recuperado;
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

void compresion_rle(char texto[], char *&resultado, int &tamano ){
    int i=0;
    int contado_resul=0;


    int longitud=0;
    while(texto[longitud]!='\0'){
        longitud++;
    }


    resultado = new char[longitud*3 + 1];

    while(texto[i]!='\0'){
        char actual_pos=texto[i];
        int contador =1;

        while(texto[i+1]!='\0' && texto[i+1]==actual_pos){
            contador=contador +1;
            i=i+1;
        }


        resultado[contado_resul] = (contador >> 8) & 0xFF;
        resultado[contado_resul+1] = contador & 0xFF;


        resultado[contado_resul+2] = actual_pos;


        contado_resul = contado_resul + 3;
        i=i+1;
    }



    tamano = contado_resul;
}

void descompresion_rle(char *resultado, int tamano, char *&texto) {
    int contado_texto = 0;



    texto = new char[1000];

    for (int i = 0; i < tamano; i += 3) {
        unsigned char byte_alto = resultado[i];
        unsigned char byte_bajo = resultado[i+1];
        char caracter = resultado[i+2];

        int contador = (byte_alto << 8) | byte_bajo;


        for (int j = 0; j < contador; j++) {
            texto[contado_texto] = caracter;
            contado_texto++;
        }
    }


    texto[contado_texto] = '\0';
}
