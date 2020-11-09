EXECS=danc_parser

CC=g++

MYFILE=Mostofa_Shakib_R11545508_Assignment4.cpp

all: ${EXECS}

${EXECS}: ${MYFILE}
	${CC} -o ${EXECS} ${MYFILE}

clean:
	rm -f ${EXECS}
