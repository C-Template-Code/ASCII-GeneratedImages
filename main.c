#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINES 1000 // Максимальное количество строк в файле

int main() {

   const char *file_path = "monroe.txt";
    int delay = 100900;

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        return 1;
    }

    initscr(); // Инициализация ncurses
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);      // Красный
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // Желтый
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);  // Magenta
    init_pair(4, COLOR_WHITE, COLOR_BLACK);    // Белый
    init_pair(5, COLOR_BLUE, COLOR_BLACK);     // Синий
    init_pair(7, COLOR_CYAN, COLOR_BLACK);     // Синий

    int ch;
    int red_shade = 0; // Индекс текущего оттенка красного
    printf("%s", "Create buffer. Render symbol on GPU default...");
    printf("%s", "Read GPU memory ....");

    // Определение количества строк в файле
    int num_lines = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            num_lines++;
        }
    }
    rewind(file);

    srand(time(NULL));

    int console_offset = 0; // Смещение текущей строки в консоли
    int file_offset = 0;    // Смещение текущей строки в файле

    // Массив для отслеживания уже выведенных строк
    int *lines_displayed = (int *)malloc(num_lines * sizeof(int));
    for (int i = 0; i < num_lines; i++) {
        lines_displayed[i] = 0;
    }

    // Включаем режим nodelay для getch(), чтобы отключить ожидание ввода
    nodelay(stdscr, TRUE);

    while (1) {
        // Выбираем случайное число строк
        int random_lines = rand() % num_lines;

        // Проверяем, была ли уже данная строка выведена
        while (lines_displayed[random_lines]) {
            random_lines = rand() % num_lines;
        }

        // Отмечаем строку как выведенную
        lines_displayed[random_lines] = 1;

        // Пересчитываем смещение в консоли и в файле
        console_offset = random_lines;
        file_offset = random_lines;

        // Перемещаем курсор в нужную позицию в консоли
        move(console_offset, 0);

        // Смещаемся в файле к нужной строке
        for (int i = 0; i < file_offset; i++) {
            while ((ch = fgetc(file)) != '\n') {
                if (ch == EOF) {
                    // Вернуться в начало файла, если достигнут конец
                    rewind(file);
                }
            }
        }

        // Считываем и выводим строку из файла
        while (1) {
            ch = fgetc(file);

            if (ch == '\n' || ch == EOF) {
                // Вернуться в начало файла, если достигнут конец строки или конец файла
                rewind(file);
                break;
            }

            if (ch == '8') {
                attron(COLOR_PAIR(2)); // Желтый для символа "8"
            } else if (ch == ':') {
                attron(COLOR_PAIR(4)); // Белый для символа ":"
            } else if (ch == 'M') {
                attron(COLOR_PAIR(7)); // Желтый для символа "M"
            } else if (ch == '*' || ch == 'о') {
                attron(COLOR_PAIR(5)); // Синий для символов "*" и "о"
            } else if (ch == '@' || ch == 'а') {
                attron(COLOR_PAIR(1)); // Красный
            }else if (ch == 'O'){
                attron(COLOR_PAIR(5)); // CИний
            } else {
                attron(COLOR_PAIR(4)); // Белый (по умолчанию)
            }

            addch(ch);
            refresh();
            usleep(delay);

            // Выключение цвета после символа
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));

            red_shade = (red_shade + 1) % 8; // Переключение к следующему оттенку красного (8 оттенков)
        }

        // Проверка на нажатие клавиши 'q' для выхода из программы
        ch = getch();
        if (ch == 'q') {
            break;
        }
}
    fclose(file);
    endwin(); // Завершение работы с ncurses
    free(lines_displayed); // Освобождаем память, выделенную для массива lines_displayed
    return 0;
}

