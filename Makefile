PROJECT_PATH ?= $(CURDIR)

include 3rdparty/altor32/rtl/sim_lib/altor32.mk

all: $(ALTOR32_LIB)
	$(MAKE) -C$(PROJECT_PATH)/src
	$(MAKE) -C$(PROJECT_PATH)/test_bin 

clean:
	rm -rf build/altor32 test
	$(MAKE) -C$(PROJECT_PATH)/src clean
	$(MAKE) -C$(PROJECT_PATH)/test_bin clean