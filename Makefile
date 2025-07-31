# === VARIABILI DI CONFIGURAZIONE ===
# Il file eseguibile CustomShell
TARGET = $(BIN_DIR)/CustomShell

# Cartelle del progetto
BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include

#I file che verranno compilati nell'eseguibile
CS_ITEMS = \
	$(BUILD_DIR)/parser.tab.c \
	$(BUILD_DIR)/lexer.yy.c \
	$(BUILD_DIR)/command.o \
	$(BUILD_DIR)/executor.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/gtkApp.o

#Le cartelle in cui cercare gli header per la compilazione
INCLUDE_LIST = \
	-I$(INCLUDE_DIR) \
	-I$(INCLUDE_DIR)/shell \
	-I$(INCLUDE_DIR)/gui \
	-I$(BUILD_DIR)

# Impostazioni del compilatore
CC = gcc
CFLAGS = -Wall $(INCLUDE_LIST)

# === REGOLA DI DEFAULT ===
# Questa regola:
# 	crea le cartelle /bin e /build 
#	crea gli oggetti necessari alla compilazione
#	genera l'eseguibile linkando gli oggetti creati
all: crea_dir crea_items $(TARGET)

# === CREA_DIR: CREA LE CARTELLE /bin E /build ===
crea_dir:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)
	@echo "Creazione Directories..."

# === CREA_ITEMS: CREA GLI OGGETTI NECESSARI ALLA COMPILAZIONE ===
crea_items: $(CS_ITEMS)
	@echo "Oggetti necessari creati!"

# === BIN/CUSTOM_SHELL: CREA L'ESEGUIBILE VERO E PROPRIO ===
# L'opzione -lfl informa il linker di collegare la libreria runtime flex dentro l'eseguibile (necessario per Flex)
# Come in fase di creazione di "gtkApp.o", anche nella fase di linking bisogna informare il compilatore di
# includere le librerie necessarie per creare l'eseguibile (quindi `pkg-config --libs gtk4`).
$(TARGET): crea_items
	$(CC) $(CFLAGS) -o $@ $(CS_ITEMS) -lfl `pkg-config --libs gtk4`
	@echo "Build andata a buon fine, usa il comando [make run] per testarla!"

# === REGOLE PER I SINGOLI OGGETTI ===
# Per la creazione del Parser
$(BUILD_DIR)/parser.tab.c: src/shell/parser.y #include/shell/command.h
	@bison -d -o $@ $<	#-d specifica di creare anche l'header parser.tab.h

# Per la creazione del Lexer
$(BUILD_DIR)/lexer.yy.c: src/shell/lexer.l
	@flex -o $@ $<

# Per la creazione degli oggetti associati ai file in src/shell
$(BUILD_DIR)/%.o: src/shell/%.c #include/shell/command.h
	@$(CC) $(CFLAGS) -c -o $@ $<

# Per la creazione della GUI a partire dai file in src/gui
# La regola di compilazione è come quella trovata nella doc:
#	"compiling GTK applications on UNIX"
$(BUILD_DIR)/gtkApp.o: src/gui/gtkApp.c
	@$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -c -o $@ $< `pkg-config --libs gtk4`

# Per la creazione dell'oggetto main
$(BUILD_DIR)/%.o: src/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

# === REGOLE MISC ===
# Cancella la Build
del:
	@rm -rf $(BIN_DIR) $(BUILD_DIR)
	@echo "Eliminazione Build andata a buon fine..."

# Esegui il programma creato
run:
	@echo "Avvio CustomShell..."
	@./$(TARGET)
	@echo "Il programma ha terminato con successo!"

# Cancella la Build e Ricreala
test: del
	@echo "Avvio test!"
	@make
	@echo "Fine test!"

# Questa riga assicura che non vi siano conflitti con 
# file chiamati "all", "crea_dir", ...
.PHONY: all crea_dir crea_items del run test
