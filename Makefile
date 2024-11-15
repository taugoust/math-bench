bin=math.$(shell uname -m)

all: $(bin)

$(bin): math.cpp
	$(CXX) -O3 -lm -o $@ $<

clean:
	rm -rf *~ *.o

mrproper: clean
	rm -rf $(bin)

.PHONY: all clean mrproper
