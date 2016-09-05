
# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	main.c Bomberman.c vbe.c video_gr.c timer.c keyboard.c sprite.c mouse.c Player.c Bitmap.c map.c bomb.c menu.c rtc.c

CCFLAGS= -Wall -03

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=


.include <bsd.prog.mk>
.include <bsd.gcc.mk>
