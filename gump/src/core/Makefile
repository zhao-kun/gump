include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=gump
LIBTARGETS=libgump${SHLIBEXT} 

OBJS=gp_loadcfg.o gp_main.o  gp_lua_util.o gp_redis_util.o baselua.o tcpcom.o
LIBOBJS=gp_util.o

LINKRULE=${CC} ${BIT} -o  ${TARGETS} ${OBJS} ${SYSLIBS} -L${RUNNERDIR}/lib -llua -lhiredis -lgump 

all:${LIBTARGETS} ${TARGETS}

${LIBTARGETS}:${LIBOBJS}
	${CC} ${BIT} ${SHLIBLDOPT} -o  ${LIBTARGETS} ${LIBOBJS}
	@cp ${LIBTARGETS} ${RUNNERDIR}/lib

${TARGETS}:${OBJS}
	${LINKRULE}
	@cp ${TARGETS} ${RUNNERDIR}/bin/
	

.c.o:
	${CC} ${CFLAGS} ${BIT} -c -I${RUNNERDIR}/include $<

.cpp.o:
	${CPP} ${CPPFLAGS} -I${RUNNERDIR}/include $<


clean:
	rm -f ${TARGETS} ${LIBTARGETS} ${OBJS} ${LIBOBJS}
