GCC = g++

EXTRA_DIST = sim.h\
		 sim_module.cpp  \
		 controller.h  


MODULES = ctrl_module sim_module 
LDFLAGS = -L/usr/local/lib -L/usr/lib
LDLIBS = -lktl -lm -lpthread -lts01


all: $(MODULES)

ctrl_module: CSV.o ctrl_module.o arm.o  
	$(GCC)   -o $@ $< ctrl_module.o arm.o  ${INCLUDE} ${LDFLAGS} ${LDLIBS} 

ctrl_module.o: ctrl_module.cpp controller.h 
	$(GCC) -W ${INCLUDE} ${CXXFLAGS} -c $< -o $@ 

CSV.o:./wire_ref/readCSV.cpp
	$(GCC) -c $< -o $@ 

sim_module: sim_tmp.o arm.o 
	$(GCC)   -o $@ $< arm.o ${LDFLAGS} ${LDLIBS}

sim_tmp.o: sim_module.cpp sim.h
	$(GCC) -W ${INCLUDE} ${CXXFLAGS} -c $< -o $@

clean:
	rm -f $(MODULES) *.o  *~


