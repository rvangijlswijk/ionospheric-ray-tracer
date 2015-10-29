EXE := ionosphericraytracer
TESTEXE := ionosphericraytracertest
LIBS := -lboost_system -lboost_log -lboost_thread -ljson_linux-gcc-4.9.1_libmt -lpthread
SOURCES := $(wildcard src/*/*.cpp)
TESTCASES := $(wildcard test/*/*.cpp)
#USERLIBS = $(wildcard *.cpp) $(wildcard contrib/*.cpp)
USERLIBS := $(wildcard contrib/*/*.h) $(wildcard contrib/*/*.cc) $(wildcard contrib/*/*.cpp)

all: $(EXE)
test: $(TESTEXE)

# Compile the simulation
$(EXE): 
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -L/usr/include/boost/log -p -pg -Wall -fmessage-length=0 -std=c++11 -DBOOST_LOG_DYN_LINK \
	src/main.cpp $(SOURCES) $(LIBS) $(USERLIBS) \
	-o $(EXE)
	@echo 'Finished building target: $@'
	@echo ' '
	
# Compile the test program, used to run unittests
$(TESTEXE):
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -L/usr/include/boost/log -p -pg -Wall -fmessage-length=0 -std=c++11 -DBOOST_LOG_DYN_LINK \
	test/main.cpp $(TESTCASES) $(SOURCES) $(LIBS) $(USERLIBS) \
	-o $(TESTEXE)
	@echo 'Finished building target: $@'
	@echo ' '

# Cleanup
clean:
	@echo 'Cleaning:'
	@if [ -f $(EXE) ]; \
	then \
		rm $(EXE); \
		echo 'Removed $(EXE)'; \
	fi;
	@if [ -f $(TESTEXE) ]; \
	then \
		rm $(TESTEXE); \
		echo 'Removed $(TESTEXE)'; \
	fi;
	@echo 'Finished cleaning'
	@echo ' '