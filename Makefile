CC	=	gcc
CXX	=	g++
LINK = g++
ECHO := echo
CFLAGS := -x c -std=gnu99 -O3
CCFLAGS := -x c++ -std=gnu++0x -I. -I/usr/include -Wno-write-strings -O3
LDFLAGS := -L/usr/lib -lpthread -lstdc++
GTEST = ./gtest/src/gtest_main.a

.PHONY: all, test, force_look, clean

all: test solve

test: network_sort_test board_test solve_test
	./network_sort_test
	./board_test
	./solve_test

RESULTS: run.sh solve
	./run.sh > $@

$(GTEST): force_look
	$(ECHO) looking into gtest/src : $(MAKE) $(MFLAGS)
	cd gtest/src; $(MAKE) $(MFLAGS)

force_look :
	true

clean:
	rm -f *.o
	rm -f *.s
	rm -f *_test
	rm -f benchmark_move
	rm -f solve

################################################################################
# board
################################################################################

board.o: board.cc board.h network_sort.h
	$(CXX) $(CCFLAGS) -c $< -o $@ 
board_test.o: board_test.cc board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

board_test: board_test.o board.o network_sort.o $(GTEST)
	$(LINK) -o $@ $^ $(LDFLAGS)

################################################################################
# solve
################################################################################

solve.o: solve.cc solve.h board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

solve_test.o: solve_test.cc solve.h testboards.h
	$(CXX) $(CCFLAGS) -c $< -o $@

solve_test: solve.o solve_test.o board.o network_sort.o getcputime.o $(GTEST)
	$(LINK) -o $@ $^ $(LDFLAGS)

solver_main.o: solver_main.cc solve.h board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

solve: solver_main.o solve.o board.o network_sort.o
	$(LINK) -o $@ $^ $(LDFLAGS)

################################################################################
# network_sort
################################################################################

network_sort.o: network_sort.c network_sort.h
	$(CC) $(CFLAGS) -c $< -o $@

network_sort_test.o: network_sort_test.cc network_sort.h
	$(CXX) $(CCFLAGS) -c $< -o $@

network_sort_test: network_sort_test.o network_sort.o $(GTEST)
	$(LINK) -o $@ $^ $(LDFLAGS)

################################################################################
# misc
################################################################################

getcputime.o: getcputime.c getcputime.h
	$(CC) $(CFLAGS) -c $< -o $@

benchmark_move.o: benchmark_move.c
	$(CXX) $(CCFLAGS) -c $< -o $@

benchmark_move: benchmark_move.o getcputime.o board.o network_sort.o
	$(LINK) -o $@ $^ $(LDFLAGS)

solve_dfs.o: solve_dfs.cc solve_dfs.h board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

solve_dfs_test.o: solve_dfs_test.cc solve_dfs.h testboards.h
	$(CXX) $(CCFLAGS) -c $< -o $@

solve_dfs_test: solve_dfs.o solve_dfs_test.o board.o getcputime.o network_sort.o $(GTEST)
	$(LINK) -o $@ $^ $(LDFLAGS)
