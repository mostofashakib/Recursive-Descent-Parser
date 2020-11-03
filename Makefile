EXECS=danc_parser

CC=g++

MYFILE=main.cpp

all: ${EXECS}

${EXECS}: ${MYFILE}
	${CC} -o ${EXECS} ${MYFILE}

clean:
	rm -f ${EXECS}
