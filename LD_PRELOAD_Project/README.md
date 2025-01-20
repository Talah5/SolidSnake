# LD_PRELOAD_Project

## Description
Ce projet utilise `LD_PRELOAD` pour intercepter et modifier le comportement des fonctions standard en C.

## Structure du projet
- **src/** : Code source principal.
- **include/** : Fichiers d'en-tête pour le code source.
- **build/** : Contient les fichiers compilés.
- **tests/** : Scripts pour tester les fonctionnalités.
- **docs/** : Documentation technique.

## Compilation
Pour compiler le projet, utilisez la commande :
```
make
```

## Exécution
Pour tester la bibliothèque avec `LD_PRELOAD`, utilisez :
```
LD_PRELOAD=./build/intercept.so <commande>
```
