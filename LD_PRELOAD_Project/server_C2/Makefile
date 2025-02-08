# Variables de compilation
CC = gcc
CFLAGS = -Wall -Wextra -pthread
BUILD_DIR = build

# Nom de l'exécutable du serveur C2
C2_SERVER = $(BUILD_DIR)/c2_server

# Fichier source du serveur
SRC_FILES = c2_server.c

# Cible principale
all: $(C2_SERVER)

# Compilation du serveur C2
$(C2_SERVER): $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Nettoyage
clean:
	rm -rf $(BUILD_DIR)

# Exécuter le serveur C2
run:
	@echo "Lancement du serveur C2..."
	@$(C2_SERVER)

.PHONY: all clean run
