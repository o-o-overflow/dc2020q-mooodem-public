all: BBS.IMG
#minimodem killondisconnect

.PHONY: BBS.IMG
BBS.IMG:
	make -C src/bbs
	cp src/bbs/BBS.IMG $@

.PHONY: minimodem
minimodem:
	make -C src/modem/minimodem/src
	cp src/modem/minimodem/src/minimodem $@

killondisconnect: src/killondisconnect.c
	gcc -o $@ $<
