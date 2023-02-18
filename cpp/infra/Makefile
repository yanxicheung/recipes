all:clean main.out
	@echo "clean first then compile then link"
	@echo $(OBJS)

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CC = g++
CFLAGS += -c  -Wall -g -std=c++11 -D_LINUX_

main.out:$(OBJS);@echo "link"
	$(CC) $^ -o $@  -lpthread
	
%.o:%.cpp;@echo "complie"
	$(CC) $^ $(CFLAGS) -o $@

PHONY:clean
clean:
	$(RM) *.o *.out -r
