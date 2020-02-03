CXX = g++
CXXFLAGS = -Wall -Wextra -Werror=return-type -pedantic -std=c++2a -g -fsanitize=address 
#-fsanitize=thread
# -fsanitize=address -fsanitize=leak
LBLIBS = -lpthread

EXEC = main

all: $(EXEC)

$(EXEC): actor.o test/test.cc
	$(CXX) $(CXXFLAGS) -o $(EXEC) test/test.cc actor.o $(LBLIBS) -I./

actor.o : core/actor.cc core/actor.h core/message.h base/ref.h base/log.h 3rd/LockFreeQueue/lockfree_queue.h 3rd/LockFreeQueue/reclaimer.h core/global_queue.h
	$(CXX) $(CXXFLAGS) -o actor.o -c core/actor.cc $(LBLIBS) -I./

clean:
	rm -rf *.o $(EXEC)