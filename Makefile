# all: main
# 	gcc -o Escalonador main.c list_table.c schedule.c

CC = gcc

HEADERS = schedule.h list_table.h

OBJ = main.o schedule.o list_table.o

EXEC = Escalonador #define nome final de arquivo

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)
	del $(OBJ)
#rm -f $(OBJ)

main.o: main.c $(HEADERS)
	$(CC) -c main.c

schedule.o: schedule.c $(HEADERS)
	$(CC) -c schedule.c

list_table.o: list_table.c $(HEADERS)
	$(CC) -c list_table.c