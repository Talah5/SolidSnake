## Le fonctionnement d'un Linker sous Linux

Un **linker** (ou éditeur de liens) est un programme qui combine plusieurs fichiers objets (`.o`) en un **exécutable** ou une **bibliothèque partagée (`.so`)**. Il s'occupe de :

- **Résolution des symboles** (assure que toutes les fonctions/variables appelées existent).
- **Assemblage du code** en un fichier exécutable final.
- **Gestion des dépendances dynamiques** (permet d’utiliser des bibliothèques partagées au lieu d’inclure tout le code dans l’exécutable).

### Types de Linking

#### ** 1. Linking Statique (`.a` - Archives)**
- Le code des bibliothèques est **copié dans l'exécutable**.
- **Avantage** : Pas besoin des `.so` au runtime.
- **Inconvénient** : Taille plus grande, impossible de mettre à jour la lib sans recompiler.
```bash
gcc -o mon_programme mon_code.c /usr/lib/libm.a
```

#### ** 2. Linking Dynamique (`.so` - Shared Object)**
- Le programme charge **dynamiquement** les bibliothèques au runtime.
- **Avantage** : Plus léger, permet des mises à jour sans recompilation.
- **Inconvénient** : Dépendance aux fichiers `.so` à l'exécution.
```bash
gcc -o mon_programme mon_code.c -lm  # Utilisation de libm.so
```

### Comment le Linker résout les dépendances ?
Quand on exécute un programme, **le loader (`ld.so`) cherche les bibliothèques dynamiques** via :
- **`/lib/` et `/usr/lib/`**
- **`LD_LIBRARY_PATH`** (variable d’environnement pour modifier le chemin des libs)
- **`/etc/ld.so.conf`** (fichier de configuration des bibliothèques)

Commande utile pour voir les dépendances :
```bash
ldd mon_programme
```
### Exemple de Makefile pour une compilation avec une bibliothèque dynamique

```makefile
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -L/usr/lib -lm  # Lien avec la bibliothèque mathématique dynamique

all: prog

prog: main.o
	$(CC) $(CFLAGS) -o prog main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f prog main.o
```
