#I would strongly recommend NOT changing any lines below except the CC and MYFILE lines.
#Before running this file, run the command:   module load gnu


EXECS=danc_analyzer

#Replace the g++ with gcc if using C
CC=g++

#Replace with the name of your C or C++ source code file.
MYFILE=parser.cpp


all: ${EXECS}

${EXECS}: ${MYFILE}
	${CC} -o ${EXECS} ${MYFILE}

clean:
	rm -f ${EXECS}
