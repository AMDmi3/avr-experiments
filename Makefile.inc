# sanity check
.if !defined(TARGET)
.error TARGET not defined
.endif

.if !defined(DEVICE)
.error DEVICE not defined
.endif

# detect language
.if exists(${TARGET}.c)
LANGUAGE=	c
.elif exists(${TARGET}.asm)
LANGUAGE=	asm
.else
.error Unknown language
.endif

# avra config for assembling
AVRA?=		avra
AVRA_INCDIR?=	/usr/local/share/avra/includes/
AVRA_FLAGS+=	-I ${AVRA_INCDIR}

# avr-gcc for C compilation
AVR_CC?=	avr-gcc
AVR_CFLAGS?=	-Os
AVR_CFLAGS+=	-mmcu=${DEVICE} -Os

# avrdude config for flashing
AVRDUDE?=	avrdude
AVRDUDE_PORT?=	/dev/ugen0.3
AVRDUDE_PROGRAMMER?=	usbasp
AVRDUDE_FLAGS+=	-p ${DEVICE} -c ${AVRDUDE_PROGRAMMER} -P ${AVRDUDE_PORT}

all: ${TARGET}

flash: ${TARGET}
.if ${LANGUAGE} == "asm"
	${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:${TARGET}.hex
.else
	${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:${TARGET}
.endif

.asm:
	${AVRA} ${AVRA_FLAGS} ${.IMPSRC}

.c: ${DEPENDS}
	${AVR_CC} ${AVR_CFLAGS} -o ${.TARGET} ${.IMPSRC}

clean:
	rm -f *.hex *.cof *.obj
