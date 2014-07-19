include ${RUNNERDIR}/etc/Make.defines

.SUFFIXES:
.SUFFIXES:.c .cpp .o


TARGETS=luatest luatest2
OBJS=pr_lua_util.o util.o
LINKRULE=${CC} -m32 -o  ${TARGETS} ${OBJS}  -L${RUNNERDIR}/lib -llua ${SYSLIBS}

all:${TARGETS}

luatest:testlua.o ${OBJS}
	${CC} -m32 -o $@ $^ -L${RUNNERDIR}/lib -llua ${SYSLIBS}

luatest2:testlua2.o ${OBJS}
	${CC} -m32 -o $@ $^ -L${RUNNERDIR}/lib -llua ${SYSLIBS}
	

.c.o:
	${CC} ${CFLAGS} -c -I${RUNNERDIR}/include $<

.cpp.o:
	${CPP} ${CPPFLAGS} -I${RUNNERDIR}/include $<


clean:
	rm -f ${TARGETS} ${OBJS}
