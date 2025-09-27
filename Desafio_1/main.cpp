#include <iostream>
#include <fstream>

using namespace std;
void copiar_cadena(char* destino, char* origen);
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
                        cout << "Metodo: RLE" << endl;
                        cout << "Rotacion: " << n_rot << " bits" << endl;
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
                            cout << "Metodo: LZ78" << endl;
                            cout << "Rotacion: " << n_rot << " bits" << endl;
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
                cout << "No se pudo resolver el caso " << i << endl;
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


        //RLE
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
                        cout << "Rotacion: " << n << " bits" << endl;
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

        //Pasando a la funcion LZ78
        if (!encontrado) {
            cout << "\RLE no funciono, intentando LZ78 desde cero " << endl;
            encontrado = buscar_lz78_limpio(nombreEnc, nombrePista);
        }

        if (!encontrado) {
            cout << "No se encontro ninguna combinación (RLE o LZ78)." << endl;
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

//Funcion no necesaria solo fue para probar codigo
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
                cout << " ENCONTRADO con LZ78" << endl;
                cout << "Rotacion: " << n << " bits" << endl;
                cout << "Clave XOR: 0x" << hex << K << dec << endl;
                cout << "Texto original (primeros 500 chars):" << endl;
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

