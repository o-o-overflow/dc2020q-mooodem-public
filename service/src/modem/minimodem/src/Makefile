.PHONY: minimodem
minimodem:
	touch config.h; gcc -o minimodem -g -D USE_SNDFILE=1 -DHAVE_CONFIG_H -DVERSION='"fuckoff"' *.c -lsndfile -lfftw3 -lm -lfftw3f
