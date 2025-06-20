#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int e, i, v, c, l, n, h, s, o, f;
  char pattern[1024];
  int cnt_ptrn;
  char *fileNAME[256];
  int cnt_fNM;
  char *fileREG;
} Flags;

void parse_flags(int argc, char *argv[], Flags *flg);
int proc_file(Flags *flg, int ext);
void reg_from_file(Flags *flg);
void add_ptrn(Flags *flg, char *pattern);
void add_fileNM(Flags *flg, char *fileNAME);
void output(FILE *ptrFile, Flags *flg, char *current_fNM);
void option_o(regex_t *regex, char *line, Flags *flg, char *current_fNM,
              int num_line);

int main(int argc, char *argv[]) {
  int ext = 0;
  Flags flg = {0};

  parse_flags(argc, argv, &flg);

  proc_file(&flg, ext);

  if (ext == 1)
    return 1;
  else
    return 0;
}

void parse_flags(int argc, char *argv[], Flags *flg) {
  for (int i = 1; i < argc;
       i++) {  // Обработка случая слитного написания флага -е и паттерна
    if (argv[i][0] == '-' && strncmp(argv[i], "-e", 2) == 0 &&
        argv[i][2] != '\0') {
      add_ptrn(flg,
               argv[i] + 2);  // Записываю в паттерн симовл идущий после флага
      flg->e = 1;
    }
  }

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strncmp(argv[i], "-f", 2) == 0 &&
        argv[i][3] != '\0') {
      flg->fileREG = argv[i] + 3;
    }
  }

  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsof")) !=
         -1) {  // Обработка обычного написнаия флагов
    switch (opt) {
      case 'e':
        flg->e = 1;
        add_ptrn(flg, optarg);  // Вызываю функцию добавления паттерна передаю в
                                // нее структуру и указатель на аргумент опции
                                // командной строки
        break;
      case 'i':
        flg->i =
            REG_ICASE;  // Присваиваю флагу -i флаг компиляции
                        // регулярноговыражения для регистронезависимого поиска
        break;
      case 'v':
        flg->v = 1;
        break;
      case 'c':
        flg->c = 1;
        break;
      case 'l':
        flg->l = 1;
        break;
      case 'n':
        flg->n = 1;
        break;
      case 'h':
        flg->h = 1;
        break;
      case 's':
        flg->s = 1;
        break;
      case 'o':
        flg->o = 1;
        break;
      case 'f':
        flg->f = 1;
        reg_from_file(flg);
        break;
    }
  }
  if (flg->pattern[0] == '\0' &&
      optind < argc) {  // Если pattern==NULL опций не было и есть
                        // необработанные аргументы в командной строке
    strcpy(flg->pattern,
           argv[optind]);  // Копирую в массив pattern текущий аргумент
    optind++;  // Увеличиваю идекс для получения следующего аргумента
  }

  if (optind < argc) {
    if (flg->f) {
      optind += 1;
      while (optind < argc) {
        add_fileNM(flg, argv[optind]);
        optind++;
      }
    } else {
      while (optind < argc) {
        add_fileNM(flg, argv[optind]);
        optind++;
      }
    }
  }
}

int proc_file(Flags *flg, int ext) {
  for (int i = 0; i < flg->cnt_fNM; i++) {
    FILE *ptrFile = fopen(flg->fileNAME[i], "r");
    if (ptrFile == NULL)
      if (flg->s)
        ext = 1;
      else {
        ext = 1;
        fprintf(stderr, "s21_grep: %s: %s\n", flg->fileNAME[i],
                strerror(errno));
      }
    else {
      output(ptrFile, flg, flg->fileNAME[i]);
      fclose(ptrFile);
    }
  }
  return ext;
}

void reg_from_file(Flags *flg) {
  FILE *ptrFile = fopen(flg->fileREG, "r");
  if (ptrFile == NULL) {
    if (!flg->s) perror(flg->fileREG);
    exit(1);
  }
  char *line = NULL;
  size_t len = 0;
  int read;
  while ((read = getline(&line, &len, ptrFile)) != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    add_ptrn(flg, line);
  }

  free(line);
  fclose(ptrFile);
}

