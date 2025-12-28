allcppfiles = main.cpp pll.cpp args.h pll.h mytime.cpp mytime.h
extpll= pll.cpp pll.h
extmytime =  mytime.cpp mytime.h
obj = main.o pll.o mytime.o
cxx = g++
exec = a.out ga.out
flags = -O3 -mfpmath=sse -fsanitize=thread -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
parallel = -pthread -DNDEBUG -Wunused-variable  $(flags)
parallelWO3 = -pthread -fsanitize=thread $(wo3)
gprO3 = -pg $(flags)
gpr = -pg $(wo3)
wo3 = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
testfold =matrixtests/
testexe = ./a.out



a.out: $(obj)
	$(cxx) $(parallel) $(obj) -o $@

ga.out: $(obj)
	$(cxx) $(parallelWO3) $(obj) -o $@

all: $(exec)

pll.o: $(extpll)
	$(cxx) -c  $(flags) $< -o $@

main.o: $(allcppfiles)
	$(cxx) -c $(flags) $< -o $@

mytime.o: $(extmytime)
	$(cxx) -c $(flags) $< -o $@

clean:
	  
	rm -f $(obj) $(exec) Frolov_PS.zip pll.zip
	
	

mainzip:
	zip Frolov_PS.zip $(allcppfiles) makefile 

zip:
	zip pll.zip $(allcppfiles) makefile  

gitadd:
	git add $(allcppfiles) makefile

