# avra config for assembling
AVRA?=		avra
AVRA_INCDIR?=	/usr/local/share/avra/includes/
AVRA_FLAGS+=	-I ${AVRA_INCDIR}

# avrdude config for flashing
AVRDUDE?=	avrdude
AVRDUDE_PORT?=	/dev/ugen0.3
AVRDUDE_PROGRAMMER?=	usbasp
AVRDUDE_FLAGS+=	-p t2313 -c ${AVRDUDE_PROGRAMMER} -P ${AVRDUDE_PORT}

all: ${TARGET}

flash: ${TARGET}
	${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:${TARGET}.hex

.asm:
	${AVRA} ${AVRA_FLAGS} ${.IMPSRC}

clean:
	rm -f *.hex *.cof *.obj
