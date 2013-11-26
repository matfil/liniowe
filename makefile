all : asd

asd : asd.cc
	g++-4.7 -std=c++11 asd.cc timer.cc main.cc ./ltest_liniowe2.so -o asd 
	
clean :
	rm asd
	rm asd_debug
	
debug : asd.cc
	g++-4.7 -std=c++11 -g -D _SUNOS asd.cc timer.cc main.cc ./ltest_liniowe2.so -o asd_debug
	gdb asd_debug 
	

view:
	lynx info/index.html
