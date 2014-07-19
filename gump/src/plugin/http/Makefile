include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=libluahttp${SHLIBEXT}
OBJS=httplua.o
LINKRULE=${CC} ${BIT}  ${SHLIBLDOPT} -o  ${TARGETS} ${OBJS}  -L${RUNNERDIR}/lib -llua -lgump -lcurl

all:${TARGETS}

${TARGETS}:${OBJS}
	${LINKRULE}
	@cp ${TARGETS} ${RUNNERDIR}/lib/
	

.c.o:
	${CC} ${CFLAGS} ${BIT} -c -I${RUNNERDIR}/include $<

.cpp.o:
	${CPP} ${CPPFLAGS} -I${RUNNERDIR}/include $<


clean:
	rm -f ${TARGETS} ${OBJS} ${RUNNERDIR}/lib/${TARGETS}
