#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Definición de tipos básicos
typedef char DATA;
typedef struct elemento {
    DATA d;
    struct elemento *siguiente;
} ELEMENTO;

typedef struct {
    int cnt;
    ELEMENTO *tope;
} PILA;

// --- Funciones de Pila (Implementadas) ---

void inicializar(PILA *s) {
    s->cnt = 0;
    s->tope = NULL;
}

void push(PILA *s, DATA x) {
    ELEMENTO *nuevo = (ELEMENTO *)malloc(sizeof(ELEMENTO));
    if (nuevo == NULL) return; // Error de memoria
    nuevo->d = x;
    nuevo->siguiente = s->tope;
    s->tope = nuevo;
    s->cnt++;
}

DATA pop(PILA *s) {
    if (s->tope == NULL) return '\0';
    ELEMENTO *temp = s->tope;
    DATA dato = temp->d;
    s->tope = temp->siguiente;
    s->cnt--;
    free(temp);
    return dato;
}

int estavacia(PILA *s) {
    return (s->cnt == 0);
}

/**
 * TAREA PRINCIPAL: Determinar si la cadena es palíndromo.
 */
int esPalindromo(char cadena[]) {
    PILA original, invertida, auxiliar;
    inicializar(&original);
    inicializar(&invertida);
    inicializar(&auxiliar);

    int i, longitud = strlen(cadena);

    // 1. Filtrar letras y llenar la pila 'auxiliar'
    for (i = 0; i < longitud; i++) {
        if (isalpha(cadena[i])) {
            push(&auxiliar, tolower(cadena[i]));
        }
    }

    // 2. Al pasar de 'auxiliar' a 'original', el orden se invierte una vez.
    // Al pasar de 'original' a 'invertida', se invierte otra vez.
    // Esto nos permite tener una pila "al derecho" y otra "al revés".
    while (!estavacia(&auxiliar)) {
        DATA letra = pop(&auxiliar);
        push(&original, letra);  // Queda en orden normal
        push(&invertida, letra); // Usaremos una copia para invertirla después
    }

    // Para que 'invertida' realmente sea el reverso de 'original',
    // necesitamos que 'original' sea el orden de entrada.
    // Re-ajuste de lógica:
    // Pila A (original): letras en orden de entrada.
    // Pila B (invertida): letras en orden inverso.
    
    // Lo más simple: Comparamos la cadena original contra el POP de la pila
    int esPal = 1;
    int j = 0;
    for (i = 0; i < longitud; i++) {
        if (isalpha(cadena[i])) {
            char letraOriginal = tolower(cadena[i]);
            char letraInvertida = pop(&original); // El pop da el último que entró (reverso)
            
            if (letraOriginal != letraInvertida) {
                esPal = 0;
                break;
            }
        }
    }

    // Limpiar pilas si quedaron elementos
    while(!estavacia(&original)) pop(&original);

    return esPal;
}

int main() {
    char prueba1[] = "Anita lava la tina";
    char prueba2[] = "Estructuras de Datos";

    printf("--- TEST DE PALINDROMOS ---\n");
    
    printf("Prueba 1: '%s' -> %s\n", prueba1, 
           esPalindromo(prueba1) ? "ES PALINDROMO" : "NO ES PALINDROMO");
           
    printf("Prueba 2: '%s' -> %s\n", prueba2, 
           esPalindromo(prueba2) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    return 0;
}
