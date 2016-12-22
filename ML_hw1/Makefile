CC = g++
CFLAGS =-Wall
.PHONY: all
data = 15 16 17 18 19 20
all: ${data}
%: %.cpp
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f ${data}
