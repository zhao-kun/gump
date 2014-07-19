include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=libluabase${SHLIBEXT}
OBJS=baselua.o tcpcom.o
LINKRULE=${CC} -m32 ${SHLIBLDOPT} -o  ${TARGETS} ${OBJS}  -L${RUNNERDIR}/lib -llua -lgump

all:${TARGETS}

${TARGETS}:${OBJS}
	${LINKRULE}
	@cp ${TARGETS} ${RUNNERDIR}/lib/
	

.c.o:
	${CC} ${CFLAGS} -c -I${RUNNERDIR}/include $<

.cpp.o:
	${CPP} ${CPPFLAGS} -I${RUNNERDIR}/include $<


clean:
	rm -f ${TARGETS} ${OBJS} ${RUNNERDIR}/lib/${TARGETS}
