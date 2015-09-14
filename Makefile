CC = g++
CFLAGS = -O3 -g -fopenmp -std=c++0x

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

ifeq ($(strip $(IMPL)), )
 IMPL=wyllie
endif

correctness: correctness.o listrank_$(IMPL).o listutils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

metrics: metrics.o listrank_$(IMPL).o listutils.o timer.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ core correctness metrics *.o
