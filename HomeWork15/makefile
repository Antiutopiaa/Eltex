# Определяем компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra

# Определяем цели для каждого исполняемого файла
TARGETS = 1/prog_sigaction 2/prog_sigprocmask 3/prog_sigwait prog_kill

all: $(TARGETS)

# Цели для каждого исполняемого файла
1/prog_sigaction: 1/sigaction.c
	$(CC) $(CFLAGS) -o $@ $<

2/prog_sigprocmask: 2/sigprocmask.c
	$(CC) $(CFLAGS) -o $@ $<

3/prog_sigwait: 3/sigwait.c
	$(CC) $(CFLAGS) -o $@ $<

prog_kill: kill.c
	$(CC) $(CFLAGS) -o $@ $<

# Очистка сгенерированных файлов
clean:
	rm -f $(TARGETS)

.PHONY: clean
