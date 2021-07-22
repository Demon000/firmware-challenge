challenge-1: challenge-1.c
	gcc -Wall -g -o $@ $^

challenge-2: challenge-2.c
	gcc -Wall -g -o $@ $^

challenge-3: serial/serial.c serial/uart_driver.c
	gcc -Wall -g -o $@ $^
