CC = clang
CFLAGS = -Wall -Werror -std=gnu99
BIN = server
LIBS =
SRCDIR = src
OBJDIR = obj
SRCS = src/main.c \
       src/getlisteningsocket.c \
       src/iptostr.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(CFLAGS) $+ $(LIBS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./$(BIN) 1030

clean:
	rm -rf $(BIN) $(OBJDIR)/*.o
