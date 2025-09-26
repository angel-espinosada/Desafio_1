#include <iostream>
#include <fstream>

using namespace std;
void copiar_cadena(char* destino, char* origen);
int longitud_cadena(char* s);
void convertir_num_atexto(int numero, char resultado[]);
void compresion_rle(char texto[], char *&resultado, int &tamano );
void descompresion_rle(unsigned char *resultado, int tamano, char *&texto);
void compresion_lz78(char texto[], unsigned char* &resultado, int &tamano);
char* descompresion_lz78(unsigned char* datos, int tamano);
char* leer_archivo(char* nombreArchivo);
unsigned char rotacion_dere (unsigned char bite, int n);
unsigned char rotacion_izqui(unsigned char bite, int n);
char* desencriptar(char* datos, int len, int n, unsigned char K);
char* encriptar_xor(char* datos, int len, unsigned char K);
char* desencriptar_xor(char* datos, int len, unsigned char K);
void guardar_archivo(const char* nombreArchivo, char* datos, int len);
char* aplicar_xor(char* datos, int len, unsigned char K);
char* desencriptar_bin(char* datos, int len, int n, unsigned char K);
bool contiene(char* texto, char* fragmento);
char* leer_archivo_bn(const char* nombreArchivo, int& longitud);
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
        cout<<"6-Leyendoo texto "<<endl;
        cout<<"7-descomprimir lz78 "<<endl;
        cout<<"8_totaciones"<<endl;
        cout<<"9_xor"<<endl;
        cout<<"10_xor"<<endl;
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

                descompresion_rle((unsigned char*)resultado, tamano, recuperado);

                cout << "Texto recuperado: " << recuperado << endl;

                delete[] resultado;
                delete[] recuperado;
            }
            break;
            case 5:{
                //cout<<"Compresion por lz87"<<endl;
                //char texto[] = "ABAABABAABAB";
                char archivo[] = "entrada.txt";  // Array de chars
                char* texto = leer_archivo(archivo);
                if (texto == nullptr) {
                    cout << "No se pudo continuar porque falta el archivo." << endl;
                    break;
                }
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
            case 6: {
                char archivo[] = "entrada.txt";
                char* texto = leer_archivo(archivo); // tu función actual (con '\0' al final)
                if (texto == nullptr) {
                    cout << "Archivo no encontrado." << endl;
                    break;
                }

                cout << "Texto original: [" << texto << "]" << endl;

                // Comprimir
                unsigned char* comprimido = nullptr;
                int tamano = 0;
                compresion_lz78(texto, comprimido, tamano);

                cout << "Comprimido en hex: ";
                for (int i = 0; i < tamano; i++) {
                    printf("%02X ", comprimido[i]);
                }
                cout << endl;

                // Descomprimir
                char* resultado = descompresion_lz78(comprimido, tamano);
                if (resultado != nullptr) {
                    cout << "Texto descomprimido: [" << resultado << "]" << endl;

                    // Comparar manualmente (solo para prueba)
                    bool igual = true;
                    for (int i = 0; texto[i] != '\0' || resultado[i] != '\0'; i++) {
                        if (texto[i] != resultado[i]) {
                            igual = false;
                            break;
                        }
                    }
                    if (igual) {
                        cout << "¡Funciona!" << endl;
                    } else {
                        cout << " No coincide." << endl;
                    }
                    delete[] resultado;
                } else {
                    cout << "Descompresión falló." << endl;
                }

                delete[] texto;
                delete[] comprimido;
                break;
            }

            case 7: { // Agregar este caso a tu menú
                cout << "=== PRUEBA COMPLETA LZ78 ===" << endl;

                // 1. Leer archivo
                char archivo[] = "entrada.txt";
                char* texto_original = leer_archivo(archivo);
                if (texto_original == nullptr) {
                    cout << "No se pudo leer el archivo." << endl;
                    break;
                }

                cout << "Texto original: " << texto_original << endl;
                cout << "Longitud: " << longitud_cadena(texto_original) << endl;

                // 2. Comprimir con LZ78
                unsigned char* datos_comprimidos;
                int tamano_comprimido;
                compresion_lz78(texto_original, datos_comprimidos, tamano_comprimido);

                cout << "\n--- COMPRESIÓN ---" << endl;
                cout << "Tamaño comprimido: " << tamano_comprimido << " bytes" << endl;
                cout << "Datos comprimidos (hex): ";
                for (int i = 0; i < tamano_comprimido; i++) {
                    cout << hex << (int)datos_comprimidos[i] << " ";
                }
                cout << dec << endl;

                // Mostrar en formato legible
                cout << "Formato LZ78: ";
                for (int i = 0; i < tamano_comprimido; i += 3) {
                    int indice = (datos_comprimidos[i] << 8) | datos_comprimidos[i + 1];
                    char caracter = datos_comprimidos[i + 2];
                    cout << "(" << indice << ",'" << caracter << "') ";
                }
                cout << endl;

                // 3. Descomprimir
                char* texto_descomprimido = descompresion_lz78(datos_comprimidos, tamano_comprimido);

                cout << "\n--- DESCOMPRESIÓN ---" << endl;
                if (texto_descomprimido != nullptr) {
                    cout << "Texto descomprimido: " << texto_descomprimido << endl;
                    cout << "Longitud: " << longitud_cadena(texto_descomprimido) << endl;

                    // 4. Verificar que son iguales
                    bool iguales = true;
                    int len_original = longitud_cadena(texto_original);
                    int len_descomprimido = longitud_cadena(texto_descomprimido);

                    if (len_original != len_descomprimido) {
                        iguales = false;
                    } else {
                        for (int i = 0; i < len_original; i++) {
                            if (texto_original[i] != texto_descomprimido[i]) {
                                iguales = false;
                                break;
                            }
                        }
                    }

                    cout << "\n--- VERIFICACIÓN ---" << endl;
                    if (iguales) {
                        cout << "✓ ÉXITO: El texto descomprimido es igual al original" << endl;
                    } else {
                        cout << "✗ ERROR: El texto descomprimido NO coincide con el original" << endl;
                    }

                    // Mostrar estadísticas
                    float ratio = (float)tamano_comprimido / (float)len_original * 100.0f;
                    cout << "Ratio de compresión: " << ratio << "%" << endl;

                    delete[] texto_descomprimido;
                } else {
                    cout << "ERROR: No se pudo descomprimir" << endl;
                }

                // Liberar memoria
                delete[] texto_original;
                delete[] datos_comprimidos;
            }
                break;

            case 8: {
                char texto[] = "HOLA";   // Texto de prueba
                int len = 4;             // Longitud
                int n = 2;               // Rotar 2 bits
                unsigned char K = 0x5A;  // Clave XOR

                // Primero encriptamos manualmente (XOR + rot izq)
                char* encriptado = new char[len];
                for (int i = 0; i < len; i++) {
                    unsigned char b = (unsigned char)texto[i];
                    b = rotacion_izqui(b, n); // rotación izquierda para encriptar
                    b = b ^ K;                // XOR con clave
                    encriptado[i] = (char)b;
                }

                cout << "Texto original: " << texto << endl;
                cout << "Encriptado (hex): ";
                for (int i = 0; i < len; i++) {
                    printf("%02X ", (unsigned char)encriptado[i]);
                }
                cout << endl;

                // Ahora desencriptamos con tu función
                char* desencriptado = desencriptar(encriptado, len, n, K);

                cout << "Desencriptado: " << desencriptado << endl;

                delete[] encriptado;
                delete[] desencriptado;
            }
            break;
            case 9: {
                char texto[] = "HOLA MUNDO";
                int len = 10;                 // longitud del texto
                unsigned char K = 0x5A;       // clave XOR

                char* encriptado = encriptar_xor(texto, len, K);

                cout << "Texto original: " << texto << endl;
                cout << "Encriptado (hex): ";
                for (int i = 0; i < len; i++) {
                    printf("%02X ", (unsigned char)encriptado[i]);
                }
                cout << endl;

                char* desencriptado = desencriptar_xor(encriptado, len, K);
                cout << "Desencriptado: " << desencriptado << endl;

                delete[] encriptado;
                delete[] desencriptado;
            }
            break;

            case 10: {
                char nombreIn[] = "entrada.txt";   // archivo de entrada
                char nombreEnc[] = "encriptado.txt"; // salida encriptada
                char nombreDec[] = "desencriptado.txt"; // salida desencriptada

                // leer archivo
                char* texto = leer_archivo(nombreIn);
                if (texto == nullptr) break;

                int len = longitud_cadena(texto);
                unsigned char K = 0x5A; // clave XOR

                cout << "Texto original:" << endl;
                cout << texto << endl;

                // encriptar
                char* encriptado = aplicar_xor(texto, len, K);
                guardar_archivo(nombreEnc, encriptado, len);

                cout << "Archivo encriptado guardado en: " << nombreEnc << endl;

                // desencriptar
                char* desencriptado = aplicar_xor(encriptado, len, K);
                guardar_archivo(nombreDec, desencriptado, len);

                cout << "Archivo desencriptado guardado en: " << nombreDec << endl;

                delete[] texto;
                delete[] encriptado;
                delete[] desencriptado;
            }
            break;
            case 11: {
                char nombreEnc[50], nombrePista[50];
                cout << "Ingrese el nombre del archivo encriptado (ej: Encriptado1.txt): ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista (ej: pista1.txt): ";
                cin >> nombrePista;

                // Leer archivos
                int len_enc, len_pista;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc); // ← debe devolver longitud
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);
                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
                    break;
                }

                // Limpiar pista (eliminar '\n' si existe)
                char* pista = pista_bruta;
                if (len_pista > 0 && pista[len_pista - 1] == '\n') {
                    pista[len_pista - 1] = '\0';
                } else {
                    pista[len_pista] = '\0';
                }

                cout << "Buscando... (pista: [" << pista << "])" << endl;

                bool encontrado = false;
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {
                        // Desencriptar
                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                        // Validar: longitud debe ser múltiplo de 3
                        if (len_enc % 3 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        // Probar RLE
                        char* original_rle = nullptr;
                        descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);
                        if (original_rle && contiene(original_rle, pista)) {
                            cout << "\nENCONTRADO con RLE" << endl;
                            cout << "n = " << n << ", K = 0x" << hex << K << dec << endl;
                            cout << "Mensaje original:\n" << original_rle << endl;
                            encontrado = true;
                            delete[] original_rle;
                        } else if (original_rle) {
                            delete[] original_rle;
                        }

                        // Probar LZ78 (solo si RLE no funcionó)
                        if (!encontrado) {
                            char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);
                            if (original_lz && contiene(original_lz, pista)) {
                                cout << "\n ENCONTRADO con LZ78" << endl;
                                cout << "n = " << n << ", K = 0x" << hex << K << dec << endl;
                                cout << "Mensaje original:\n" << original_lz << endl;
                                encontrado = true;
                                delete[] original_lz;
                            } else if (original_lz) {
                                delete[] original_lz;
                            }
                        }

                        delete[] desencriptado;
                    }
                }

                if (!encontrado) {
                    cout << " No se encontró ninguna combinación válida." << endl;
                }

                delete[] encriptado;
                delete[] pista_bruta;
                break;
            }
            case 12: {
                char nombreEnc[50], nombrePista[50];
                cout << "Ingrese el nombre del archivo encriptado: ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista: ";
                cin >> nombrePista;

                // Leer archivos
                int len_enc, len_pista;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
                    if (encriptado) delete[] encriptado;
                    if (pista_bruta) delete[] pista_bruta;
                    break;
                }

                // Limpiar pista
                char* pista = new char[len_pista + 1];
                int pista_len = 0;
                for (int i = 0; i < len_pista; i++) {
                    if (pista_bruta[i] != '\n' && pista_bruta[i] != '\r') {
                        pista[pista_len++] = pista_bruta[i];
                    }
                }
                pista[pista_len] = '\0';

                cout << "Archivo: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                // VALIDACIÓN: debe ser múltiplo de 3
                if (len_enc % 3 != 0) {
                    cout << "ERROR: Archivo no es múltiplo de 3, no puede ser RLE/LZ78" << endl;
                    delete[] encriptado;
                    delete[] pista_bruta;
                    delete[] pista;
                    break;
                }

                cout << "Buscando combinación correcta..." << endl;
                bool encontrado = false;

                // Buscar combinación correcta
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    cout << "Probando rotación " << n << "..." << endl;

                    for (int K = 0; K <= 255 && !encontrado; K++) {
                        if (K % 50 == 0) {
                            cout << "  Clave " << K << "/255" << endl;
                        }

                        try {
                            // Desencriptar
                            char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                            // Probar RLE
                            char* original_rle = nullptr;
                            descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);

                            if (original_rle && contiene(original_rle, pista)) {
                                cout << "\nENCONTRADO con RLE" << endl;
                                cout << "Rotación: " << n << " bits" << endl;
                                cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                cout << "\nTexto original (primeros 500 chars):" << endl;

                                for (int i = 0; i < 500 && original_rle[i] != '\0'; i++) {
                                    cout << original_rle[i];
                                }
                                cout << endl;

                                encontrado = true;
                                delete[] original_rle;
                                delete[] desencriptado;
                                break;
                            }

                            if (original_rle) delete[] original_rle;

                            // Probar LZ78 si RLE falló
                            if (!encontrado) {
                                char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                                if (original_lz && contiene(original_lz, pista)) {
                                    cout << "\n ENCONTRADO con LZ78" << endl;
                                    cout << "Rotación: " << n << " bits" << endl;
                                    cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                    cout << "\nTexto original (primeros 500 chars):" << endl;

                                    for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) {
                                        cout << original_lz[i];
                                    }
                                    cout << endl;

                                    encontrado = true;
                                    delete[] original_lz;
                                    delete[] desencriptado;
                                    break;
                                }

                                if (original_lz) delete[] original_lz;
                            }

                            delete[] desencriptado;

                        } catch (...) {
                            cout << "Error en combinación n=" << n << " K=" << K << endl;
                            continue;
                        }
                    }
                }

                if (!encontrado) {
                    cout << "No se encontró ninguna combinación válida." << endl;
                }

                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
                break;
            }

        default:
    {

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

void descompresion_rle(unsigned char *resultado, int tamano, char *&texto) {
    /*if (tamano % 3 != 0) {
        texto = nullptr;
        return;
    }
    int contado_texto = 0;

    //texto = new char[100000];
    for (int i = 0; i < tamano; i += 3) {
        unsigned char byte_alto = resultado[i];
        unsigned char byte_bajo = resultado[i+1];
        char caracter = resultado[i+2];
        int contador = (byte_alto << 8) | byte_bajo;

        for (int j = 0; j < contador; j++) {
            texto[contado_texto] = caracter;
            contado_texto++;
        }
        texto = new char[contado_texto + 1];
    }
    texto[contado_texto] = '\0';*/

    if (tamano % 3 != 0) {
        texto = nullptr;
        return;
    }

    // Paso 1: calcular longitud total
    int longitud_total = 0;
    for (int i = 0; i < tamano; i += 3) {
        unsigned char alto = resultado[i];
        unsigned char bajo = resultado[i+1];
        int contador = (alto << 8) | bajo;
        longitud_total += contador;
    }

    // Paso 2: reservar memoria
    texto = new char[longitud_total + 1];

    // Paso 3: descomprimir
    int pos = 0;
    for (int i = 0; i < tamano; i += 3) {
        unsigned char alto = resultado[i];
        unsigned char bajo = resultado[i+1];
        char caracter = resultado[i+2];
        int contador = (alto << 8) | bajo;

        for (int j = 0; j < contador; j++) {
            texto[pos++] = caracter;
        }
    }
    texto[pos] = '\0';

}

void compresion_lz78(char texto[], unsigned char* &resultado, int &tamano) {

    int longitud_texto = 0;
    while (texto[longitud_texto] != '\0') longitud_texto++;
    char **dict = new char*[longitud_texto + 1];
    int *dictLen = new int[longitud_texto + 1];
    dict[0] = nullptr;
    dictLen[0] = 0;
    int entradas = 0;

    int capacidad = longitud_texto * 3 + 3;
    resultado = new unsigned char[capacidad];
    int contado_resul = 0;

    int i = 0;
    while (i < longitud_texto) {
        int best_k = 0;
        int best_len = 0;

        for (int k = 1; k <= entradas; k++) {
            int l = dictLen[k];
            if (l <= best_len) continue;
            if (i + l > longitud_texto) continue;
            int m = 0;
            while (m < l && texto[i + m] == dict[k][m]) m++;
            if (m == l) {
                best_len = l;
                best_k = k;
            }

        }
        int pos = i + best_len;

        if (pos >= longitud_texto) {

            resultado[contado_resul++] = (unsigned char)((best_k >> 8) & 0xFF);
            resultado[contado_resul++] = (unsigned char)(best_k & 0xFF);
            resultado[contado_resul++] = 0x00;
            break;
        }

        char nuevo = texto[pos];

        resultado[contado_resul++] = (unsigned char)((best_k >> 8) & 0xFF);
        resultado[contado_resul++] = (unsigned char)(best_k & 0xFF);
        resultado[contado_resul++] = (unsigned char)nuevo;

        int newLen = dictLen[best_k] + 1;
        char *newStr = new char[newLen + 1];

        for (int t = 0; t < dictLen[best_k]; t++) newStr[t] = dict[best_k][t];

        newStr[newLen - 1] = nuevo;
        newStr[newLen] = '\0';


        entradas++;
        dict[entradas] = newStr;
        dictLen[entradas] = newLen;
        i = pos + 1;
    }
    tamano = contado_resul;

    for (int k = 1; k <= entradas; k++) {
        delete[] dict[k];
    }
    delete[] dict;
    delete[] dictLen;
}


// Leer archivo completo a memoria

char* leer_archivo(char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb"); // abrir en modo binario
    if (!archivo) {

        cout << "Error: el archivo '" << nombreArchivo << "' no existe o no se pudo abrir." << endl;

        return nullptr;
    }

    // ir al final para saber tamaño
    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    // reservar memoria dinámica (+1 para '\0')
    char* buffer = new char[tamano + 1];

    // leer todo el archivo
    size_t leidos = fread(buffer, 1, tamano, archivo);
    buffer[leidos] = '\0'; // fin de cadena

    fclose(archivo);
    return buffer; // devolvemos puntero
}

char* leer_archivo_bn(const char* nombreArchivo, int& longitud) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (!archivo) {
        cout << "Error: el archivo '" << nombreArchivo << "' no existe." << endl;
        longitud = 0;
        return nullptr;
    }

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    char* buffer = new char[tamano]; // ← SIN +1, SIN '\0'

    size_t leidos = fread(buffer, 1, tamano, archivo);
    fclose(archivo);

    longitud = (int)leidos; // ← DEVUELVES LA LONGITUD REAL
    return buffer;
}

