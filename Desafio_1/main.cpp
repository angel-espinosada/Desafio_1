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
bool buscar_lz78_limpio(const char* nombreEnc, const char* nombrePista);

int main() {
    int opcion=1;
    while(opcion!=0){
    cout << "Bienvenidos al Desafio 1" << endl;
    cout << "Compresion y decodificacion de texto" << endl;

    cout<<"Seleccione una opcion"<<endl;
    cout<<"1- Descomprimir y desencriptar de forma automatica"<<endl;
    cout<<"2- Manual, pista y archivo encriptado"<<endl;
    cout<<"0-Ingrese 0, para salir."<<endl;
    cin>>opcion;
    switch (opcion) {
    case 1:{
        int n;
        cout << "Ingrese el numero de casos a evaluar: ";
        cin >> n;

        for (int i = 1; i <= n; i++) {
            char nombreEnc[50], nombrePista[50];
            sprintf(nombreEnc, "Encriptado%d.txt", i);
            sprintf(nombrePista, "pista%d.txt", i);

            cout << "\n=== Procesando caso " << i << " ===" << endl;

            // Leer archivos
            int len_enc = 0, len_pista = 0;
            char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
            char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

            if (!encriptado || !pista_bruta) {
                cout << "Error: no se pudieron leer los archivos del caso " << i << endl;
                continue;
            }

            // Limpiar pista
            char* pista = new char[len_pista + 1];
            int pista_len = 0;
            for (int j = 0; j < len_pista; j++) {
                if (pista_bruta[j] != '\n' && pista_bruta[j] != '\r') {
                    pista[pista_len++] = pista_bruta[j];
                }
            }
            pista[pista_len] = '\0';

            // Intentar RLE
            bool encontrado = false;
            for (int n_rot = 1; n_rot <= 7 && !encontrado; n_rot++) {
                for (int K = 0; K <= 255 && !encontrado; K++) {
                    char* desencriptado = desencriptar_bin(encriptado, len_enc, n_rot, (unsigned char)K);
                    /*if (len_enc % 3 != 0) {
                        delete[] desencriptado;
                        continue;
                    }
*/
                    char* original = nullptr;
                    descompresion_rle((unsigned char*)desencriptado, len_enc, original);
                    if (original && contiene(original, pista)) {
                        cout << "Método: RLE" << endl;
                        cout << "Rotación: " << n_rot << " bits" << endl;
                        cout << "Clave XOR: 0x" << hex << K << dec << endl;
                        cout << "Mensaje original:\n" << original << endl;
                        encontrado = true;
                    }
                    delete[] original;
                    delete[] desencriptado;
                }
            }

            // Si no se encontró, intentar LZ78
            if (!encontrado) {
                for (int n_rot = 1; n_rot <= 7 && !encontrado; n_rot++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {
                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n_rot, (unsigned char)K);
                        //if (len_enc % 3 != 0) {
                            //delete[] desencriptado;
                            //continue;
                        //}

                        char* original = descompresion_lz78((unsigned char*)desencriptado, len_enc);
                        if (original && contiene(original, pista)) {
                            cout << "Método: LZ78" << endl;
                            cout << "Rotación: " << n_rot << " bits" << endl;
                            cout << "Clave XOR: 0x" << hex << K << dec << endl;
                            cout << "Mensaje original:\n" << original << endl;
                            encontrado = true;
                        }
                        delete[] original;
                        delete[] desencriptado;
                    }
                }
            }

            if (!encontrado) {
                cout << "❌ No se pudo resolver el caso " << i << endl;
            }

            // Liberar memoria
            delete[] encriptado;
            delete[] pista_bruta;
            delete[] pista;
        }

    }
    break;

    case 2:{

        char nombreEnc[50], nombrePista[50];
        cout << "Ingrese el nombre del archivo encriptado (ej: Encriptado1.txt): ";
        cin >> nombreEnc;
        cout << "Ingrese el nombre del archivo de pista (ej: pista1.txt): ";
        cin >> nombrePista;

        bool encontrado = false;

        // ===============================
        // INTENTAR CON RLE PRIMERO
        // ===============================
        cout << "\n=== Intentando con RLE ===" << endl;

        int len_enc = 0, len_pista = 0;
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

        cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
        cout << "Pista: [" << pista << "]" << endl;

        // Buscar con RLE
        for (int n = 1; n <= 7 && !encontrado; n++) {
            for (int K = 0; K <= 255 && !encontrado; K++) {
                char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                if (len_enc % 2 != 0) {
                    delete[] desencriptado;
                    continue;
                }

                char* original_rle = nullptr;
                descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);

                if (original_rle && contiene(original_rle, pista)) {
                    // Verificar recompresión
                    char* recomp = nullptr;
                    int tam_recomp = 0;
                    compresion_rle(original_rle, recomp, tam_recomp);

                    bool igual_bytes = false;
                    if (recomp && tam_recomp == len_enc) {
                        igual_bytes = true;
                        for (int b = 0; b < tam_recomp; b++) {
                            if ((unsigned char)recomp[b] != (unsigned char)desencriptado[b]) {
                                igual_bytes = false;
                                break;
                            }
                        }
                    }

                    if (recomp) delete[] recomp;

                    if (igual_bytes) {
                        cout << "\n ENCONTRADO con RLE" << endl;
                        cout << "Rotación: " << n << " bits" << endl;
                        cout << "Clave XOR: 0x" << hex << K << dec << endl;
                        cout << "Texto original (primeros 500 chars):" << endl;
                        for (int i = 0; i < 500 && original_rle[i] != '\0'; i++) cout << original_rle[i];
                        cout << endl;
                        encontrado = true;
                    }
                }

                if (original_rle) delete[] original_rle;
                delete[] desencriptado;
            }
        }

        // Limpiar memoria de RLE
        delete[] encriptado;
        delete[] pista_bruta;
        delete[] pista;

        // ===============================
        // SI RLE NO FUNCIONA, LLAMAR FUNCIÓN LZ78 LIMPIA
        // ===============================
        if (!encontrado) {
            cout << "\n--- RLE no funcionó, intentando LZ78 desde cero ---" << endl;
            encontrado = buscar_lz78_limpio(nombreEnc, nombrePista);
        }

        if (!encontrado) {
            cout << "\n❌ No se encontró ninguna combinación (RLE o LZ78)." << endl;
        }

        break;
    }
    break;

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
        /*
        cout<<"Seleccione una opcion"<<endl;
        cout<<"1- Encriptar"<<endl;
        cout<<"2- Desencriptar"<<endl;
        cout<<"3- Comprimir en RLE"<<endl;
        cout<<"4-Descomprimir"<<endl;
        cout<<"5-comprimir lz78 "<<endl;
        cout<<"6-Leyendoo texto "<<endl;
        cout<<"7-descomprimir lz78 "<<endl;
        cout<<"8_totaciones"<<endl;
        cout<<"9_xor"<<endl;
        cout<<"10_xor"<<endl;
        cout<<"13 prueba RLE"<<endl;
        cout<<"14 prueba lz78"<<endl;
        cout<<"17 prueba lz78"<<endl;
        cout<<"18 por qen prueba lz78"<<endl;
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

                // Leer archivos (binario)
                int len_enc = 0, len_pista = 0;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
                    if (encriptado) delete[] encriptado;
                    if (pista_bruta) delete[] pista_bruta;
                    break;
                }

                // Limpiar pista (eliminar \r \n)
                char* pista = new char[len_pista + 1];
                int pista_len = 0;
                for (int i = 0; i < len_pista; i++) {
                    if (pista_bruta[i] != '\n' && pista_bruta[i] != '\r') {
                        pista[pista_len++] = pista_bruta[i];
                    }
                }
                pista[pista_len] = '\0';

                cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                bool encontrado = false;

                // ===============================
                // 1) Intentar con RLE (todas las combinaciones)
                // ===============================
                cout << "\n=== Intentando con RLE ===" << endl;
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {

                        // Desencriptar (binario) usando tu función (longitud permanece len_enc)
                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                        // Validar formato RLE: longitud debe ser múltiplo de 3
                        if (len_enc % 3 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        // Intentar descomprimir RLE
                        char* original_rle = nullptr;
                        descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);

                        if (original_rle) {
                            // Si contiene la pista, verificamos recompresion-byte-a-byte
                            if (contiene(original_rle, pista)) {
                                // Recomprimos el texto obtenido y comparamos bytes
                                char* recomp = nullptr;
                                int tam_recomp = 0;
                                compresion_rle(original_rle, recomp, tam_recomp); // tu función original

                                bool igual_bytes = false;
                                if (recomp != nullptr && tam_recomp == len_enc) {
                                    igual_bytes = true;
                                    for (int b = 0; b < tam_recomp; b++) {
                                        if ((unsigned char)recomp[b] != (unsigned char)desencriptado[b]) {
                                            igual_bytes = false;
                                            break;
                                        }
                                    }
                                }

                                if (recomp) delete[] recomp;

                                if (igual_bytes) {
                                    cout << "\n✅ ENCONTRADO con RLE (verificado por recompresión)" << endl;
                                    cout << "Rotación: " << n << " bits" << endl;
                                    cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                    cout << "\nTexto original (primeros 500 chars):" << endl;
                                    for (int i = 0; i < 500 && original_rle[i] != '\0'; i++) cout << original_rle[i];
                                    cout << endl;
                                    encontrado = true;
                                } else {
                                    // falso positivo; seguir buscando
                                }
                            }
                            delete[] original_rle;
                        }

                        delete[] desencriptado;
                    }
                }

                // ===============================
                // 2) Si no se encontró con RLE, intentar LZ78
                // ===============================
                if (!encontrado) {
                    cout << "\n=== Intentando con LZ78 ===" << endl;
                    for (int n = 1; n <= 7 && !encontrado; n++) {
                        for (int K = 0; K <= 255 && !encontrado; K++) {

                            char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                            // Validar formato LZ78: longitud comprimida también debe venir en bloques de 3
                            if (len_enc % 3 != 0) {
                                delete[] desencriptado;
                                continue;
                            }

                            // Intentar descomprimir LZ78
                            char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                            if (original_lz) {
                                if (contiene(original_lz, pista)) {
                                    // Recomprimir LZ78 y comparar bytes
                                    unsigned char* recomp_lz = nullptr;
                                    int tam_recomp_lz = 0;
                                    compresion_lz78(original_lz, recomp_lz, tam_recomp_lz);

                                    bool igual_bytes = false;
                                    if (recomp_lz != nullptr && tam_recomp_lz == len_enc) {
                                        igual_bytes = true;
                                        for (int b = 0; b < tam_recomp_lz; b++) {
                                            if ((unsigned char)recomp_lz[b] != (unsigned char)desencriptado[b]) {
                                                igual_bytes = false;
                                                break;
                                            }
                                        }
                                    }

                                    if (recomp_lz) delete[] recomp_lz;

                                    if (igual_bytes) {
                                        cout << "\n✅ ENCONTRADO con LZ78 (verificado por recompresión)" << endl;
                                        cout << "Rotación: " << n << " bits" << endl;
                                        cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                        cout << "\nTexto original (primeros 500 chars):" << endl;
                                        for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) cout << original_lz[i];
                                        cout << endl;
                                        encontrado = true;
                                    } else {
                                        // falso positivo
                                    }
                                }
                                delete[] original_lz;
                            }

                            delete[] desencriptado;
                        }
                    }
                }

                if (!encontrado) {
                    cout << "\n❌ No se encontró ninguna combinación verificada." << endl;
                }

                // liberar recursos
                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
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
            case 13: {
                char original[] = "AAAAABBB";
                cout << "Texto original: [" << original << "]" << endl;

                // Comprimir
                char* comprimido = nullptr;
                int tam_comp = 0;
                compresion_rle(original, comprimido, tam_comp);

                cout << "Comprimido (" << tam_comp << " bytes): ";
                for (int i = 0; i < tam_comp; i++) {
                    printf("%02X ", (unsigned char)comprimido[i]);
                }
                cout << endl;

                // Descomprimir
                char* recuperado = nullptr;
                descompresion_rle((unsigned char*)comprimido, tam_comp, recuperado);

                if (recuperado) {
                    cout << "Texto descomprimido: [" << recuperado << "]" << endl;

                    // Comparar
                    bool ok = true;
                    for (int i = 0; original[i] != '\0' || recuperado[i] != '\0'; i++) {
                        if (original[i] != recuperado[i]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        cout << "✅ RLE: ÉXITO" << endl;
                    } else {
                        cout << "❌ RLE: FALLÓ" << endl;
                    }
                    delete[] recuperado;
                } else {
                    cout << "❌ RLE: descompresión devolvió nullptr" << endl;
                }

                delete[] comprimido;
                break;
            }

            case 14: {
                char original[] = "aaaaabbbbbbccccccddddeeeeffffgggghhhh";
                cout << "Texto original: [" << original << "]" << endl;

                // Comprimir
                unsigned char* comprimido = nullptr;
                int tam_comp = 0;
                compresion_lz78(original, comprimido, tam_comp);

                cout << "Comprimido (" << tam_comp << " bytes): ";
                for (int i = 0; i < tam_comp; i++) {
                    printf("%02X ", comprimido[i]);
                }
                cout << endl;

                // Mostrar bloques
                cout << "Bloques LZ78: ";
                for (int i = 0; i < tam_comp; i += 3) {
                    int idx = (comprimido[i] << 8) | comprimido[i+1];
                    char c = comprimido[i+2];
                    cout << "(" << idx << ",'" << c << "') ";
                }
                cout << endl;

                // Descomprimir
                char* recuperado = descompresion_lz78(comprimido, tam_comp);
                if (recuperado) {
                    cout << "Texto descomprimido: [" << recuperado << "]" << endl;

                    // Comparar
                    bool ok = true;
                    for (int i = 0; original[i] != '\0' || recuperado[i] != '\0'; i++) {
                        if (original[i] != recuperado[i]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        cout << "✅ LZ78: ÉXITO" << endl;
                    } else {
                        cout << "❌ LZ78: FALLÓ" << endl;
                    }
                    delete[] recuperado;
                } else {
                    cout << "❌ LZ78: descompresión devolvió nullptr" << endl;
                }

                delete[] comprimido;
                break;
            }


            case 16: {
                char texto_original[] = "BABA BABA BABA";
                cout << "Texto original: [" << texto_original << "]" << endl;

                // 1. Comprimir en RLE (con tu versión)
                char* comprimido = nullptr;
                int tam_comp = 0;
                compresion_rle(texto_original, comprimido, tam_comp);

                cout << "Comprimido (" << tam_comp << " bytes)" << endl;

                // 2. Encriptar con rotación + XOR
                int n = 3;
                unsigned char K = 0x5A;
                char* encriptado = desencriptar(comprimido, tam_comp, n, K);
                // 👆 usamos desencriptar porque es reversible (sirve para encriptar también)

                // 3. Guardar archivo encriptado
                guardar_archivo("EncriptadoPrueba.txt", encriptado, tam_comp);
                cout << "Archivo encriptado generado: EncriptadoPrueba.txt" << endl;

                // 4. Guardar pista
                char pista[] = "BABA";
                guardar_archivo("pistaPrueba.txt", pista, longitud_cadena(pista));
                cout << "Archivo de pista generado: pistaPrueba.txt" << endl;

                // Liberar memoria
                delete[] comprimido;
                delete[] encriptado;

                break;
            }

            case 17: {
                char nombreEnc[260], nombrePista[260];
                cout << "Ingrese el nombre del archivo encriptado (ej: encriptado2.txt): ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista (ej: pista1.txt): ";
                cin >> nombrePista;

                // Leer archivos (binario)
                int len_enc = 0, len_pista = 0;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc); // debe leer binario y devolver len
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
                    if (encriptado) delete[] encriptado;
                    if (pista_bruta) delete[] pista_bruta;
                    break;
                }

                // Limpiar pista (eliminar \r \n)
                char* pista = new char[len_pista + 1];
                int pista_len = 0;
                for (int i = 0; i < len_pista; i++) {
                    if (pista_bruta[i] != '\n' && pista_bruta[i] != '\r') {
                        pista[pista_len++] = pista_bruta[i];
                    }
                }
                pista[pista_len] = '\0';

                cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                bool encontrado = false;

                cout << "\n=== Intentando con LZ78 (solo LZ78) ===" << endl;

                // mostrar primer bloque del archivo encriptado para inspección
                cout << "HEAD encriptado (hex, 64 bytes max): ";
                for (int i = 0; i < len_enc && i < 64; i++) printf("%02X ", (unsigned char)encriptado[i]);
                cout << endl;

                // bucle de búsqueda (rotación n de 1..7, clave K de 0..255)
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    // opción: imprimir progreso cada cierta K grande para no saturar
                    for (int K = 0; K <= 255 && !encontrado; K++) {

                        // Desencriptar binario: desencriptar_bin debe devolver char* con len_enc bytes (binarios)
                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);
                        if (!desencriptado) continue;

                        // DEBUG rápido: imprime los primeros 12 bytes del resultado desencriptado
                        if (K % 64 == 0) {
                            cout << "n=" << n << " K=0x" << hex << K << dec << " -> HEAD desencriptado: ";
                            for (int i = 0; i < len_enc && i < 24; i++) printf("%02X ", (unsigned char)desencriptado[i]);
                            cout << endl;
                        }

                        // Validar formato LZ78: tamaño debe ser múltiplo de 3
                        if (len_enc % 3 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        // Intentar descomprimir LZ78
                        char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                        if (original_lz) {
                            // si contiene la pista, verificamos recompresión-byte-a-byte
                            if (contiene(original_lz, pista)) {
                                // Recomprimir LZ78 y comparar bytes
                                unsigned char* recomp_lz = nullptr;
                                int tam_recomp_lz = 0;
                                compresion_lz78(original_lz, recomp_lz, tam_recomp_lz);

                                bool igual_bytes = false;
                                if (recomp_lz != nullptr && tam_recomp_lz == len_enc) {
                                    igual_bytes = true;
                                    for (int b = 0; b < tam_recomp_lz; b++) {
                                        if ((unsigned char)recomp_lz[b] != (unsigned char)desencriptado[b]) {
                                            igual_bytes = false;
                                            break;
                                        }
                                    }
                                }

                                if (recomp_lz) delete[] recomp_lz;

                                if (igual_bytes) {
                                    cout << "\n✅ ENCONTRADO con LZ78 (verificado por recompresión)" << endl;
                                    cout << "Rotación: " << n << " bits" << endl;
                                    cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                    cout << "\nTexto original (primeros 500 chars):" << endl;
                                    for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) cout << original_lz[i];
                                    cout << endl;
                                    encontrado = true;
                                } else {
                                    // falso positivo; continuar búsqueda
                                }
                            }
                            delete[] original_lz;
                        }

                        delete[] desencriptado;
                    }
                }

                if (!encontrado) {
                    cout << "\n❌ No se encontró ninguna combinación verificada (LZ78)." << endl;
                }

                // liberar recursos
                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
                break;
            }
            case 18: {
                char nombreEnc[] = "Encriptado4.txt";
                char nombrePista[] = "pista4.txt";

                // Leer archivos
                int len_enc = 0, len_pista = 0;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
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

                // Probar SOLO con LZ78 y parámetros conocidos (n=3, K=0x40)
                int n = 3;
                unsigned char K = 0x40;

                cout << "Probando LZ78 con n=" << n << ", K=0x" << hex << (int)K << dec << "..." << endl;

                char* desencriptado = desencriptar_bin(encriptado, len_enc, n, K);
                char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                if (original_lz) {
                    cout << "Texto descomprimido (primeros 200 chars):" << endl;
                    for (int i = 0; i < 200 && original_lz[i] != '\0'; i++) {
                        cout << original_lz[i];
                    }
                    cout << endl;

                    if (contiene(original_lz, pista)) {
                        cout << "\n✅ ¡ÉXITO! LZ78 funcionó con los parámetros correctos." << endl;
                    } else {
                        cout << "\n❌ La pista NO se encontró en el texto descomprimido." << endl;
                    }
                    delete[] original_lz;
                } else {
                    cout << "\n❌ descompresion_lz78 devolvió nullptr." << endl;
                }

                delete[] desencriptado;
                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
                break;
            }

            case 19: {//funciona a parte lz78
                char nombreEnc[50], nombrePista[50];
                cout << "Ingrese el nombre del archivo encriptado (ej: Encriptado2.txt): ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista (ej: pista2.txt): ";
                cin >> nombrePista;

                int len_enc = 0, len_pista = 0;
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

                cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                bool encontrado = false;

                cout << "\n=== Intentando con LZ78 (solo LZ78) ===" << endl;

                for (int n = 1; n <= 7 && !encontrado; n++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {

                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                        if (len_enc % 3 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        // 🔎 DEBUG: mostrar primeros 10 bloques
                        cout << "\nn=" << n << " K=0x" << hex << K << dec << " -> Bloques:" << endl;
                        for (int i = 0; i < len_enc && i < 30; i += 3) {
                            int idx = (desencriptado[i] << 8) | (unsigned char)desencriptado[i + 1];
                            unsigned char c = (unsigned char)desencriptado[i + 2];
                            cout << "(" << idx << ",'" << (isprint(c) ? (char)c : '.') << "') ";
                        }
                        cout << endl;

                        // Intentar descompresión
                        char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                        if (original_lz) {
                            if (contiene(original_lz, pista)) {
                                cout << "\n✅ ENCONTRADO con LZ78" << endl;
                                cout << "Rotación: " << n << " bits" << endl;
                                cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                cout << "\nTexto original (primeros 500 chars):" << endl;
                                for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) cout << original_lz[i];
                                cout << endl;
                                encontrado = true;
                            }
                            delete[] original_lz;
                        }

                        delete[] desencriptado;
                    }
                }

                if (!encontrado) {
                    cout << "\n❌ No se encontró ninguna combinación verificada (LZ78)." << endl;
                }

                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
                break;
            }
            case 20: {
                char nombreEnc[50], nombrePista[50];
                cout << "Ingrese el nombre del archivo encriptado (ej: Encriptado1.txt): ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista (ej: pista1.txt): ";
                cin >> nombrePista;

                // Leer archivos (binario)
                int len_enc = 0, len_pista = 0;
                char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
                char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

                if (!encriptado || !pista_bruta) {
                    cout << "Error al leer archivos." << endl;
                    if (encriptado) delete[] encriptado;
                    if (pista_bruta) delete[] pista_bruta;
                    break;
                }

                // Limpiar pista (eliminar \r \n)
                char* pista = new char[len_pista + 1];
                int pista_len = 0;
                for (int i = 0; i < len_pista; i++) {
                    if (pista_bruta[i] != '\n' && pista_bruta[i] != '\r') {
                        pista[pista_len++] = pista_bruta[i];
                    }
                }
                pista[pista_len] = '\0';

                cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                bool encontrado = false;

                // ===============================
                // Intentar con RLE
                // ===============================
                cout << "\n=== Intentando con RLE ===" << endl;
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {

                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                        // Validación rápida
                        if (len_enc % 2 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        char* original_rle = nullptr;
                        descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);

                        if (original_rle) {
                            if (contiene(original_rle, pista)) {
                                // Recomprueba
                                char* recomp = nullptr;
                                int tam_recomp = 0;
                                compresion_rle(original_rle, recomp, tam_recomp);

                                bool igual_bytes = false;
                                if (recomp != nullptr && tam_recomp == len_enc) {
                                    igual_bytes = true;
                                    for (int b = 0; b < tam_recomp; b++) {
                                        if ((unsigned char)recomp[b] != (unsigned char)desencriptado[b]) {
                                            igual_bytes = false;
                                            break;
                                        }
                                    }
                                }

                                if (recomp) delete[] recomp;

                                if (igual_bytes) {
                                    cout << "\n✅ ENCONTRADO con RLE" << endl;
                                    cout << "Rotación: " << n << " bits" << endl;
                                    cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                    cout << "Texto original (primeros 500 chars):" << endl;
                                    for (int i = 0; i < 500 && original_rle[i] != '\0'; i++) cout << original_rle[i];
                                    cout << endl;
                                    encontrado = true;
                                }
                            }
                            delete[] original_rle;




                        }
                        delete[] desencriptado;
                    }
                }

                // ===============================
                // Intentar con LZ78
                // ===============================
                if (!encontrado) {
                    cout << "\n=== Intentando con LZ78 ===" << endl;
                    for (int n = 1; n <= 7 && !encontrado; n++) {
                        for (int K = 0; K <= 255 && !encontrado; K++) {

                            char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                            if (len_enc % 3 != 0) {
                                delete[] desencriptado;
                                continue;
                            }

                            // Debug: ver primeros bytes en hex
                            cout << "n=" << n << " K=0x" << hex << K << dec << " -> HEAD desencriptado: ";
                            for (int i = 0; i < 24 && i < len_enc; i++) {
                                printf("%02X ", (unsigned char)desencriptado[i]);
                            }
                            cout << endl;

                            char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

                            if (original_lz) {
                                if (contiene(original_lz, pista)) {
                                    // Recomprueba
                                    unsigned char* recomp_lz = nullptr;
                                    int tam_recomp_lz = 0;
                                    compresion_lz78(original_lz, recomp_lz, tam_recomp_lz);

                                    bool igual_bytes = false;
                                    if (recomp_lz != nullptr && tam_recomp_lz == len_enc) {
                                        igual_bytes = true;
                                        for (int b = 0; b < tam_recomp_lz; b++) {
                                            if ((unsigned char)recomp_lz[b] != (unsigned char)desencriptado[b]) {
                                                igual_bytes = false;
                                                break;
                                            }
                                        }
                                    }

                                    if (recomp_lz) delete[] recomp_lz;

                                    if (igual_bytes) {
                                        cout << "\n✅ ENCONTRADO con LZ78" << endl;
                                        cout << "Rotación: " << n << " bits" << endl;
                                        cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                        cout << "Texto original (primeros 500 chars):" << endl;
                                        for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) cout << original_lz[i];
                                        cout << endl;
                                        encontrado = true;
                                    }
                                }
                                delete[] original_lz;
                            }
                            delete[] desencriptado;
                        }
                    }
                }

                if (!encontrado) {
                    cout << "\n❌ No se encontró ninguna combinación (RLE o LZ78)." << endl;
                }

                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;
                break;
            }
            case 21: {
                char nombreEnc[50], nombrePista[50];
                cout << "Ingrese el nombre del archivo encriptado (ej: Encriptado1.txt): ";
                cin >> nombreEnc;
                cout << "Ingrese el nombre del archivo de pista (ej: pista1.txt): ";
                cin >> nombrePista;

                bool encontrado = false;

                // ===============================
                // INTENTAR CON RLE PRIMERO
                // ===============================
                cout << "\n=== Intentando con RLE ===" << endl;

                int len_enc = 0, len_pista = 0;
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

                cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
                cout << "Pista: [" << pista << "]" << endl;

                // Buscar con RLE
                for (int n = 1; n <= 7 && !encontrado; n++) {
                    for (int K = 0; K <= 255 && !encontrado; K++) {
                        char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

                        if (len_enc % 2 != 0) {
                            delete[] desencriptado;
                            continue;
                        }

                        char* original_rle = nullptr;
                        descompresion_rle((unsigned char*)desencriptado, len_enc, original_rle);

                        if (original_rle && contiene(original_rle, pista)) {
                            // Verificar recompresión
                            char* recomp = nullptr;
                            int tam_recomp = 0;
                            compresion_rle(original_rle, recomp, tam_recomp);

                            bool igual_bytes = false;
                            if (recomp && tam_recomp == len_enc) {
                                igual_bytes = true;
                                for (int b = 0; b < tam_recomp; b++) {
                                    if ((unsigned char)recomp[b] != (unsigned char)desencriptado[b]) {
                                        igual_bytes = false;
                                        break;
                                    }
                                }
                            }

                            if (recomp) delete[] recomp;

                            if (igual_bytes) {
                                cout << "\n✅ ENCONTRADO con RLE" << endl;
                                cout << "Rotación: " << n << " bits" << endl;
                                cout << "Clave XOR: 0x" << hex << K << dec << endl;
                                cout << "Texto original (primeros 500 chars):" << endl;
                                for (int i = 0; i < 500 && original_rle[i] != '\0'; i++) cout << original_rle[i];
                                cout << endl;
                                encontrado = true;
                            }
                        }

                        if (original_rle) delete[] original_rle;
                        delete[] desencriptado;
                    }
                }

                // Limpiar memoria de RLE
                delete[] encriptado;
                delete[] pista_bruta;
                delete[] pista;

                // ===============================
                // SI RLE NO FUNCIONA, LLAMAR FUNCIÓN LZ78 LIMPIA
                // ===============================
                if (!encontrado) {
                    cout << "\n--- RLE no funcionó, intentando LZ78 desde cero ---" << endl;
                    encontrado = buscar_lz78_limpio(nombreEnc, nombrePista);
                }

                if (!encontrado) {
                    cout << "\n❌ No se encontró ninguna combinación (RLE o LZ78)." << endl;
                }

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
*/

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

    // Calcular longitud total
    int total = 0;
    for (int i = 0; i < tamano; i += 3) {
        int len = (resultado[i] << 8) | resultado[i+1];
        total += len;
    }

    texto = new char[total + 1];
    int pos = 0;
    for (int i = 0; i < tamano; i += 3) {
        int len = (resultado[i] << 8) | resultado[i+1];
        char c = resultado[i+2];
        for (int j = 0; j < len; j++) {
            texto[pos++] = c;
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
                nueva[j] = dict[indice][j];  // ← ya no -1
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

bool buscar_lz78_limpio(const char* nombreEnc, const char* nombrePista) {
    int len_enc = 0, len_pista = 0;
    char* encriptado = leer_archivo_bn(nombreEnc, len_enc);
    char* pista_bruta = leer_archivo_bn(nombrePista, len_pista);

    if (!encriptado || !pista_bruta) {
        if (encriptado) delete[] encriptado;
        if (pista_bruta) delete[] pista_bruta;
        return false;
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

    cout << "Archivo encriptado: " << len_enc << " bytes" << endl;
    cout << "Pista: [" << pista << "]" << endl;

    bool encontrado = false;
    cout << "\n=== Intentando con LZ78 (función limpia) ===" << endl;

    for (int n = 1; n <= 7 && !encontrado; n++) {
        for (int K = 0; K <= 255 && !encontrado; K++) {
            char* desencriptado = desencriptar_bin(encriptado, len_enc, n, (unsigned char)K);

            // DEBUG: mostrar primeros 10 bloques (opcional, puedes comentar)
            if (n == 3 && K >= 85 && K <= 95) {
                cout << "n=" << n << " K=0x" << hex << K << dec << " -> Bloques: ";
                for (int i = 0; i < len_enc && i < 30; i += 3) {
                    int idx = (desencriptado[i] << 8) | (unsigned char)desencriptado[i + 1];
                    unsigned char c = (unsigned char)desencriptado[i + 2];
                    cout << "(" << idx << ",'" << (isprint(c) ? (char)c : '.') << "') ";
                }
                cout << endl;
            }

            // Intentar descompresión
            char* original_lz = descompresion_lz78((unsigned char*)desencriptado, len_enc);

            if (original_lz && contiene(original_lz, pista)) {
                cout << "\n✅ ENCONTRADO con LZ78" << endl;
                cout << "Rotación: " << n << " bits" << endl;
                cout << "Clave XOR: 0x" << hex << K << dec << endl;
                cout << "\nTexto original (primeros 500 chars):" << endl;
                for (int i = 0; i < 500 && original_lz[i] != '\0'; i++) cout << original_lz[i];
                cout << endl;
                encontrado = true;
            }

            if (original_lz) delete[] original_lz;
            delete[] desencriptado;
        }
    }

    // Limpiar memoria
    delete[] encriptado;
    delete[] pista_bruta;
    delete[] pista;

    return encontrado;
}