void add_ptrn(Flags *flg, char *pattern) {
  if (flg->cnt_ptrn != 0) {
    strcat(flg->pattern + flg->cnt_ptrn, "|");
    flg->cnt_ptrn++;
  }
  flg->cnt_ptrn += sprintf(flg->pattern + flg->cnt_ptrn, "(%s)", pattern);
}

void add_fileNM(Flags *flg, char *fileNAME) {
  if (flg->cnt_fNM < 256) {
    flg->fileNAME[flg->cnt_fNM] = fileNAME;
    flg->cnt_fNM++;
  }
}

void output(FILE *ptrFile, Flags *flg, char *current_fNM) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  regex_t regex;
  int num_line = 0;

  int reti =
      regcomp(&regex, flg->pattern,
              REG_EXTENDED |
                  flg->i);  // Добавил REG_EXTENDED чтобы не экранировать () и
                            // при конкатенации строк или использование флага i
  if (reti) {
    printf("Could not compile regex\n");
    exit(1);
  }

  while ((read = getline(&line, &len, ptrFile)) != -1) {
    int cont_flag = 0;
    reti = regexec(&regex, line, 0, NULL, 0);

    if (flg->o && flg->v) cont_flag = 1;

    if (flg->n && !flg->c && !flg->l &&
        !flg->o) {  // Если флаг -n при совпадении вывожу номер строки и строку,
                    // без совпадения пропускаю и увеличиваю счетчик
      num_line++;
      if ((flg->v && reti) || (!flg->v && !reti)) {
        if (flg->cnt_fNM > 1 && flg->h) {
          printf("%d:%s", num_line, line);
        } else if (flg->cnt_fNM > 1) {
          printf("%s:%d:%s", current_fNM, num_line, line);
        } else {
          printf("%d:%s", num_line, line);
        }
      }
      cont_flag = 1;
    }

    if (flg->c && !flg->l) {  // Если флаг -с, при совпадении увеличиваю счетчик
                              // строк на 1. В конце фукции вывожу результат
      if (flg->v) {
        if (reti == REG_NOMATCH) num_line++;
      } else {
        if (!reti) num_line++;
      }
      cont_flag = 1;
    }

    if (flg->l) {  // Если флаг -l, считаю строки
      if (!reti) {
        num_line++;
      } else if (reti)
        num_line++;
      cont_flag = 1;
    }

    if (flg->v && !flg->o && !flg->c && !flg->l &&
        !flg->n) {  // Если флаг -v, вывожу не совпадающие строки
      if (reti) {
        if (flg->cnt_fNM > 1 && flg->h) {
          printf("%s", line);
        } else if (flg->cnt_fNM > 1) {
          printf("%s:%s", current_fNM, line);
        } else
          printf("%s", line);
      }
      cont_flag = 1;
    }
    if (flg->o && !flg->v && !flg->c && !flg->l) {
      num_line++;
      option_o(&regex, line, flg, current_fNM, num_line);
    }

    else if (reti == REG_NOMATCH)
      cont_flag = 1;  // Если нет совпадений - запускаю следующий шаг цикла
    else if (cont_flag)
      continue;
    else {
      if (flg->cnt_fNM > 1 && flg->h) {
        printf("%s", line);
      } else if (flg->cnt_fNM > 1) {
        printf("%s:%s", current_fNM, line);
      } else
        printf("%s", line);  // Если есть совпадение - вывожу строку.
    }
  }

  if (flg->c && !flg->l) {  // Вывод результата при флаге -с
    if (flg->cnt_fNM > 1 && flg->h) {
      printf("%d\n", num_line);
    } else if (flg->cnt_fNM > 1) {
      printf("%s:%d\n", current_fNM, num_line);
    } else
      printf("%d\n", num_line);
  }
  if (flg->l) {
    if (num_line > 0) printf("%s\n", current_fNM);
  }
  regfree(&regex);
  free(line);
}

void option_o(regex_t *regex, char *line, Flags *flg, char *current_fNM,
              int num_line) {
  regmatch_t mtch;
  int offset = 0;
  while (1) {
    int reti = regexec(regex, line + offset, 1, &mtch, 0);
    if (reti != 0) break;

    if (flg->cnt_fNM > 1 && !flg->h) {
      printf("%s:", current_fNM);
    }

    if (flg->n) printf("%d:", num_line);

    for (int i = mtch.rm_so; i < mtch.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += mtch.rm_eo;
  }
}