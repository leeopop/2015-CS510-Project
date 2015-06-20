PROJECT_PATH ?= $(CURDIR)/../

CXXFLAGS+= -std=c++11
CXXFLAGS+= -I/usr/share/verilator/include
CXXFLAGS+= -I/usr/share/verilator/include/vltstd
CXXFLAGS+= -I$(PROJECT_PATH)/3rdparty/KENSv3/include
CXXFLAGS+= -I$(PROJECT_PATH)/build/altor32
CXXFLAGS+= -L$(PROJECT_PATH)/build/altor32
CXXFLAGS+= -L$(PROJECT_PATH)/3rdparty/KENSv3/lib

VERILATOR_ROOT = /usr/share/verilator
#VERILATOR_FILES+= $(wildcard $(VERILATOR_ROOT)/include/*.c)
VERILATOR_FILES+= \
$(VERILATOR_ROOT)/include/verilated_cov.cpp \
$(VERILATOR_ROOT)/include/verilated.cpp \
$(VERILATOR_ROOT)/include/verilated_dpi.cpp \
$(VERILATOR_ROOT)/include/verilated_save.cpp \
$(VERILATOR_ROOT)/include/verilated_vcd_c.cpp 
#$(VERILATOR_ROOT)/include/verilated_vpi.cpp
#verilated_vcd_sc.cpp 

SOURCES+= main.cpp top.cpp module.cpp
OBJS+= $(SOURCES:.cpp=.o)
LIBS= -laltor32 -lelf -le

CXXFLAGS+= -g -O0 

all: $(PROJECT_PATH)/build/test

$(PROJECT_PATH)/build/test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(VERILATOR_FILES) $(OBJS) -Wl,--start-group $(LIBS) -Wl,--end-group -o $(PROJECT_PATH)/build/test 
	
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf *.o