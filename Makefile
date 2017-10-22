CC = clang
CFLAGS = -g -Wall -Werror -std=gnu99
BIN = server
LIBS = -lpthread
SRCDIR = src
OBJDIR = obj
SRCS = src/main.c \
	   src/getlisteningsocket.c \
	   src/iptostr.c \
	   src/handleclient.c \
	   src/acceptconnections.c \
	   src/parserequest.c \
	   src/filesize.c \
	   src/makefilepath.c \
	   src/resptostr.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(CFLAGS) $(LIBS) $+

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./$(BIN) 1030

clean:
	rm -rf $(BIN) $(OBJDIR)
