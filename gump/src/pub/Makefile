
CONFIGURE=./configure

TARGETS32=hiredis32 lua32 config32 
TARGETS=hireds lua config

32bit:${TARGETS32}

64bit:${TARGETS}

all:${TARGETS32}

#all:hireds lua config

#针对linux 32 的编译选项，如果不是linux32 可以把这部份去掉 --host=i686-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
config32:
	cd curl; ${CONFIGURE} --prefix=${RUNNERDIR}  --host=i686-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"; make clean ;make ;make install

hiredis32:
	cd hiredis;make clean;make 32bit 

lua32:
	cd lua;make clean;make linux32

config:
	cd curl; ${CONFIGURE} --prefix=${RUNNERDIR}  --host=i686-linux-gnu ; make clean ;make ;make install

hiredis:
	cd hiredis;make clean;make

lua:
	cd lua;make clean;make


clean:
	cd curl;make clean;
	cd hiredis;make clean;
	cd lua;make clean;
