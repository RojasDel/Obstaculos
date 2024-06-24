#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Celda {
    int x, y;
};

void mostrarLaberinto(char** laberinto, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            cout << laberinto[i][j] << ' ';
        }
        cout << endl;
    }
}

bool esValida(int x, int y, int filas, int columnas) {
    return x >= 0 && x < filas && y >= 0 && y < columnas;
}

// Función generadora de números aleatorios
int miRand(int i) { return rand() % i; }

void generarLaberinto(char** laberinto, int filas, int columnas, int jugadorX, int jugadorY, int salidaX, int salidaY) {
    queue<Celda> cola;
    cola.push((Celda){jugadorX, jugadorY});

    bool** visitado = new bool*[filas];
    for (int i = 0; i < filas; ++i) {
        visitado[i] = new bool[columnas];
        for (int j = 0; j < columnas; ++j) {
            visitado[i][j] = false;
        }
    }
    visitado[jugadorX][jugadorY] = true;

    // Direcciones de movimiento: derecha, izquierda, abajo, arriba
    int movX[] = {0, 0, 1, -1};
    int movY[] = {1, -1, 0, 0};

    while (!cola.empty()) {
        Celda actual = cola.front();
        cola.pop();

        // Desordenar los movimientos para generar caminos aleatorios
        int direcciones[] = {0, 1, 2, 3};
        random_shuffle(direcciones, direcciones + 4, miRand);

        for (int i = 0; i < 4; i++) {
            int nuevoX = actual.x + movX[direcciones[i]];
            int nuevoY = actual.y + movY[direcciones[i]];

            if (esValida(nuevoX, nuevoY, filas, columnas) && !visitado[nuevoX][nuevoY]) {
                visitado[nuevoX][nuevoY] = true;
                laberinto[nuevoX][nuevoY] = ' ';
                cola.push((Celda){nuevoX, nuevoY});
            }
        }
    }

    // Aseguramos que haya un camino claro desde P hasta S
    laberinto[jugadorX][jugadorY] = 'P';
    laberinto[salidaX][salidaY] = 'S';

    // Añadir obstáculos aleatorios
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (laberinto[i][j] == ' ' && rand() % 4 == 0) { // Aproximadamente 25% de celdas libres se convierten en obstáculos
                laberinto[i][j] = '#';
            }
        }
    }

    // Mantener el camino despejado
    laberinto[jugadorX][jugadorY] = 'P';
    laberinto[salidaX][salidaY] = 'S';

    // Liberar memoria
    for (int i = 0; i < filas; ++i) {
        delete[] visitado[i];
    }
    delete[] visitado;
}

bool resolverLaberinto(char** laberinto, int x, int y, int filas, int columnas, bool** visitado) {
    // Si alcanzamos la salida, hemos encontrado una solución
    if (laberinto[x][y] == 'S') {
        return true;
    }

    // Marcar la posición actual como visitada
    visitado[x][y] = true;

    // Movimientos posibles: derecha, izquierda, abajo, arriba
    int movX[] = {0, 0, 1, -1};
    int movY[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int nuevoX = x + movX[i];
        int nuevoY = y + movY[i];

        // Verificar si la nueva posición es válida
        if (esValida(nuevoX, nuevoY, filas, columnas) && laberinto[nuevoX][nuevoY] != '#' && !visitado[nuevoX][nuevoY]) {
            if (resolverLaberinto(laberinto, nuevoX, nuevoY, filas, columnas, visitado)) {
                laberinto[nuevoX][nuevoY] = '.'; // Marcar el camino correcto
                return true;
            }
        }
    }

    // Retroceso: desmarcar la posición actual como visitada
    visitado[x][y] = false;
    return false;
}

int main() {
    srand(time(0));

    int filas, columnas;
    cout << "Ingresa el numero de filas: ";
    cin >> filas;
    cout << "Ingresa el numero de columnas: ";
    cin >> columnas;

    // Crear el laberinto
    char** laberinto = new char*[filas];
    for (int i = 0; i < filas; ++i) {
        laberinto[i] = new char[columnas];
        for (int j = 0; j < columnas; ++j) {
            laberinto[i][j] = '#'; // Inicializar todas las celdas como obstáculos
        }
    }

    int jugadorX = 1, jugadorY = 1;
    int salidaX = filas - 2, salidaY = columnas - 2;

    generarLaberinto(laberinto, filas, columnas, jugadorX, jugadorY, salidaX, salidaY);

    bool** visitado = new bool*[filas];
    for (int i = 0; i < filas; ++i) {
        visitado[i] = new bool[columnas];
        for (int j = 0; j < columnas; ++j) {
            visitado[i][j] = false;
        }
    }

    resolverLaberinto(laberinto, jugadorX, jugadorY, filas, columnas, visitado);

    int jugadorPosX = jugadorX;
    int jugadorPosY = jugadorY;
    char movimiento;
    cout << "Usa W (arriba), A (izquierda), S (abajo), D (derecha) para moverte. Presiona 'q' para salir." << endl;

    while (true) {
        mostrarLaberinto(laberinto, filas, columnas);
        cout << "Movimiento: ";
        cin >> movimiento;

        if (movimiento == 'q') break;

        int nuevoX = jugadorPosX;
        int nuevoY = jugadorPosY;

        if (movimiento == 'w') nuevoX--;
        else if (movimiento == 's') nuevoX++;
        else if (movimiento == 'a') nuevoY--;
        else if (movimiento == 'd') nuevoY++;

        if (esValida(nuevoX, nuevoY, filas, columnas) && laberinto[nuevoX][nuevoY] != '#') {
            jugadorPosX = nuevoX;
            jugadorPosY = nuevoY;
            laberinto[jugadorPosX][jugadorPosY] = 'P'; // Posición actual del jugador
            laberinto[jugadorX][jugadorY] = '.'; // Marcar las posiciones anteriores como parte del camino recorrido
            jugadorX = nuevoX;
            jugadorY = nuevoY;
        } else {
            cout << "Movimiento invalido!" << endl;
        }

        if (jugadorPosX == salidaX && jugadorPosY == salidaY) {
            cout << "¡Has llegado a la salida!" << endl;
            break;
        }
    }

    // Liberar memoria utilizada por el laberinto y la matriz visitado
    for (int i = 0; i < filas; ++i) {
        delete[] laberinto[i];
        delete[] visitado[i];
    }
    delete[] laberinto;
    delete[] visitado;

    return 0;
}

