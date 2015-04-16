# Compilador que será utilizado
CC=g++

# flags de compilación
CFLAGS=-g -Wall

# flags de linkeo
LDFLAGS=

# Agrear acá los directorios a incluir en la compilación
INCDIR=src/aed2 src/base src/modulos src/tests
INC_PARAMS=$(foreach d, $(INCDIR), -I $d)

# Agregar acá los archivos .cpp a compilar
SOURCES=src/base/test_catedra.cpp src/base/Driver.cpp src/base/ArbolSintactico.cpp src/modulos/conjString.cpp src/modulos/Mapa.cpp src/modulos/Ciudad.cpp src/modulos/RestriccionTP.cpp

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) $(INC_PARAMS) $(CFLAGS) $< -c -o $@