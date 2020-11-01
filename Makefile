EXECS=danc_parser

#Replace the g++ with gcc if using C
CC=g++

#Replace with the name of your C or C++ source code file.
MYFILE=Mostofa_Shakib_R11545508_Assignment4.cpp


all: ${EXECS}

${EXECS}: ${MYFILE}
	${CC} -o ${EXECS} ${MYFILE}

clean:
	rm -f ${EXECS}
