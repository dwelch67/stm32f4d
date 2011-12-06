
all : showmidi logmidi mididump

mididump : mididump.c
	gcc mididump.c -o mididump

showmidi : showmidi.c ser.c ser.h 
	gcc showmidi.c ser.c -o showmidi


logmidi : logmidi.c ser.c ser.h 
	gcc logmidi.c ser.c -o logmidi

clean :
	rm -f showmidi
	rm -f logmidi

