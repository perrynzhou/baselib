SOURCE  := $(wildcard src/*.c) $(wildcard ../src/*.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))


TARGET  := test_db

#compile and lib parameter
#编译参数
CC      := gcc
LIBS    := -lpthread
LDFLAGS :=
DEFINES :=
INCLUDE := -I./src -I./src/
CFLAGS  := -g -Wall -O0 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
  
  
#i think you should do anything here
#下面的基本上不需要做任何改动了
.PHONY : everything objs clean veryclean rebuild
  
everything : $(TARGET)
  
all : $(TARGET)
  
objs : $(OBJS) 
  
rebuild: veryclean everything
                
clean :
        rm -fr ./src/*.so  ./src/*.o
        rm -fr $(TARGET)
    
veryclean : clean
        rm -fr $(TARGET)
  
$(TARGET) : $(OBJS)
        $(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
