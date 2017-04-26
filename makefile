##################################################
#
#	Makefile
#
#	--
#	INF01147 - Compiladores
#	Khin Baptista
#	Lorenzo Dal'Aqua
#
##################################################

CC	= gcc
LINKER	= gcc

CFLAGS	= -std=c99 -Wall -Wno-implicit-function-declaration -Wno-unused-function
LDFLAGS	=

##################################################

# Name of the project (executable binary)
Project = etapa2

# Source files names
SourceFiles = y.tab.c lex.yy.c hash.c lang171.c main.c

##################################################

CPP = $(SourceFiles)
OBJ = $(CPP:%.c=%.o)
DEP = $(OBJ:%.o=%.d)

##################################################

.PHONY: all clean

all: $(Project)

$(Project): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(DEP)
%.o: %.c
	$(CC) -MMD -c -o $@ $< $(CFLAGS)

#y.tab.h:
y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

test: lex.yy.c
	gcc tests/index.c -o test.out && ./test.out && rm ./test.out

clean:
	rm -f *.o *.d lex.yy.c y.tab.h y.tab.c $(Project)

##################################################
