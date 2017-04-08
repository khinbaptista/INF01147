##################################################
#
#	Makefile
#	--
#	Khin Baptista
#	Lorenzo Dal'Aqua
#
##################################################

CC	= gcc
LINKER	= gcc

CFLAGS	= -Wall -std=c99
LDFLAGS	=

##################################################

# Name of the project (executable binary)
Project = etapa1

# Source files names
SourceFiles = main.c hash.c

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

lex: scanner.l
	lex scanner.l && gcc lex.yy.c && ./a.out

clean:
	rm -f *.o *.d lex.yy.c a.out $(Project)

##################################################
