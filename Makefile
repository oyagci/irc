.PHONY: all clean fclean server client

all: server client

server:
	make -C src/server

client:
	make -C src/client

clean:
	make -C src/client clean
	make -C src/server clean

fclean: clean
	make -C src/server fclean || (exit 0)
	make -C src/client fclean || (exit 0)

re: fclean all

depend:
	make -C src/client depend
	make -C src/server depend
