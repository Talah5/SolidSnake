## Threads sous Linux : fonctionnement et gestion
Un **thread** est un "sous-processus léger" qui partage la mémoire avec son parent. Contrairement à un processus, un thread :
- **Partage la mémoire avec les autres threads du programme.**
- **A son propre contexte d’exécution (registre, pile, etc.).**
- **S’exécute en parallèle des autres threads.**

### Comment fonctionne un thread sous Linux ?
Linux utilise **`pthread`** (POSIX Threads) pour gérer les threads.

### Création d’un thread simple :
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_function(void *arg) {
    printf("Thread exécuté avec l'argument : %s\n", (char *)arg);
    return NULL;
}

int main() {
    pthread_t thread;
    char *message = "Hello from thread";
    
    if (pthread_create(&thread, NULL, thread_function, (void *)message) != 0) {
        perror("Erreur lors de la création du thread");
        return 1;
    }

    pthread_join(thread, NULL);
    printf("Thread terminé.\n");
    return 0;
}
```

### Synchronisation avec les mutex
Quand plusieurs threads accèdent à une même variable, **des conflits peuvent apparaître**. Pour éviter ça, on utilise des **mutex** (verrous) :
```c
pthread_mutex_t lock;

void *safe_function(void *arg) {
    pthread_mutex_lock(&lock);  // Verrouillage
    printf("Thread sécurisé : %s\n", (char *)arg);
    pthread_mutex_unlock(&lock);  // Déverrouillage
    return NULL;
}
```