// Versión de strlen
int longitud_cadena( char* s) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

// V ersión de strcpy
void copiar_cadena(char* destino, char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

char* descompresion_lz78(unsigned char* datos, int tamano) {
    if (tamano % 3 != 0) return nullptr;

    int maxEntradas = tamano / 3;
    char** dict = new char*[maxEntradas + 1];
    int* dictLen = new int[maxEntradas + 1];

    // Entrada 0: cadena vacía
    dict[0] = nullptr;
    dictLen[0] = 0;

    char* texto = new char[tamano * 10]; // más seguro: peor caso es largo
    int textoLen = 0;
    int entradas = 0;

    for (int i = 0; i < tamano; i += 3) {
        int indice = (datos[i] << 8) | datos[i + 1];
        char c = (char)datos[i + 2];

        // Validación crítica
        if (indice > entradas) {
            // Índice inválido → no es LZ78 válido
            delete[] texto;
            for (int k = 1; k <= entradas; k++) delete[] dict[k];
            delete[] dict;
            delete[] dictLen;
            return nullptr;
        }

        int prefijo_len = (indice == 0) ? 0 : dictLen[indice];
        int newLen = prefijo_len + 1;
        char* nueva = new char[newLen + 1];

        if (indice > 0) {
            for (int j = 0; j < prefijo_len; j++) {
                nueva[j] = dict[indice][j];
            }
        }
        nueva[prefijo_len] = c;
        nueva[newLen] = '\0';

        // Añadir al diccionario
        entradas++;
        dict[entradas] = nueva;
        dictLen[entradas] = newLen;

        // Añadir a salida
        for (int j = 0; j < newLen; j++) {
            texto[textoLen++] = nueva[j];
        }
    }

    texto[textoLen] = '\0';

    // Liberar diccionario
    for (int k = 1; k <= entradas; k++) {
        delete[] dict[k];
    }
    delete[] dict;
    delete[] dictLen;

    return texto;
}

//metodos de encriptacion

unsigned char rotacion_dere (unsigned char bite, int n) {
    n = n % 8;
    return (bite >> n) | (bite << (8 - n));
}


unsigned char rotacion_izqui(unsigned char bite, int n) {
    n = n % 8;
    return (bite << n) | (bite >> (8 - n));
}


char* desencriptar(char* datos, int len, int n, unsigned char K) {
    char* salida = new char[len+1];
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)datos[i];
        b = b ^ K;                 // XOR con clave
        b = rotacion_dere(b, n);    // rotación derecha n bits
        salida[i] = (char)b;
    }
    salida[len] = '\0';
    return salida;
}


