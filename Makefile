#####MAKEFILE
CC=g++
.PHONY : clean
vcd_diff : SourceMain.o diff.o VCDparse.o ValChange.o
	$(CC) -o $@ SourceMain.o diff.o VCDparse.o ValChange.o

ValChange.o : ValChange.cpp 
	$(CC) -c ValChange.cpp 

VCDparse.o : VCDparse.cpp g_func.h
	$(CC) -c VCDparse.cpp

diff.o : diff.cpp g_func.h
	$(CC) -c diff.cpp 

SourceMain.o : SourceMain.cpp g_func.h
	$(CC) -c SourceMain.cpp

clean : 
	rm -f vcd_diff SourceMain.o diff.o VCDparse.o ValChange.o 
