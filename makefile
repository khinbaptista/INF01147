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

CFLAGS	= -std=c99 -Wall
LDFLAGS	=

##################################################

# Name of the project (executable binary)
Project = lang171

# Source files names
SourceFiles = main.c lex.yy.c y.tab.c hash.c lang171.c

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

y.tab.h:
y.tab.c: parser.y
	yacc -d parser.y

test: lex.yy.c
	gcc tests/index.c -o test.out && ./test.out && rm ./test.out

clean:
	rm -f *.o *.d lex.yy.c $(Project)

##################################################
