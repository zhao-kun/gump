#Public definitions will be found here
MAKE=make
MAKEOPT="BIT=-m32"

ALLDIRS=${RUNNERDIR}/src/plugin/corba \
	${RUNNERDIR}/src/plugin/http \
	${RUNNERDIR}/src/plugin/tuxedo \
	${RUNNERDIR}/src/core


all:
	@for subdir in ${ALLDIRS}; \
	do \
		cd $$subdir; \
		echo "Makeing $$subdir"; \
		${MAKE} ${MAKEOPT} ; \
		echo "Done"; \
	done

clean:
	@for subdir in ${ALLDIRS}; \
	do \
		cd $$subdir; \
		echo "Cleanning $$subdir"; \
		${MAKE} ${MAKEOPT} clean; \
		echo "Done"; \
	done

config:
	echo "Configure ${RUNNERDIR}/autoconf/";
	cd ${RUNNERDIR}/autoconf; ./configure 

pub32:
	cd pub; make 32bit

pub:
	cd pub; make 64bit