char* encriptar_xor(char* datos, int len, unsigned char K) {
    char* salida = new char[len + 1]; // +1 para '\0'
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)datos[i];
        b = b ^ K;         // XOR con la clave
        salida[i] = (char)b;
    }
    salida[len] = '\0';    // terminador
    return salida;
}

char* desencriptar_xor(char* datos, int len, unsigned char K) {
    char* salida = new char[len + 1]; // +1 para '\0'
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)datos[i];
        b = b ^ K;         // XOR con la misma clave
        salida[i] = (char)b;
    }
    salida[len] = '\0';    // terminador
    return salida;
}


void guardar_archivo(const char* nombreArchivo, char* datos, int len) {
    FILE* archivo = fopen(nombreArchivo, "wb");
    if (!archivo) {
        cout << "Error: no se pudo abrir el archivo para escribir." << endl;
        return;
    }
    fwrite(datos, 1, len, archivo);
    fclose(archivo);
}

char* aplicar_xor(char* datos, int len, unsigned char K) {
    char* salida = new char[len + 1]; // +1 para '\0'
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)datos[i];
        b = b ^ K;        // aplicar XOR con la clave
        salida[i] = (char)b;
    }
    salida[len] = '\0';   // terminador
    return salida;
}

bool contiene(char* texto, char* fragmento) {
    if (texto == nullptr || fragmento == nullptr) return false;
    int len_texto = 0, len_frag = 0;
    while (texto[len_texto] != '\0') len_texto++;
    while (fragmento[len_frag] != '\0') len_frag++;
    if (len_frag == 0) return true;
    if (len_frag > len_texto) return false;

    for (int i = 0; i <= len_texto - len_frag; i++) {
        bool coincide = true;
        for (int j = 0; j < len_frag; j++) {
            if (texto[i + j] != fragmento[j]) {
                coincide = false;
                break;
            }
        }
        if (coincide) return true;
    }
    return false;
}

char* desencriptar_bin(char* datos, int len, int n, unsigned char K) {
    char* salida = new char[len];
    for (int i = 0; i < len; i++) {
        unsigned char b = (unsigned char)datos[i];
        b = b ^ K;
        b = ((b >> n) | (b << (8 - n))); // rotación derecha
        salida[i] = (char)b;
    }
     salida[len] = '\0';
    return salida;
}
