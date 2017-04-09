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

CFLAGS	= -std=c99
LDFLAGS	=

##################################################

# Name of the project (executable binary)
Project = etapa1

# Source files names
SourceFiles = main.c etapa1.c lex.yy.c hash.c

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

lex.yy.c: scanner.l
	lex scanner.l

test: lex.yy.c
	gcc tests/index.c -o test.out && ./test.out && rm ./test.out

clean:
	rm -f *.o *.d lex.yy.c $(Project)

##################################################
