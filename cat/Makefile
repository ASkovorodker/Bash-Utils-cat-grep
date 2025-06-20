SHELL := /bin/bash
FLAGS = -Wall -Werror -Wextra -std=c11
FILE = bytes.txt
CHECK = 2>&1  valgrind --tool=memcheck --leak-check=yes
SOURCE = s21_cat.c
TARGET = s21_cat

all: s21_cat tests
s21_cat: clean
	gcc $(FLAGS) $(SOURCE) -o $(TARGET)

tests:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -v $(FILE)) <(cat -v $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat -T $(FILE)) <(cat -T $(FILE))
	-diff <(./s21_cat -E $(FILE)) <(cat -E $(FILE))
	-diff <(./s21_cat -etsnb $(FILE)) <(cat -etsnb $(FILE))
	-diff <(./s21_cat -st $(FILE)) <(cat -st $(FILE))
	-diff <(./s21_cat -se $(FILE)) <(cat -se $(FILE))
	-diff <(./s21_cat -sn $(FILE)) <(cat -sn $(FILE))
	-diff <(./s21_cat -sb $(FILE)) <(cat -sb $(FILE))
	-diff <(./s21_cat -sT $(FILE)) <(cat -sT $(FILE))
	-diff <(./s21_cat -sE $(FILE)) <(cat -sE $(FILE))
	-diff <(./s21_cat --number-nonblank $(FILE)) <(cat --number-nonblank $(FILE))
	-diff <(./s21_cat --number $(FILE)) <(cat --number $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat --squeeze-blank $(FILE))

clean:
	rm -rf s21_cat