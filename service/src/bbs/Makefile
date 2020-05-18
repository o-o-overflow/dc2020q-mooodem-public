DISK_IMAGE=BBS.IMG
EXECUTABLE=BBS.EXE

all: $(DISK_IMAGE)

$(EXECUTABLE): main.c
	./dockbuild-ia16 make do-cross-build

.PHONY:
do-cross-build: main.c
	ia16-elf-gcc -o $(EXECUTABLE) -O0 -mcmodel=small -Xlinker -Map=$(EXECUTABLE).MAP -ffunction-sections -Xlinker --gc-sections $^

$(DISK_IMAGE): $(EXECUTABLE)
	# Create an AUTOEXEC.BAT to configure serial port and launch BBS service
	echo -n "" > AUTOEXEC.BAT
	echo "MODE COM1:9600,N,8,1,P" >> AUTOEXEC.BAT
	echo "$(EXECUTABLE) >COM1 <COM1" >> AUTOEXEC.BAT
	unix2dos AUTOEXEC.BAT

	# Generate FILES.TXT, whitelist of files that can be downloaded
	echo "QUOTES.TXT\nNOTES.TXT" > FILES.TXT

	# Generate dummy flag file as a clue for the downloadable image (overwritten during container build)
	echo "OOO{not the real flag}" > FLAG.TXT

	# Copy files into disk image
	cp BLANK.IMG $(DISK_IMAGE)
	mcopy -o -i $@ AUTOEXEC.BAT ::/AUTOEXEC.BAT
	mcopy -o -i $@ QUOTES.TXT ::/QUOTES.TXT
	mcopy -o -i $@ FILES.TXT ::/FILES.TXT
	mcopy -o -i $@ NOTES.TXT ::/NOTES.TXT
	mcopy -o -i $@ FLAG.TXT ::/FLAG.TXT
	mcopy -o -i $@ $< ::/$(EXECUTABLE)

	# Now compress a disk image that people can download to reverse offline
	rm -f BBS.ZIP
	zip BBS.ZIP $(DISK_IMAGE)

	# Add downloadable image to disk
	mcopy -o -i $@ BBS.ZIP ::/BBS.ZIP
	echo "BBS.ZIP" >> FILES.TXT
	mcopy -o -i $@ FILES.TXT ::/FILES.TXT

	# List final contents
	mdir -i $@

SERIAL=tcp:127.0.0.1:1235,server
.PHONY: run
run: $(DISK_IMAGE)
	qemu-system-i386 -drive index=0,if=floppy,format=raw,file=$< -serial $(SERIAL) -s -display none

.PHONY: clean
clean:
	rm -f $(DISK_IMAGE) $(EXECUTABLE) $(EXECUTABLE).MAP AUTOEXEC.BAT FILES.TXT FLAG.TXT BBS.ZIP
