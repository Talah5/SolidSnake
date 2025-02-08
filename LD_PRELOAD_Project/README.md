# **LD_PRELOAD_Project**

## **Description**
Ce projet utilise la technique de `LD_PRELOAD` pour intercepter et modifier le comportement de fonctions standard en C, telles que `getpwnam` et `crypt`, afin de collecter des identifiants et des informations système. Ces données sont ensuite exfiltrées vers un serveur Command & Control (C2) après une séquence de port knocking pour sécuriser l'accès. Le projet inclut également un mécanisme de masquage de fichiers et permet au serveur C2 de gérer plusieurs clients simultanément.


---

## **Structure du projet**
- **`client_malware/`** : Contient le code source du malware
  - `credentials.c` : Effectue un "port knocking" et exfiltre des informations locales et des identifiants vers un serveur distant.
  - `lib_hide_file.c` : Gestion de l'exfiltration des données vers le serveur C2
  - `Makefile`: Compilation du malware
  - **`include/`** : Fichiers d'en-tête pour le code source :
    - `credentials.h`: Header du malware
- **`server_C2/`** : Contient le code source du serveur C2
  - `c2_server.c` : Gère les connexions des malwares, écoute les knocks et traite les données exfiltrées
  - `Makefile`: Compilation du serveur
- **`docs/`** : Documentation technique (ex. : architecture, explications détaillées).

---

## **Fonctionnalités**
- Interception des fonctions standard comme `getpwnam` et `crypt` pour collecter des identifiants via `LD_PRELOAD`.
- Envoi des informations collectées au serveur C2 après une séquence de port knocking.
- Serveur C2 capable de gérer plusieurs clients simultanément après avoir validé la séquence de port knocking.
- Mécanisme de port knocking (sur les ports 1234, 2345, 3456) pour sécuriser l'accès et activer le serveur C2.
- Masquage du fichier `solidsnake` via `lib_hide_file.c` pour éviter sa détection.

---

## **Prérequis**
Avant de commencer, assurez-vous que les outils et bibliothèques suivants sont installés sur votre système :
- **GCC** (compilateur)
- **Make** (outil de gestion de la compilation)
- **libc6-dev** (pour les outils de développement de la bibliothèque standard)
- **libssl-dev** (bibliothèque nécessaire pour les manipulations liées à `crypt`)

Pour installer les dépendances nécessaires, exécutez :
```bash
sudo apt update
sudo apt-get install build-essential libc6-dev libssl-dev
```

---

## **Compilation**
Pour compiler le projet, exécutez la commande suivante dans le répertoire du projet :
```bash
make
```
### **Partie Client** 

Cela générera les fichiers suivants pour le client :

- **`build/libhide_file.so`**  : La bibliothèque partagée pour intercepter et masquer le fichier `solidsnake` via `LD_PRELOAD`.
- **`build/credentials.so`**  : La bibliothèque partagée pour intercepter les fonctions standard (`getpwnam`, `crypt`) et exfiltrer les données.

### **Partie Serveur**

Cela générera également le fichier suivant pour le serveur :

- **`build/c2_server`** : L'exécutable du serveur C2, qui gère les connexions des malwares et traite les données exfiltrées.

Pour nettoyer les fichiers compilés et le répertoire build :
```bash
make clean
```

---

## **Exécution**

### **1. Lancer le serveur C2**
Dans un terminal côté machine serveur, exécutez le serveur C2 pour écouter les connexions :
```bash
make run_c2
```

### **2. Stopper SSH côté malware**
Sur la machine infectée (malware), arrêtez le service SSH :
```bash
sudo systemctl stop ssh
```

### **3. Tester l'interception**
Dans un terminal côté malware, utilisez LD_PRELOAD pour intercepter les appels lors du démarrage de SSH :
```bash
sudo LD_PRELOAD=/chemin/vers/intercept.so /usr/sbin/sshd -D
```
Si une erreur s'affiche, créez directement le fichier nécessaire :

```bash
sudo mkdir -p /run/sshd
```
### **4. Lancer la connexion depuis une troisième machine**
Utilisez SSH pour vous connecter à l'adresse IP de votre machine infectée :

```bash
ssh user@<IP_de_votre_machine_infectée>
```
### **5. Vérifier le serveur C2**

Les données interceptées (comme les identifiants et informations système) doivent apparaître dans le terminal où tourne le serveur C2. Une fois les données interceptées, un fichier nommé `infected_hosts.txt` sera créé et contiendra toutes les informations récupérées.

### **6. LD_PRELOAD caché dans  pour le fichier solidsnake dans `/etc/ld.so.preload`**
Si tu veux que `LD_PRELOAD` soit chargé à chaque exécution de programme, pour cacher le fichier `solidsnake`, ajoute le chemin du `.so` dans `/etc/ld.so.preload` :
```bash
echo "/chemin/vers/intercept.so" | sudo tee -a /etc/ld.so.preload
```
**Avantage** :

  Appliqué à tout le système, même pour l'utilisateur root.

**ATTENTION** :

  Si le `.so` a une erreur, tout le système peut devenir inutilisable !

**Sécurise en gardant une sauvegarde** :
```bash
cp /etc/ld.so.preload /etc/ld.so.preload.bak
```
---

## **Contributeurs**
- **Talah5**

---

