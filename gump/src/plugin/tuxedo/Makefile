include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=libluatuxedo${SHLIBEXT} 
TARGETSW=libluawtuxedo${SHLIBEXT}
OBJS=tuxedolua.o
LINKRULE=${CC} ${BIT}  ${SHLIBLDOPT} -o  ${TARGETS} ${OBJS}  -L${RUNNERDIR}/lib -llua -lgump -lcurl   -L${TUXDIR}/lib  -ltux -lbuft  -lfml -lfml32 -lengine  -ldl -lpthread /usr/lib/libcrypt.a
LINKRULEW=${CC} ${BIT}  ${SHLIBLDOPT} -o  ${TARGETSW} ${OBJS}  -L${RUNNERDIR}/lib -llua -lgump -lcurl   -L${TUXDIR}/lib  -lwsc -lbuft  -lfml -lfml32 -lengine  -ldl -lpthread /usr/lib/libcrypt.a

all:${TARGETS} ${TARGETSW}

${TARGETS}:${OBJS}
	${LINKRULE}
	@cp ${TARGETS} ${RUNNERDIR}/lib/
	
${TARGETSW}:${OBJS}
	${LINKRULEW}
	@cp ${TARGETS} ${RUNNERDIR}/lib/

.c.o:
	${CC} ${CFLAGS} ${BIT} -c -I${RUNNERDIR}/include -I${TUXDIR}/include $<

.cpp.o:
	${CPP} ${CPPFLAGS} -I${RUNNERDIR}/include $<


clean:
	rm -f ${TARGETS} ${OBJS} ${RUNNERDIR}/lib/${TARGETS}
