#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int n, b, s, e, v, t, T, E, er;
} Flags;

void parse_falgs(int argc, char *argv[], Flags *options);
void output(FILE *ptrFile, Flags *options);
void opt_v(int ch);

int main(int argc, char *argv[]) {
  Flags options;
  parse_falgs(argc, argv, &options);

  char *fileNAME = argv[argc - 1];

  FILE *ptrFile = fopen(fileNAME, "rb");
  if (ptrFile == NULL)
    perror("Ошибка открытия файла");
  else {
    output(ptrFile, &options);
    fclose(ptrFile);
  }
  return 0;
}

void parse_falgs(int argc, char *argv[], Flags *options) {
  memset(options, 0, sizeof(Flags));  // Инициализирую все флаги нулями
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number-nonblank") == 0)
        options->b = 1;
      else if (strcmp(argv[i], "--number") == 0)
        options->n = 1;
      else if (strcmp(argv[i], "--squeeze-blank") == 0)
        options->s = 1;
      else {
        options->er = 1;
        printf("Неизвестный флаг: %s\n", argv[i]);
      }
    } else if (argv[i][0] == '-') {  // Проверка на флаг - если '-', то флаг
      int len = strlen(argv[i]);  // Беру длину строки аргумента
      for (int j = 1; j < len;
           j++) {  // Прохожу циклом по каждому символу аргумента
        switch (argv[i][j]) {  // Проставляю 1 если флаг есть в аргументах
          case 'n':
            options->n = 1;
            break;
          case 'b':
            options->b = 1;
            break;
          case 's':
            options->s = 1;
            break;
          case 'e':
            options->v = 1;
            options->E = 1;
            break;
          case 'v':
            options->v = 1;
            break;
          case 't':
            options->v = 1;
            options->T = 1;
            break;
          case 'T':
            options->T = 1;
            break;
          case 'E':
            options->E = 1;
            break;
          default:
            options->er = 1;
            printf("Неизвестный флаг: %c\n", argv[i][1]);
            break;
        }
      }
    }
  }
  if (options->er == 1) exit(EXIT_FAILURE);
}

void output(FILE *ptrFile, Flags *options) {
  int line_start = 1;  // Находимся в начале строки? - 1 или 0
  int current_str_empty = 1;  // Текущая строка пустая? - 1 или 0
  int last_str_empty = 0;  // Последняя строка пустая? - 1 или 0

  int ch;
  int count = 0;

  while ((ch = fgetc(ptrFile)) != EOF) {
    if (options->s) {
      if (line_start == 1) {  // Если находимся в начале строки
        if (last_str_empty == 1 &&
            ch == '\n') {  // Если случился перевод строки и последняя строка
                           // пустая
          continue;  // Сбрасываем цикл - пропускаем строку
        } else
          last_str_empty = 0;
      }
    }

    if (options->b) {
      if (line_start == 1) {  // Если находимся в начале строки
        if (ch != '\n') {  // Если первый символ не перенос строки
          count++;  // увеличиваем счетчик на один
          printf("%6d\t", count);  // нумеруем строку
        }
        line_start = 0;  // Опускаем флаг начала строки
      }
    }

    if (options->n && !options->b) {
      if (line_start == 1) {  // Если находимся в начале строки
        count++;  // увеличиваем счетчик на один
        printf("%6d\t", count);  // нумеруем строку
      }
    }

    if (ch == '\n' && options->E)
      printf("$\n");

    else if (options->T && ch == '\t') {
      printf("^I");
    }

    else if (options->v) {
      opt_v(ch);
    }

    else
      putchar(ch);  // Выводим символ. Работает с флагами и без
    line_start = 0;

    if (ch == '\n') {  // По окончании строки
      last_str_empty = current_str_empty;  // Запоминаем состояние текущей
                                           // строки в последнюю строку
      current_str_empty = 1;  // Говорим, что текущая строка пустая
      line_start = 1;  // Поднимаем флаг начала строки

    } else if (ch != ' ' &&
               ch != '\t') {  // Если символ не пробел и не табуляция
      current_str_empty = 0;  // Строка не пустая
    }
  }
}

void opt_v(int ch) {
  if (ch >= 0 && ch < 32 && ch != '\n' && ch != '\t')
    printf("^%c", ch + 64);  // ^A (1) → 'A' (65)
  else if (ch == 127)
    printf("^?");
  else if (ch >= 128 && ch <= 255)
    printf("M-%c", (ch & 0x7F));
  else
    putchar(ch);
}
