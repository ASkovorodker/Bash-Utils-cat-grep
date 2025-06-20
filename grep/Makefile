SHELL := /bin/bash
FLAGS = -Wall -Werror -Wextra -std=c11
FILE = test_grep.txt
FILE2 = test_grep2.txt
REGFILE = regex.txt
CHECK = 2>&1  valgrind --tool=memcheck --leak-check=yes
SOURCE = s21_grep.c
TARGET = s21_grep
REGEX = 00

all: s21_grep tests
s21_grep: clean
	gcc $(FLAGS) $(SOURCE) -o $(TARGET)

tests:
	-diff <(./s21_grep $(REGEX) $(FILE)) <(grep $(REGEX) $(FILE))
	-diff <(./s21_grep -n $(REGEX) $(FILE)) <(grep -n $(REGEX) $(FILE))
	-diff <(./s21_grep -c $(REGEX) $(FILE)) <(grep -c $(REGEX) $(FILE))
	-diff <(./s21_grep -l $(REGEX) $(FILE)) <(grep -l $(REGEX) $(FILE))
	-diff <(./s21_grep -v $(REGEX) $(FILE)) <(grep -v $(REGEX) $(FILE))
	-diff <(./s21_grep -i $(REGEX) $(FILE)) <(grep -i $(REGEX) $(FILE))
	-diff <(./s21_grep -e $(REGEX) $(FILE)) <(grep -e $(REGEX) $(FILE))
	-diff <(./s21_grep -e$(REGEX) $(FILE)) <(grep -e$(REGEX) $(FILE))
	-diff <(./s21_grep -s $(REGEX) $(FILE)) <(grep -s $(REGEX) $(FILE))
	-diff <(./s21_grep -e oo -e 77 $(FILE)) <(grep -e oo -e 77 $(FILE))
	-diff <(./s21_grep -h $(REGEX) $(FILE)) <(grep -h $(REGEX) $(FILE))

	-diff <(./s21_grep $(REGEX) $(FILE) $(FILE2)) <(grep $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -n $(REGEX) $(FILE) $(FILE2)) <(grep -n $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -c $(REGEX) $(FILE) $(FILE2)) <(grep -c $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -l $(REGEX) $(FILE) $(FILE2)) <(grep -l $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -v $(REGEX) $(FILE) $(FILE2)) <(grep -v $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -i $(REGEX) $(FILE) $(FILE2)) <(grep -i $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -e $(REGEX) $(FILE) $(FILE2)) <(grep -e $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -e$(REGEX) $(FILE) $(FILE2)) <(grep -e$(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -s $(REGEX) $(FILE) $(FILE2)) <(grep -s $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -e oo -e 77 $(FILE) $(FILE2)) <(grep -e oo -e 77 $(FILE) $(FILE2))
	-diff <(./s21_grep -h $(REGEX) $(FILE) $(FILE2)) <(grep -h $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -f $(REGFILE) $(FILE) $(FILE2)) <(grep -f $(REGFILE) $(FILE) $(FILE2))
	-diff <(./s21_grep -o a.b test_grep3.txt) <(grep -o a.b test_grep3.txt)
	-diff <(./s21_grep -o $(REGEX) $(FILE) $(FILE2)) <(grep -o $(REGEX) $(FILE) $(FILE2))


	-diff <(./s21_grep -vh $(REGEX) $(FILE) $(FILE2)) <(grep -vh $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -nh $(REGEX) $(FILE) $(FILE2)) <(grep -nh $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -ch $(REGEX) $(FILE) $(FILE2)) <(grep -ch $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -iv $(REGEX) $(FILE) $(FILE2)) <(grep -iv $(REGEX) $(FILE) $(FILE2))
	-diff <(./s21_grep -in $(REGEX) $(FILE) $(FILE2)) <(grep -in $(REGEX) $(FILE) $(FILE2))

	-diff <(./s21_grep -f $(REGFILE) -c $(FILE) $(FILE2)) <(grep -f $(REGFILE) -c $(FILE) $(FILE2))



clean:
	rm -rf s21_grep