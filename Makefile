# Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Nome do executável
TARGET = compiler

# Diretório de arquivos fonte
SRCDIR = .

# Arquivos fonte (.cpp)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Arquivos objeto (.o)
OBJECTS = $(SOURCES:.cpp=.o)

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

# Limpar arquivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos compilados removidos"

# Recompilar tudo
rebuild: clean build

.PHONY: all build run run-file clean rebuild

