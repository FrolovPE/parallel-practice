allcppfiles = main.cpp pll.cpp args.cpp pll.h
extcpp = pll.cpp pll.h
obj = main.o pll.o
cxx = g++
exec = a.out
flags = -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
parallel = -pthread $(flags)
gprO3 = -pg $(flags)
gpr = -pg $(wo3)
wo3 = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
testfold =matrixtests/
testexe = ./a.out


a.out: $(obj)
	$(cxx) $(parallel) $(obj) -o $@

pll.o: $(extcpp)
	$(cxx) -c $< -o $@

main.o: $(allcppfiles)
	$(cxx) -c $< -o $@

clean:
	rm -f $(obj) $(exec)


