# SolidSnake

SolidSnake est un projet de recherche en sécurité offensive. Ce projet combine des techniques avancées telles que l'injection de code, les hook de bibliothèques dynamiques (LD_PRELOAD), la persistance, la dissimulation, et un système de Command-and-Control (C2). Le projet est réalisé dans un cadre éducatif pour explorer et documenter des approches offensives.

---

## **Fonctionnalités**

### 1. **LD_PRELOAD Malware**
- Exploite `LD_PRELOAD` pour précharger une bibliothèque et hooker des fonctions critiques du système.
- Exemple : Exfiltrer des clés SSH via le hook des fonctions OpenSSL.
- Supporte :
  - Les utilisateurs standards.
  - Les utilisateurs root.
- Intègre un mécanisme de port-knocking pour une activation furtive.

### 2. **Command-and-Control (C2)**
- Implémente un serveur multi-threadé pour gérer plusieurs clients simultanément.
- Permet :
  - L'enregistrement des clients.
  - L'association de clés SSH aux clients.
- Utilise des connexions TCP basiques.

### 3. **Techniques de dissimulation**
- Trois options :
  - Cacher un fichier dans le système.
  - Cacher une connexion réseau (par exemple, la masquer dans `netstat`).
  - Supprimer ou bloquer les journaux pour masquer les traces.

### 4. **Persistance**
- Techniques pour rester actif après un redémarrage du système.
- Approches possibles :
  - Modification des registres sous Windows.
  - Ajout de services ou de tâches planifiées sous Linux.

---

## **Prérequis**
- Système d'exploitation : Linux.
- Outils nécessaires :
  - GCC pour la compilation.
  - Valgrind ou ThreadSanitizer pour la détection des erreurs.
  - Wireshark pour analyser les communications réseau.
- Une machine virtuelle (recommandé pour les tests).

---

## **Installation et Compilation**

### 1. **Clonez le dépôt** :
-```bash
   git clone https://github.com/Talah5/SolidSnake.git
   cd SolidSnake

### 2. **Compilez le projet** : 
    -'''bash
    make

### 3. **Exécutez le programme** : 
    -'''bash
    ./solid_snake

### 4. **Nettoyez les fichiers compilés (optionnel)** :
    '''bash
    make clean

## **Utilisation**

1. ### **Menu interactif** :
    - Le programme propose un menu pour tester les différentes fonctionnalités (LD_PRELOAD, C2, dissimulation, persistance).
2. ### **Commandes** :

- L'option 1 initialise les hooks LD_PRELOAD.
- L'option 2 configure le serveur C2.
- L'option 3 configure une technique de persistance.
- L'option 4 teste une méthode de dissimulation.

## **Structure du projet** 

SolidSnake/
├── src/
│   ├── main.c                       # Point d'entrée du programme
│   ├── ld_preload.c                 # Implémentation du malware LD_PRELOAD
│   ├── c2_server.c                  # Serveur C2 multi-threadé
│   ├── c2_client.c                  # Client pour le C2
│   ├── persistence.c                # Techniques de persistance
│   ├── dissimulation.c              # Techniques de dissimulation
│   ├── utils.c                      # Fonctions utilitaires
│   ├── headers/                     # Fichiers d'en-tête (.h)
│       ├── ld_preload.h
│       ├── c2.h
│       ├── persistence.h
│       ├── dissimulation.h
│       ├── utils.h
├── tests/
│   ├── test_ld_preload.c            # Tests pour LD_PRELOAD
│   ├── test_c2.c                    # Tests pour le C2
│   ├── test_persistence.c           # Tests pour la persistance
│   ├── test_dissimulation.c         # Tests pour la dissimulation
├── docs/
│   ├── ld_preload.md                # Documentation sur LD_PRELOAD
│   ├── threads.md                   # Documentation sur les threads
│   ├── persistence.md               # Documentation sur la persistance
│   ├── dissimulation.md             # Documentation sur la dissimulation
├── Makefile                         # Automatisation de la compilation
├── README.md                        # Présentation du projet
└── LICENSE                          # Licence du projet

## **Doucmentation**

    - LD_PRELOAD : docs/ld_preload.md
    - Threads : docs/threads.md
    - Persistance : docs/persistence.md
    - Dissimulation : docs/dissimulation.md

## **Avertissement**

Ce projet est uniquement destiné à un usage éducatif dans un cadre légal. Toute utilisation malveillante de ce code est strictement interdite.

## **Contributeurs**

- Talah5

## **Licence**

Ce projet est sous licence MIT. Consultez le fichier LICENSE pour plus de détails