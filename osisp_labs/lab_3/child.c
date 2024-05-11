//программа, которая читает и выводит переменные окружения из файла, в зависимости от переданного аргумента командной строки.
#define _DEFAULT_SOURCE
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char** environ;

char* find(char* name, char** envir) { // ищет переменную окружения по имени
    char* result = NULL;
    for(int i = 0; envir[i]; i++) {  //ищет переменную окружения по имени name в массиве envir
        if(strncmp(name, envir[i], strlen(name)) == 0) {// если переменная найдена, то   
            char* buf = NULL;
            buf = (char*)malloc(sizeof(char) * strlen(envir[i]));
            strcpy(buf, envir[i]);  //она копируется,

            result = strtok(envir[i], "=");

            strcpy(envir[i], buf);  //разбивается на имя и значение,
            free(buf);

            result = strtok(NULL, "\0"); 
        }
    }
    return result; //и возвращается значение
 }

int main(int argc, char** argv, char** env) { //объявление переменных и открытие файла с переменными окружения
    FILE* environment_variables; // для чтения переменных окружения их файла
    char* envFileName = "ENV_VAR"; // файл, где хранятся переменные окружения
    char* filepath = NULL;  // путь к файлу
    char type = argv[1][0];

    if(argc < 2)  //проверяется количество аргументов командной строки
    {
        puts("Invalid child args");
        exit(EXIT_FAILURE);
    }

    puts(argv[0]);  // получается тип операции из первого аргумента
    printf("%i\n%i\n", (int)getpid(), (int)getppid());

    //в зависимости от типа операции (+, *, &), определяется источник переменных окружения:
    filepath = type == '+' ? getenv(envFileName) : // использование ф-ций execve() и fork()
            type == '*' ? find(envFileName, env) : // из аргуметов коммандной строки
            type == '&' ? find(envFileName, environ) : NULL; // из массива переменных окружения

    if(filepath == NULL){  // если путь к файлу не найден - сообщение об ошибке и выход из программы
        puts("Getting by type error");
        exit(EXIT_FAILURE);
    }

    environment_variables = fopen(filepath, "r"); 
    if(environment_variables != NULL)
    {
        // вывод и чтение переменных окружения
        char str[512];
        while(EOF != fscanf(environment_variables, "%s\n", str)){
            if(getenv(str))
                printf("%s=%s\n", str, getenv(str));
            else
                continue;
        }
        fclose(environment_variables);
    }
    else
        perror("Environment variables file");

    exit(EXIT_SUCCESS);
    return 0;
}