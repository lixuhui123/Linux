
$(bin):$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm $(bin) $(obj)
