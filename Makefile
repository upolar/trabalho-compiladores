# Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Nome do executável
TARGET = compiler

# Diretório de arquivos fonte
SRCDIR = .

# Arquivos fonte (.cpp)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Remove a.cpp do build pois é um arquivo de entrada de teste
SOURCES := $(filter-out $(SRCDIR)/a.cpp, $(SOURCES))

# Arquivos objeto (.o)
OBJECTS = $(SOURCES:.cpp=.o)

# Arquivo de entrada padrão para teste
TEST_FILE = a.cpp

# Target padrão
all: build
	
# Compilar o projeto
build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Compilação concluída: $(TARGET)"

# Compilar arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Executar o programa com arquivo padrão
run: $(TARGET)
	./$(TARGET) $(TEST_FILE)

# Executar com arquivo específico (uso: make run-file FILE=nome_arquivo)
run-file: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Erro: Especifique o arquivo com FILE=nome_arquivo"; \
		exit 1; \
	fi
	./$(TARGET) $(FILE)

# Limpar arquivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos compilados removidos"

# Recompilar tudo
rebuild: clean build

.PHONY: all build run run-file clean rebuild

