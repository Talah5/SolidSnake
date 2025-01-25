# **LD_PRELOAD_Project**

## **Description**
Ce projet utilise la technique de `LD_PRELOAD` pour intercepter et modifier le comportement des fonctions standard en C. Il permet d'exfiltrer des données interceptées vers un serveur Command & Control (C2) et propose également des mécanismes supplémentaires, comme le port knocking.

---

## **Structure du projet**
- **`src/`** : Contient le code source principal :
  - `intercept.c` : Interception des fonctions `SSL_read` et `SSL_write`.
  - `exfiltration.c` : Gestion de l'exfiltration des données vers le serveur C2.
  - `c2_server.c` : Serveur C2 pour recevoir les données interceptées.
  - `port_knocking.c` : Implémentation du port knocking.
  - `utils.c` : Fonctions utilitaires.
- **`include/`** : Fichiers d'en-tête pour le code source.
- **`build/`** : Contient les fichiers compilés (`.o`, `.so`, exécutables).
- **`tests/`** : Scripts et programmes pour tester les différentes fonctionnalités.
- **`docs/`** : Documentation technique (ex. : architecture, explications détaillées).

---

## **Fonctionnalités**
- Interception des fonctions standard (`SSL_read`, `SSL_write`) via `LD_PRELOAD`.
- Envoi des données interceptées à un serveur C2 via un socket TCP.
- Serveur C2 capable de gérer plusieurs clients simultanément.
- Mécanisme de port knocking pour sécuriser l'accès.

---

## **Prérequis**
Avant de commencer, assurez-vous que les outils suivants sont installés sur votre système :
- **GCC** (compilateur)
- **OpenSSL** (bibliothèque nécessaire pour intercepter `SSL_read` et `SSL_write`)
- **Make**

Pour installer OpenSSL :
```bash
sudo apt update
sudo apt install libssl-dev
```

---

## **Compilation**
Pour compiler le projet, exécutez la commande suivante dans le répertoire du projet :
```bash
make
```

Cela génère :
- **`build/intercept.so`** : La bibliothèque partagée pour `LD_PRELOAD`.
- **`build/c2_server`** : L'exécutable du serveur C2.

Pour nettoyer les fichiers compilés :
```bash
make clean
```

---

## **Exécution**

### **1. Lancer le serveur C2**
Dans un terminal, exécutez le serveur C2 pour écouter les connexions :
```bash
./build/c2_server
```

### **2. Tester l'interception**
Dans un autre terminal, utilisez `LD_PRELOAD` pour intercepter les appels :
```bash
LD_PRELOAD=./build/intercept.so curl -v https://example.com
```

### **3. Vérifier le serveur**
Les données interceptées doivent apparaître dans le terminal où tourne le serveur C2.

---

## **Tests**
Des tests automatisés sont disponibles dans le répertoire `tests/`. Voici quelques exemples :
- **Test de la bibliothèque `intercept.so`** :
  ```bash
  LD_PRELOAD=./build/intercept.so curl https://example.com
  ```

- **Test unitaire de l'exfiltration** :
  Compilez et exécutez :
  ```bash
  make
  ./build/test_exfiltration
  ```


---

## **Contributeurs**
- **Talah5**

---

## **Licence**
Ajoute ici la licence de ton projet (ex. : MIT, GPL).

