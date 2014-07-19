include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=libluacorba${SHLIBEXT}
OBJS=corba_entry.o corba_plugin.o
CXX=g++
LINKRULE=${CC} ${BIT} ${SHLIBLDOPT} -o  ${TARGETS} ${OBJS}  -L${RUNNERDIR}/lib -llua  -lmbadapt -lmb4c_rt  -ladtpub

all:${TARGETS}

${TARGETS}:${OBJS}
	${LINKRULE}
	@cp ${TARGETS} ${RUNNERDIR}/lib/
	

.c.o:
	${CC} ${CFLAGS} ${BIT} -c -I${RUNNERDIR}/include  -I./metabus $<

.cpp.o:
	${CXX}  ${BIT} -g -I${RUNNERDIR}/include   -I./metabus -c $<


clean:
	rm -f ${TARGETS} ${OBJS} ${RUNNERDIR}/lib/${TARGETS}
