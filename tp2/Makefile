CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
BIN_DIR=./bin


SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
EXEC=tp2



all: $(SRC) $(EXEC)
    
%.o:%.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(EXEC): $(OBJ) 
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ)


clean:	
	rm -rf $(OBJ)
