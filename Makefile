# avra config for assembling
AVRA?=		avra
AVRA_INCDIR?=	/usr/local/share/avra/includes/
AVRA_FLAGS+=	-I ${AVRA_INCDIR}

# avrdude config for flashing
AVRDUDE?=	sudo avrdude
AVRDUDE_PORT?=	/dev/ppi0
AVRDUDE_PROGRAMMER?=	amdmi3 # config for custom parallel port programmer
AVRDUDE_FLAGS+=	-p t13 -c ${AVRDUDE_PROGRAMMER} -P ${AVRDUDE_PORT}

TARGET?=	multitasking

ALL_TARGETS=	flashingleds subroutines interrupts timer multitasking

all: ${ALL_TARGETS}

target: ${TARGET}

flash: ${TARGET}
	${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:${FLASH_TARGET}.hex

.asm:
	${AVRA} ${AVRA_FLAGS} ${.IMPSRC}

clean:
	rm -f *.hex *.cof *.obj
