$(MAKE) = make

all:
	$(MAKE) -C src -f Makefile

clean:
	$(MAKE) -C src -f Makefile clean
