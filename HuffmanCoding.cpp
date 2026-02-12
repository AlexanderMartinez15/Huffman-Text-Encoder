//Alexander Martinez Cruz
// DR. Juan O. Lopez Gerena
// CCOM 4006-M10
// huffman_code Proyect el siguiente proyecto  recibe un documento txt
// copia su data verifica su frecuencias crea el arbol de huffman 
//con esa data conseguimos su ecndoing cuanto requiere el original y la diferencia

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <functional> 

using namespace std;

// Definimos los nodos
struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;
};

// struct para comparacion
//Este operador de comparacion coloca los nodos delHuffman tree con las frecuencias mas bajas antes en el priority quuee

 struct comparar
 {
    bool operator()(Node* left, Node* right) const {
        return left->frequency > right->frequency;
    }
};

//declaracion de funciones en bloque
string load_data(const string& archivo_a_leer);
unordered_map<char, int> compute_fd(const string& data);
Node* huffman_tree(const unordered_map<char, int>& fd);
unordered_map<char, string> huffman_code(Node* root);
string encode(const unordered_map<char, string>& code, const string& data);
void procesar_resultados(const unordered_map<char, int>& fd, const unordered_map<char, string>& code, const string& data, const string& encoded);

int main() {
    //aqui podemos cambir mas facilemnte  el input
    //llamamos las fucniones para presentarlas
     string archivo_a_leer = "input1.txt";
    
    string data = load_data(archivo_a_leer);
    
     unordered_map<char, int> fd = compute_fd(data);
      Node* root = huffman_tree(fd);
     unordered_map<char, string> code = huffman_code(root);
      string encoded = encode(code, data);
    procesar_resultados(fd, code, data, encoded);

   

    return 0;
}

// cargamos la data y si no se carga produce error 
    string load_data(const string& archivo_a_leer) {
    ifstream file(archivo_a_leer);
    string data;
        //si el archivo esta arbierto  tomamos la data y lo cerramos
    if (file.is_open()) {
        getline(file, data);
        file.close();
    } else {
        cout << "Error: no se encontro archivo :(( trate de nuevo" << endl;
    }

    return data;
}

// Implementacion del hash table almacena cada caracter como un key
//su frecuencia como el valor correspondiente. Este enfoque rastrea eficazmente la frecuencia de cada caracter en el dato
unordered_map<char, int> compute_fd(const string& data) {
    unordered_map<char, int> fd;

    for (char c : data) {
        if (isalpha(c)) {  // verificaa si el caracter es una letra gracias a la funcion "isalpha" nos facilita el trabajo
            //de ser asi incrementa el contador de frecuencia para ese caracteren especifico
            fd[c]++;
        }
    }
    //regresa fd 
    return fd;    
}

// huffman_tree
     Node* huffman_tree(const unordered_map<char, int>& fd) {
    priority_queue<Node*, vector<Node*>, comparar> pq;
    //el siguiente ciclo:
    // recorre cada par de key-value en el mapa fd Para cada par, crea un nuevo nodo y asigna key (pair.first)
    // como datos del nodo y el valor (pair.second) como la frecuencia del nodo
    //indentamos los nodos left y right como null
    // se añade a un priority priority_queue
    //en resumen construye los nodos
    for (const auto& pair : fd) {
        Node* newNode = new Node;
        newNode->data = pair.first;
        newNode->frequency = pair.second;
         newNode->left = nullptr;
        newNode->right = nullptr;
        pq.push(newNode);
    }
    //formacion del arbol
    // Se elimina el el nodo de menor frecuencia y se pone a la izquierda
    // Elimina el siguiente menor y lo pone en la derecha
    // crea nodo parent
    // Asigna la frecuencia del nodo parent como la suma deleft y right
    // Establece left como el hijo izquierdo y right como el hijo derecho del nodo parent
    //agregar parent de nuevo 
    //  asi sucesivamente al finalizar el nodo que queda  en top sera la raiz
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node;
        parent->data = '\0';
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top();
}

// Implementacion del hoffman
    unordered_map<char, string> huffman_code(Node* root) {
     unordered_map<char, string> code;
     string currentCode;

     // Funcion para correr por el arbol y codificarlo
    // si el nodo actual es una hoja le asigna el codigo binario actual al caracter y lo devuelve
    //Si no es una hoja, llame a la funcion para el hijo izquierdo con 0 y 
    //luego para el hijo derecho con  1 lagregado al codigo actual
    // se repite hasta  que todos tengan su codigo binario 
    // regresa el map
    function<void(Node*, string)> generar_codigo = [&](Node* node, string codigo_hasta_ahora) {
        if (node->left == nullptr && node->right == nullptr) {
            code[node->data] = codigo_hasta_ahora;
         return;
        }

        generar_codigo(node->left, codigo_hasta_ahora + "0");
        generar_codigo(node->right, codigo_hasta_ahora + "1");
    };

    generar_codigo(root, currentCode);
     return code;
}

                                // encode
    // Se utiliza un mapa para asignar codigos Huffman a los caracteres y la cadena de datos que se desea codificar
//busca su codigo en el mapa code, y si lo encuentra, agrega ese codigo al resultado codificado
// Si encuentra su codigo en el mapa de codigo, agregue el codigo al resultado codificado
// si no tiene se produce error
    string encode(const unordered_map<char, string>& code, const string& data) {
    string encoded;

    for (char c : data) {
        if (code.find(c) != code.end()) {
            encoded += code.at(c);
        } else {
            cout << "Error: No se encontro huffman code para el caracter " << c  << endl;
        }
    }

    return encoded;
}

// procesamiento de resultados
// Print la tabla dr frecuencia, string orignal, encoded , bytes del prognal, econde y difrencia
void procesar_resultados(const unordered_map<char, int>& fd, const unordered_map<char, string>& code, const string& data, const string& encoded) {
    
    cout << "Simbolo\tFrecuencia\tCuenta\tCodigo\n";
    for (const auto& pair : fd) {
        cout << pair.first << "\t" << pair.second << "\t\t" << fd.at(pair.first) << "\t" << code.at(pair.first) << endl;
    }
    cout << endl;

    
        //presenta resultados 
        cout << "String original:\n" << data << endl << endl;
    cout << " String codificado:\n" << encoded << endl << endl;

     // Calculamos y    presentamos resultados
     int originalBytes = data.length() * 1;
    int encodedBytes = encoded.length();
    double savingsPercentage = ((originalBytes - encodedBytes) / (double)originalBytes) * 100.0;

    cout << "El string original requiere: " << originalBytes << " bytes" << endl;
    cout << "El string codificado require: " << encodedBytes << " bytes" << endl;
    cout << "La diferencia de espacio requeroda es:" << savingsPercentage << "%" << endl;
}
//nota full screen para resultados  completos
