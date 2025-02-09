## `LD_PRELOAD` : Injection de bibliothèque en mémoire
### Qu'est-ce que `LD_PRELOAD` ?
`LD_PRELOAD` est une variable d’environnement permettant **d’injecter une bibliothèque (`.so`) avant toutes les autres**. Elle est utilisée pour :
- **Rediriger des appels système** (`open()`, `read()`, `execve()`, etc.).
- **Intercepter des fonctions sans modifier l’exécutable** (hooking dynamique).
- **Créer des rootkits ou cacher des fichiers (comme `libhide.so`)**.

### Comment ça marche ?
Quand un programme est lancé :
1. **Le loader (`ld.so`) charge d’abord les bibliothèques indiquées dans `LD_PRELOAD`.**
2. **Si une fonction du programme a le même nom qu’une fonction dans `LD_PRELOAD`, celle de `LD_PRELOAD` est exécutée en premier !**
3. **Le programme ne voit que la version modifiée des fonctions !**

### Exemple d’interception de `open()`
On crée une bibliothèque qui **bloque l’accès aux fichiers contenant "secrets"** :

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <fcntl.h>

int open(const char *pathname, int flags, ...) {
    static int (*orig_open)(const char *, int, ...) = NULL;
    if (!orig_open) orig_open = dlsym(RTLD_NEXT, "open");

    if (strstr(pathname, "secrets")) {
        printf("Accès bloqué à : %s\n", pathname);
        return -1;
    }
    return orig_open(pathname, flags);
}
```
**Compilation :**
```bash
gcc -shared -o block_open.so -fPIC block_open.c -ldl
```
**Test :**
```bash
LD_PRELOAD=./block_open.so cat secrets.txt  # Bloqué !
```

### Exemple d’interception de `execve()` pour bloquer l'exécution de certaines commandes

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

int execve(const char *filename, char *const argv[], char *const envp[]) {
    static int (*orig_execve)(const char *, char *const[], char *const[]) = NULL;
    if (!orig_execve) orig_execve = dlsym(RTLD_NEXT, "execve");

    if (strstr(filename, "nano") || strstr(filename, "vim")) {
        printf("Exécution de %s bloquée !\n", filename);
        return -1;
    }
    return orig_execve(filename, argv, envp);
}
```

**Compilation et test :**
```bash
gcc -shared -o block_exec.so -fPIC block_exec.c -ldl
LD_PRELOAD=./block_exec.so nano  # Doit être bloqué
```
