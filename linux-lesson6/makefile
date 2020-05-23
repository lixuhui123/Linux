#a 生成对象 a.c 依赖对象
#gcc a.c -o a 编译命令
#a:a.c
#	gcc a.c -o a

#$^ 为所有的依赖对象
#$@ 为生成对象
#$< 依赖对象中的第一个


#a:a.c b.c
#	gcc $^ -o $@

#.PHONY:a
#a:a.o b.o
#	gcc $^ -o $@
#
#%.o:%.c
#	gcc -c $< -o $@
#
#.PHONY:clean
#clean:
#	rm a a.o b.o

src=$(wildcard *.c)
obj=$(patsubst %.c, %.o, $(src))

bin=aa

CFLAGS=-c
#debug
ifdef _DEBUG
CFLAGS=-c -g
endif

include makefile.inc
