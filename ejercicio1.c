#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define EMPTY 0
#define FULL 10000

// --- DEFINICIONES DE TIPOS ---

typedef enum {FALSO, VERDADERO} BOOLEAN;
// Nota: En C estándar se suelen usar TRUE/FALSE, aquí los igualamos para compatibilidad
#define TRUE VERDADERO
#define FALSE FALSO

struct dato {
    enum {OPERADOR, VALOR} tipo;
    union {
        char op;
        int val;
    } u;
};
typedef struct dato DATO;

struct elemento {
    DATO d; // Cambiado de 'dato' a 'd' para coincidir con la estructura original
    struct elemento *siguiente;
};
typedef struct elemento ELEMENTO;

struct pila {
    int cnt;
    ELEMENTO *tope;
};
typedef struct pila PILA;

// --- PROTOTIPOS ---
void inicializar(PILA *stk);
void push(PILA *stk, DATO x);
DATO pop(PILA *stk);
BOOLEAN estavacia(PILA *stk);
BOOLEAN estallena(PILA *stk);
void rellenar(PILA *stk, const char *str);
int evaluar(PILA *polaca);

// --- IMPLEMENTACIÓN DE PRIMITIVAS ---

void inicializar(PILA *stk) {
    stk->cnt = 0;
    stk->tope = NULL;
}

void push(PILA *stk, DATO x) {
    ELEMENTO *nuevo = (ELEMENTO *)malloc(sizeof(ELEMENTO));
    if (nuevo == NULL) {
        fprintf(stderr, "ERROR: Heap Overflow\n");
        exit(1);
    }
    nuevo->d = x; // Usamos 'd' como definiste en struct elemento
    nuevo->siguiente = stk->tope;
    stk->tope = nuevo;
    stk->cnt++;
}

DATO pop(PILA *stk) {
    if (estavacia(stk)) {
        fprintf(stderr, "ERROR: Stack Underflow\n");
        exit(1);
    }
    DATO ret;
    ELEMENTO *temp;
    
    temp = stk->tope;
    ret = temp->d;
    stk->tope = temp->siguiente;
    stk->cnt--;
    free(temp);
    return ret;
}

BOOLEAN estavacia(PILA *stk) {
    return (stk->cnt == 0) ? TRUE : FALSE;
}

BOOLEAN estallena(PILA *stk) {
    return (stk->cnt == FULL) ? TRUE : FALSE;
}

// --- LÓGICA DE PROCESAMIENTO ---



void rellenar(PILA *stk, const char *str) {
    const char *p = str;
    DATO d;
    PILA aux;

    inicializar(stk);
    inicializar(&aux);

    while (*p != '\0') {
        while (isspace(*p) || *p == ',') p++;
        if (*p == '\0') break;

        // Verificamos si es un operador (+, -, *)
        // Nota: El '-' puede ser operador o inicio de número negativo, 
        // simplificamos asumiendo que después de un operador hay espacio.
        if ((*p == '+' || *p == '-' || *p == '*') && (isspace(*(p+1)) || *(p+1) == '\0')) {
            d.tipo = OPERADOR;
            d.u.op = *p;
            p++;
        } else {
            d.tipo = VALOR;
            sscanf(p, "%d", &d.u.val);
            while (*p != '\0' && !isspace(*p) && *p != ',') p++;
        }
        push(&aux, d);
    }

    // Invertimos para que el primer elemento de la cadena sea el primero en salir (LIFO)
    while (!estavacia(&aux)) {
        push(stk, pop(&aux));
    }
}

int evaluar(PILA *polaca) {
    DATO d, d1, d2;
    PILA evaluacion;
    inicializar(&evaluacion);

    while (!estavacia(polaca)) {
        d = pop(polaca);
        if (d.tipo == VALOR) {
            push(&evaluacion, d);
        } else {
            // Los operadores en RPN actúan sobre los dos últimos valores
            d2 = pop(&evaluacion);
            d1 = pop(&evaluacion);
            
            DATO resultado;
            resultado.tipo = VALOR;
            switch (d.u.op) {
                case '+': resultado.u.val = d1.u.val + d2.u.val; break;
                case '-': resultado.u.val = d1.u.val - d2.u.val; break;
                case '*': resultado.u.val = d1.u.val * d2.u.val; break;
            }
            push(&evaluacion, resultado);
        }
    }
    return pop(&evaluacion).u.val;
}

// --- MAIN ---
int main(void) {
    char str[] = "3 4 5 + *"; // (4 + 5) * 3 = 27
    PILA polaca;

    printf("--- CALCULADORA RPN ---\n");
    rellenar(&polaca, str);
    
    printf("Expresion: %s\n", str);
    printf("Resultado esperado: 27\n");
    printf("Resultado obtenido: %d\n", evaluar(&polaca));

    return 0;
}
