

CC:=gcc
#CC:=arm-xilinx-linux-gnueabi-gcc

OBJS=main.o bktree.o
CFLAGS=-Wall

all: app

app: ${OBJS}
	${CC} -o $@ ${OBJS}
%.o:%.c
	$(CC) -c  ${CFLAGS} $<

clean:
	rm -rf *.o app
	
