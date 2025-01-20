#!/bin/bash

# Nom du projet
PROJECT_NAME="LD_PRELOAD_Project"

# Créer les répertoires
echo "Création de la structure de projet..."
mkdir -p $PROJECT_NAME/{src,include,build,tests,docs}

# Créer les fichiers de base
echo "Création des fichiers source..."
touch $PROJECT_NAME/src/{intercept.c,exfiltration.c,utils.c}
touch $PROJECT_NAME/include/{intercept.h,exfiltration.h,utils.h}

echo "Création des fichiers pour tests..."
touch $PROJECT_NAME/tests/test_intercept.sh
chmod +x $PROJECT_NAME/tests/test_intercept.sh

echo "Création de la documentation..."
touch $PROJECT_NAME/docs/architecture.md
touch $PROJECT_NAME/README.md

echo "Création du Makefile..."
cat << 'EOF' > $PROJECT_NAME/Makefile
CC = gcc
CFLAGS = -shared -fPIC -Wall -Wextra -ldl
SRC_DIR = src
BUILD_DIR = build

all: $(BUILD_DIR)/intercept.so

$(BUILD_DIR)/intercept.so: $(SRC_DIR)/intercept.c $(SRC_DIR)/exfiltration.c $(SRC_DIR)/utils.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
EOF

# Ajouter un exemple de script de test
echo "Ajout d'un script de test..."
cat << 'EOF' > $PROJECT_NAME/tests/test_intercept.sh
#!/bin/bash
# Script pour tester LD_PRELOAD
echo "Test de LD_PRELOAD avec la bibliothèque intercept.so"
LD_PRELOAD=../build/intercept.so cat /etc/passwd
EOF

# Ajouter un contenu initial dans le README
echo "Ajout du contenu initial dans le README..."
cat << EOF > $PROJECT_NAME/README.md
# $PROJECT_NAME

## Description
Ce projet utilise \`LD_PRELOAD\` pour intercepter et modifier le comportement des fonctions standard en C.

## Structure du projet
- **src/** : Code source principal.
- **include/** : Fichiers d'en-tête pour le code source.
- **build/** : Contient les fichiers compilés.
- **tests/** : Scripts pour tester les fonctionnalités.
- **docs/** : Documentation technique.

## Compilation
Pour compiler le projet, utilisez la commande :
\`\`\`
make
\`\`\`

## Exécution
Pour tester la bibliothèque avec \`LD_PRELOAD\`, utilisez :
\`\`\`
LD_PRELOAD=./build/intercept.so <commande>
\`\`\`
EOF

echo "Structure de projet créée avec succès dans le dossier '$PROJECT_NAME'."
