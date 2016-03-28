TARGET = hdsfusemnt 
SRCS   = hdsfusemnt.cpp fuseentity.cpp hdsfuseoper.cpp debugUtility.c
OBJS   = $(addsuffix .o, $(basename $(SRCS))) 
DEPS   = $(addsuffix .d, $(basename $(SRCS)))

#LDFLAGS	      := -pthread libfuse.a -lulockmgr -ldl 
LDFLAGS	      := -pthread -lfuse -lulockmgr -ldl 

ifeq ($(ARCH), i386)  
#CXXFLAGS      := -g -O2 -Wall -DDEBUG_CALLSTACK  
#CFLAGS        := -g -O2 -Wall -DDEBUG_CALLSTACK 
else
C++11         = -std=c++11 -Wno-literal-suffix
CXXFLAGS      := -g -Wall -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse 
CFLAGS        := -g -Wall -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse
#STATIC_FLAGS  = -static
endif

all: $(OBJS) 
	${CROSS_COMPILE}g++ $(C++11) $(STATIC_FLAGS) -o $(TARGET) $(OBJS) $(CXXFLAGS) $(LDFLAGS)

-include $(DEPS)

%.o : %.cpp
	${CROSS_COMPILE}g++ $(C++11) $(CXXFLAGS) -c $< -o $@

%.o : %.c
	${CROSS_COMPILE}gcc $(CFLAGS) -c $< -o $@

# create dependency file (.d)
%.d: %.c
	${CROSS_COMPILE}gcc $(CFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

# create dependency file (.d)
%.d: %.cpp
	${CROSS_COMPILE}g++ $(CXXFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@ 

install:
	${CROSS_COMPILE}strip ${TARGET}
	${CROSS_COMPILE}strip ${TARGET}
	sudo cp ${SERVER_TARGET} ${ROOT_PATH}/usr/bin

clean:
	rm -rf *.d
	rm -rf *.o
	rm -f $(TARGET)

