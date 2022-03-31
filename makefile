CC=gcc
SERVER=text-server
CLIENT=text-client

text: $(SERVER) $(CLIENT)

text-client:
	$(CC) -o $(CLIENT) $(CLIENT).c

text-server:
	$(CC) -o $(SERVER) $(SERVER).c

clean:
	rm -rf $(CLIENT) $(SERVER) server client *.o

tmp:
	$(CC) -o server server.c && gcc -o client client.c
