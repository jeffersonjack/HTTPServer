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
	   src/directory.c \
	   src/getdirlist.c \
	   src/makefilepath.c \
	   src/resptostr.c \
	   src/headers.c \
	   src/contenttype.c \
	   src/printhttp.c \
	   src/httptime.c \
	   src/makeresponse_dir.c \
	   src/makeresponse_file.c \
       src/makeresponse_404.c
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
