EXE := ionosphericraytracer
LIBS := -lboost_system -lboost_log -lboost_thread -ljson_linux-gcc-4.9.1_libmt -lpthread
SOURCES := $(wildcard src/*/*.cpp)
#USERLIBS = $(wildcard *.cpp) $(wildcard contrib/*.cpp)
USERLIBS := $(wildcard contrib/*.cpp)

all: $(EXE)

$(EXE): 
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -L/usr/include/boost/log -p -pg -Wall -fmessage-length=0 -std=c++11 -DBOOST_LOG_DYN_LINK \
	$(SOURCES) $(LIBS) $(USERLIBS) \
	-o $(EXE)
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	rm $(EXE)
