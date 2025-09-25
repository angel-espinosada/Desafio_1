#include <iostream>

using namespace std;

void convertir_num_atexto(int numero, char resultado[]);
void compresion_rle(char texto[], char *&resultado, int &tamano );
void descompresion_rle(char *resultado, int tamano, char *&texto);
void compresion_lz78(char texto[], unsigned char* &resultado, int &tamano);
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
        cout<<"5-comprimir lz78 "<<endl;
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
            case 5:{
                cout<<"Compresion por lz87"<<endl;
                char texto[] = "ABAABABAABAB";
                unsigned char* resultado; // memoria dinámica
                int tamano;

                compresion_lz78(texto, resultado, tamano);

                cout << "Texto original: " << texto << endl;
                cout << "Comprimido en hex:" << endl;

                for (int i = 0; i < tamano; i++) {
                    printf("%02X ", resultado[i]);
                }
                cout << endl;

                delete[] resultado; // liberar memoria dinámica


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

void compresion_lz78(char texto[], unsigned char* &resultado, int &tamano) {
    // 1) calcular longitud del texto
    int longitud_texto = 0;
    while (texto[longitud_texto] != '\0') longitud_texto++;

    // 2) reservar diccionario dinámico: máximo entradas = longitud_texto
    char **dict = new char*[longitud_texto + 1];   // dict[1..entradas], dict[0] vacío
    int *dictLen = new int[longitud_texto + 1];
    dict[0] = nullptr;
    dictLen[0] = 0;
    int entradas = 0;

    // 3) reservar buffer de salida (tope: 3 * longitud_texto bytes)
    int capacidad = longitud_texto * 3 + 3;
    resultado = new unsigned char[capacidad];
    int contado_resul = 0;

    // 4) recorrido del texto
    int i = 0;
    while (i < longitud_texto) {
        // buscar la entrada del diccionario MÁS LARGA que coincida con texto[i..]
        int best_k = 0;
        int best_len = 0;

        for (int k = 1; k <= entradas; k++) {
            int l = dictLen[k];
            // si no cabe en lo que queda del texto, o no es más larga que mejor actual, saltar
            if (l <= best_len) continue;
            if (i + l > longitud_texto) continue;

            // comparar dict[k] con texto[i..i+l-1]
            int m = 0;
            while (m < l && texto[i + m] == dict[k][m]) m++;
            if (m == l) { // coincide completamente
                best_k = k;
                best_len = l;
            }
        }

        int pos = i + best_len;

        // si la coincidencia llega hasta el final del texto (no hay "siguiente carácter")
        if (pos >= longitud_texto) {
            // emitimos (best_k, 0x00) como marcador de final
            resultado[contado_resul++] = (unsigned char)((best_k >> 8) & 0xFF);
            resultado[contado_resul++] = (unsigned char)(best_k & 0xFF);
            resultado[contado_resul++] = 0x00;
            break;
        }

        // hay un "siguiente carácter" que complementa
        char nuevo = texto[pos];

        // emitir (best_k, nuevo)
        resultado[contado_resul++] = (unsigned char)((best_k >> 8) & 0xFF);
        resultado[contado_resul++] = (unsigned char)(best_k & 0xFF);
        resultado[contado_resul++] = (unsigned char)nuevo;

        // agregar nueva entrada al diccionario: dict[entradas+1] = dict[best_k] + nuevo
        int newLen = dictLen[best_k] + 1;
        char *newStr = new char[newLen + 1]; // +1 para '\0'
        // copiar prefijo
        for (int t = 0; t < dictLen[best_k]; t++) newStr[t] = dict[best_k][t];
        // añadir caracter nuevo
        newStr[newLen - 1] = nuevo;
        newStr[newLen] = '\0';

        // registrar en diccionario
        entradas++;
        dict[entradas] = newStr;
        dictLen[entradas] = newLen;

        // avanzar en el texto
        i = pos + 1;
    }

    tamano = contado_resul;

    // liberar memoria del diccionario
    for (int k = 1; k <= entradas; k++) {
        delete[] dict[k];
    }
    delete[] dict;
    delete[] dictLen;
}
