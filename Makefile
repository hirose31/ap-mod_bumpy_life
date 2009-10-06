
APXS      = apxs
APACHECTL = apachectl

MODULE    = mod_bumpy_life

MY_CFLAGS = -Wall
#MY_CFLAGS = -Wall -DDEBUG

all: $(MODULE).o

$(MODULE).o: $(MODULE).c
	$(APXS) -Wc,'$(MY_CFLAGS)' -c    $(MODULE).c

install: $(MODULE).c
	$(APXS) -Wc,'$(MY_CFLAGS)' -c -i $(MODULE).c

clean:
	$(RM) *.o *.lo *.slo *.la *~
	$(RM) -r .libs

start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

