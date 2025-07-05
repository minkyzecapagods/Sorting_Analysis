# Compilador
CXX := g++

# Flags de compilação
CXXFLAGS := -std=c++17 -Iinclude -Wall -O2

# Nome do executável
TARGET := sorting_tester

# Diretórios
SRC_DIR := src
BUILD_DIR := build

# Arquivos fonte
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Regra padrão
all: $(BUILD_DIR) $(TARGET)

# Cria o diretório build se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Linka os objetos para gerar o executável
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compila cada arquivo fonte para um objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean