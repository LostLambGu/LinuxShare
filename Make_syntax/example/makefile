src = $(wildcard *.c)
objs = $(patsubst %.c, %.o, $(src))

dep_files := $(patsubst %.o, %.o.d, $(objs))
dep_files := $(wildcard $(dep_files))

CFLAGS = -Werror -Iinclude

test : $(objs)
	gcc -o test $^

#ifneq ($(dep_files),)
#include $(dep_files)
#endif

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF $@.d

show :
	@echo objs = $(objs)
	@echo dep_files = $(dep_files)

clean :
	rm *.o $(dep_files) test

.PHONY : show clean