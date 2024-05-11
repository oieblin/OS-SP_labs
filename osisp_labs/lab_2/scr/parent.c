#define _DEFAULT_SOURCE
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char** environ;

 void init_child(char* child_path, char type) { // инициализация нового дочернего процесса
    static int child_counter = 0;               // счётчик процессов
    char* child_argv[3] = {(char*)0};

    if(child_path == NULL) {
        puts("Child's path is empty.");
        return;
    }

    if(child_counter > 99) {
        puts("Too many child streams.");
        return;
    }
    
    pid_t pid;
    switch (pid = fork()) {      // создание нового дочернего процесса

        case -1:
            perror("Fork");
            exit(EXIT_FAILURE);
        break;

        case 0:
            child_argv[0] = (char*)malloc(10 * sizeof(char));             // В дочернем процессе функция выделяет память 
                                                                               //для аргументов командной строки и устанавливает 
                                                                               //их в имя дочернего процесса и указанный тип
            child_argv[1] = (char*)malloc(1 * sizeof(char));

            if(child_counter < 10) {
                sprintf(child_argv[0], "child_0%d", child_counter);
            } else {
                sprintf(child_argv[0], "child_%d", child_counter);
            }
            sprintf(child_argv[1], "%c", type);

        
            if (execve(child_path, child_argv, environ) == -1) {   //для замены образа дочернего процесса 
                                                                                //указанным исполняемым файлом
                perror("Execve");                                              //+ обработка ошибки
                exit(1);
            }
            break;

        default:
            child_counter++;
            break;
    }
    return;
 }

 char* find(char* name, char** envir) {                          //Функция find ищет определенную переменную 
    char* result = NULL;                                         //окружения в массиве environ и возвращает 
    for(int i = 0; envir[i]; i++) {                              //ее значение. Она принимает два параметра: 
                                                                 //name (имя переменной окружения для поиска) и
        if(strncmp(name, envir[i], strlen(name)) == 0) { // envir (массив переменных окружения)
            char* buf = NULL;
            buf = (char*)malloc(sizeof(char) * strlen(envir[i]));
            strcpy(buf, envir[i]);

            result = strtok(envir[i], "=");

            strcpy(envir[i], buf);
            free(buf);                     //Функция перебирает массив envir и проверяет, начинается ли текущий элемент 
                           //с указанного name. Если да, функция извлекает значение переменной окружения и возвращает её значение
            result = strtok(NULL, "\0");
        }
    }
    return result;
 }

 void sorting(char** environ) {              //сортирует массив environ в порядке возрастания на основе правил сравнения локали
    for(int i = 0; environ[i + 1]; i++) {
        for(int j = 0; environ[j + 1]; j++) {
            if(strcoll(environ[j], environ[j + 1]) > 0) { //для сравнения соседних элементов в массиве environ
                char* buf = environ[j + 1];
                environ[j + 1] = environ[j];
                environ[j] = buf;
            }
        }
    }
    return;
 }

int main(int argc, char** argv, char** envir) {   //устанавливает переменную окружения "ENV_VAR"
    if (argc < 2) {
        puts("Environment file path empty");
        exit(EXIT_FAILURE);
    }
    char childPathName[] = "CHILD_PATH";
    char *path = NULL,
         *environPath = NULL,
         *envirPath = NULL;
    setlocale(LC_COLLATE, "C");
    setenv("ENV_VAR", argv[1], 1);
    
    sorting(environ);
    for(int i = 0; environ[i]; i++) {
        puts(environ[i]);
    }
    // для хранения переменных окружения
    path = getenv(childPathName);
    environPath = find(childPathName, environ);
    envirPath = find(childPathName, envir);

    while (1) {
        switch (getchar()) {
            case '+':
                init_child(path, '+'); //возвращает значение переменной окружения "CHILD_PATH" и сохраняет его 
                //в переменную path
                break;
//ищет значение переменной окружения "CHILD_PATH" в массиве environ и сохраняет его в переменную environPath.
            case '*':
                init_child(envirPath, '*');
                break;
//оторая ищет значение переменной окружения "CHILD_PATH" в массиве envir (который, вероятно, является копией environ) и сохраняет его
//               в переменную envirPath
            case '&':
                init_child(environPath, '&');
                break;

            case 'q':
                return 0;
                break;

            default:
                break;
        }
    }

    return 0;
}



/*  fork():

    Создает новый дочерний процесс, который является точной копией вызывающего (родительского) процесса.
    В родительском процессе fork() возвращает идентификатор дочернего процесса (PID).
    В дочернем процессе fork() возвращает 0.
    Это позволяет отличить родительский и дочерний процессы в дальнейшем коде.

    getenv():

    Позволяет получить значение переменной окружения по ее имени.
    Переменные окружения - это именованные объекты, содержащие текстовую информацию, доступную всем запускаемым программам.
    Они используются для хранения параметров, настроек, ключей и другой информации.

        execve():
        Заменяет образ текущего процесса на новый, указанный в аргументах.
        Принимает путь к исполняемому файлу, аргументы командной строки и массив переменных окружения.
        Позволяет запустить другую программу, сохраняя контекст текущего процесса (открытые файлы, сигналы и т.д.).
    getpid() и getppid():
        getpid() возвращает идентификатор (PID) текущего процесса.
        getppid() возвращает идентификатор (PID) родительского процесса текущего процесса.
        Эти функции позволяют процессам получать информацию о своем окружении и иерархии.

*/