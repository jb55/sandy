NAME ?= sandy
BIN ?= $(NAME)
PREFIX ?= /usr/local
CFLAGS = -ggdb -O0  -I include -Wall -Wextra -std=c99 \
						-Wno-unused-function \
						-Wno-unused-parameter \
						-Wno-unused-variable \
						-Wno-cast-align \
						-Wno-padded
LDFLAGS = -lSDL2 -lGL

SRC=src

SHLIB=$(SRC)/lib$(NAME).so

OBJS = $(SRC)/event.o
OBJS += $(SRC)/render.o
OBJS += $(SRC)/world.o
OBJS += $(SRC)/field.o
OBJS += $(SRC)/morton.o

all: $(BIN)

%.o: %.c ../include/%.h
	$(CC) $(CFLAGS) -fPIC $(DEFS) -c $< -o $@

$(SHLIB): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@

$(BIN): $(SRC)/main.o $(SHLIB)
	$(CC) $(CFLAGS) $(DEFS) $^ $(LDFLAGS) -o $@

install: $(BIN)
	install -d $(PREFIX)/bin
	install $(BIN) $(PREFIX)/bin

nixbuild:
	nix-shell --command 'make -j4'

clean:
	rm -f $(SRC)/main.o $(OBJS) $(SHLIB) $(BIN)
