We have made changes to the following files in the xv6 folder: syscall.c, syscall.h, sysproc.c, user.h, usys.S, printf.c. We added some new files to the folder as well. 

**To compile and start xv6**

make

make qemu-nox


**part 4.2.1 strace on and off**

strace on

echo hello

strace off

echo hello


**part 4.2.3 strace run**

strace off

echo hi

strace run echo hi

echo hi


**part 4.2.4 strace dump**

strace on

echo helloWorld!

cat README

strace dump

echo helloAgain

strace dump


**part 4.2.5 Child Process**

strace on

trace_fork


**part 4.2.6 Fomatting output**

strace on

ls

echo hello

cat README


**part 4.3 e-flag**

strace on

strace -e write

echo hi

echo hi

strace -e fork

trace_fork

**part 4.3 s-flag and f-flag and combined**

strace -s

application

strace -f 

application

strace -f -e open

application 

strace -s -e open

application

**4.3.5 Implement -c Option**

strace on

strace -c ls

strace -c cat README

strace -c echo hi

**4.4 Write output to file**

strace on 

strace -o outFile cat README

strace off

cat outFile

**4.5 Application** 

strace on	

application_2

strace -c application_2 

strace -f 

application_2


**4.5 Tracing on Linux**

gcc -o application_2 application_2.c

./application_2

strace -c ./application_2

strace -e trace=openat,read,close,access ./application_2




