DEST	      = /usr/pds/bin

DESTMAN	      = /usr/man/catp/idle.p

EXTHDRS	      = /usr/include/X/Xlib.h \
		/usr/include/curses.h \
		/usr/include/math.h \
		/usr/include/signal.h \
		/usr/include/stdio.h \
		/usr/include/strings.h \
		/usr/include/sys/time.h

LDFLAGS	      = -lX -lcurses -ltermcap

LINKER	      = cc

OBJS	      = idle.o

PROGRAM	      = idle

SRCS	      = idle.c

INCLUDES      = csr.icon \
		csr_mask.icon \
		idle.icon

MAN	      = idle.man

CAT	      = idle.cat

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo -n "Loading $(PROGRAM) ... "
		@$(LINKER) $(OBJS) $(LIBS) -o $(PROGRAM) $(LDFLAGS)
		@echo "done"

clean:;		@rm -f $(OBJS) $(CAT)

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@install -s $(PROGRAM) $(DEST)

program:        $(PROGRAM)

man:		$(CAT)
		cp $(CAT) $(DESTMAN)

$(CAT):		$(MAN)
		jnroff -man $(MAN) >$(CAT)

$(OBJS): $(EXTHDRS) $(INCLUDES)

