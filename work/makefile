# 生成时间
BUILD_TIME := '$(shell date '+%Y-%m-%d %T')'

#编译工具变量
CPP = g++
CC=gcc
LD = g++
RM = rm -rf
NULL = /dev/null

# 编译生成文件
PROGRAM := ./bin/sys
TARGET=$(PROGRAM)

#编译参数变量
CFLAGS = -c -g -w -I /usr/local/include/log4cpp 
ASFLAGS = -c
# -L的要先指定，否则会报错
SYSLIBS =  -L /usr/local/lib/ -llog4cpp -lpthread	-lpaho-mqtt3c	
LDFLAGS = 
OPT_NORMAL = -funroll-loops 							# -funroll-loops编译选项使得程序中的循环步骤完全展开。
														# 这样会增加汇编代码的长度，并且攻击者需要隔离出每个循环，进行单独的修改，从而增大了攻击难度
OPT_CPP = -std=c++11									# cpp编译

##################################################################
#                      默认的编译规则                            #
##################################################################
.PHONY : all

# 变量定义-------------------------------------------------------

# 以下几个操作的目的是为了生成所有需要的.o文件的名字,根据.cpp文件自动生成.
# 具体是先检索cpp文件名,而后将.cpp后缀替换为.o,最后将目录都指向output

# 首先寻找src目录下的所有cpp文件
# 第一步:用shell命令列出src下所有目录
SOURCES :=$(shell ls -FR src | grep ':')

# 第二步:剔除目录后的冒号
SOURCES := $(patsubst %:, %, $(SOURCES)) 

# 生成包含路径
SOURCE_DIR := $(SOURCES)
# 把形如src src/aaa 的路径改为 -Isrc -Isrc/aaa
SOURCE_DIR := $(addprefix -I,$(SOURCE_DIR))


# 第三步:找出所有目录下的cpp文件
# 找到source的文件夹，对应在output上新建
OUTPUTDIR := $(subst src/,output/,$(SOURCES))
#$(foreach dir,$(OUTPUTDIR),$(shell mkdir $(dir)))

#找到所有的	.c .cpp
SOURCESALL := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp $(dir)/*.c))

# 将.cpp替换为.o
OBJS := $(patsubst %.cpp,%.o,$(SOURCESALL))
# 将.c替换为.o
OBJS := $(patsubst %.c,%.o,$(OBJS))

# 将src/替换为output/
OBJS :=$(subst src/,output/,$(OBJS))


# 变量定义结束-----------------------------------------------------

# 查找所有.cpp编译生成.o	
output/%.o : src/%.cpp
	@$(CPP)  $(SOURCE_DIR) $(CFLAGS) $(OPT_NORMAL) $(OPT_CPP) $< -o $@
# 查找所以.c编译生成.o
output/%.o : src/%.c
	@$(CC)  $(SOURCE_DIR) $(CFLAGS) $(OPT_NORMAL)  $< -o $@


all : 	$(TARGET)

$(TARGET) : $(OBJS)
	$(LD)  $(OBJS)	$(SYSLIBS)  -o	$(TARGET)
	chmod a+x $(TARGET)
	@echo "$(BUILD_TIME)"								# 打印编译时间


##################################################################
#                      默认的清除规则                            #
##################################################################
.PHONY : clean
clean:
	$(RM) output/*
	mkdir $(OUTPUTDIR)

