/**
 * @file statistics.cpp
 * @author Mark Lagodych <lgd.mrk@outlook.com>
 * @date 2022-09-22
 * @brief Програма для обчислення елементарних статистичних характеристик.
 */

#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;


/** Вираховує довжину статичного масиву */
#define ARRLEN(STATIC_ARRAY) (sizeof(STATIC_ARRAY) / sizeof(*STATIC_ARRAY))


/** Числовий ряд, заданий у підручнику. :( */
const float predefined_data[] = {
    56.5, 27.9, 32.4, 59.23, 44.87, 41, 39.7, 49.98
};

const int predefined_length = ARRLEN(predefined_data);


/** Середнє значення */
float average(float array[], int length);

/** Стандартне відхилення */
float stddev(float array[], float average, int length);

/** Медіана */
float median(float sorted_array[], int length);

/** Повертає кількість однакових чисел у масиві, починаючи з start_index */
int count_repeated_number(float sorted_array[], int length, int start_index);

/** Мода
 * Якщо в масиві кілька значень повторюються найбільшу кількість разів,
 * повертається перше (найменше)
 */
float mode_single(float sorted_array[], int length);

/** Повертає інформацію про моди: кількість мод та кількість їх повторів */
void modes_info(
    float sorted_array[], int length, int& mode_count, int& mode_repeat_count
);

/** Знаходить моди масиву та записує їх у modes
 * mode_repeat_count - кількість повторів мод, можна отримати за допомогою
 * функції modes_info
 */
void modes(
    float sorted_data[], int data_length, float modes[], int mode_repeat_count
);


/** Виводить масив */
void print_array(float array[], int length);

/** Вводить масив */
void read_array(float array[], int length);

/** Копіює елементи масиву із source до destination */
void copy_array(const float source[], float destination[], int length);

/** Генерує масив псевдовипадкових чисел від 0 до 100 */
void generate_random_array(float array[], int length);

/** Виводить привітання */
void print_greeting();

/** Виводить завершальне слово */
void print_goodbye();

/** Вибори користувача щодо вхідних даних */
enum DataSourceChoice {
    CHOICE_INVALID = 0,         // Неправильний ввід
    CHOICE_USE_PREDEFINED = 1,  // Використовувати попередньо визначені дані 
    CHOICE_READ_INPUT = 2,      // Ввести дані
    CHOICE_GENERATE_RANDOM = 3, // Згенерувати дані
};

/** Вводить вибір джерела даних */
DataSourceChoice read_data_source_choice();

/** Визначає довжину даних */
int get_data_length(DataSourceChoice choice);

/** Формує дані */
void get_data(DataSourceChoice choice, float array[], int length);

/** Виводить детальну інформацію про дані */
void print_data(DataSourceChoice choice, float array[], int length);

/** Вираховує та демонструє статистичні характеристики */
void demo(float data[], int length);

/** Вираховує та демонструє множинну моду */
void demo_modes(float sorted_data[], int length);


int main() {
    print_greeting();

    DataSourceChoice choice = read_data_source_choice();

    if (choice == CHOICE_INVALID)
        return 1;

    int length = get_data_length(choice);

    float data[length];
    get_data(choice, data, length);

    print_data(choice, data, length);

    // Тут відбувається усе найцікавіше
    demo(data, length);

    print_goodbye();
    return 0;
}


void demo(float data[], int length) {
    sort(data, data+length);

    cout << endl;

    float average_number = average(data, length);
    cout << "Середнє: " << average_number << endl;

    cout << "Стандартне відхилення: "
         << stddev(data, average_number, length)
         << endl;

    cout << "Медіана: "
         << median(data, length)
         << endl;

    cout << "Мода (одинична): "
         << mode_single(data, length)
         << endl;

    demo_modes(data, length);
}


void demo_modes(float data[], int length) {
    // Кількість мод та їх повторів
    int mode_count, mode_repeat_count;
    modes_info(data, length, mode_count, mode_repeat_count);

    float modal_numbers[mode_count];
    modes(data, length, modal_numbers, mode_repeat_count);

    if (mode_count == length) {
        cout << "Кожне число повторюється лише один раз" << endl;
        return;
    }

    cout << "Моди (" << mode_count << ", "
         << mode_repeat_count << " повторів): ";
    print_array(modal_numbers, mode_count);
}


void print_greeting() {
    cout << "Вас вітає програма для обчислення елементарних характеристик "
         << "числових рядів" << endl;
}


void print_goodbye() {
    cout << endl << "На здоров'я!" << endl;
}


DataSourceChoice read_data_source_choice() {
    cout << endl
         << "1. Взяти дані з підручника" << endl
         << "2. Ввести дані" << endl
         << "3. Згенерувати випадкові дані" << endl
         << "Обраний варіант: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case CHOICE_USE_PREDEFINED:
        case CHOICE_READ_INPUT:
        case CHOICE_GENERATE_RANDOM:
            return static_cast<DataSourceChoice>(choice);

        default:
            return CHOICE_INVALID;
    }
}


int get_data_length(DataSourceChoice choice) {
    if (choice == CHOICE_USE_PREDEFINED)
        return predefined_length;

    int length;
    cout << endl << "Довжина числового ряду: ";
    cin >> length;

    return length;
}


void get_data(DataSourceChoice choice, float array[], int length) {
    switch (choice) {
        case CHOICE_USE_PREDEFINED:
            copy_array(predefined_data, array, length);
        break;

        case CHOICE_READ_INPUT:
            cout << "Числовий ряд:" << endl;
            read_array(array, length);
        break;

        case CHOICE_GENERATE_RANDOM:
            generate_random_array(array, length);
        break;

        default:
        break;
    }
}


void print_data(DataSourceChoice choice, float array[], int length) {
    // Якщо користувач тільки що ввів дані, йому не потрібно їх виводити
    if (choice == CHOICE_READ_INPUT)
        return;

    cout << endl << "Дано " << length << " чисел:" << endl;
    print_array(array, length);
}


void print_array(float array[], int length) {
    for (int i=0; i<length; i++)
        cout << array[i] << ' ';
    cout << endl;
}


void read_array(float array[], int length) {
    for (int i=0; i<length; i++)
        cin >> array[i];
}


void copy_array(const float src[], float dst[], int length) {
    for (int i=0; i<length; i++)
        dst[i] = src[i];
}


void generate_random_array(float array[], int length) {
    srand(time(NULL));

    for (int i=0; i<length; i++)
        array[i] = roundf(100.f * ((float) rand() / (float) RAND_MAX));
}


float average(float array[], int length) {
    float sum = 0.0;

    for (int i=0; i<length; i++)
        sum += array[i];

    return sum / length;
}


float stddev(float array[], float average, int length) {
    float tmp = 0.0;

    for (int i=0; i<length; i++) {
        float deviation = array[i] - average;
        tmp += deviation * deviation;
    }

    return sqrtf(tmp / length);
}


float median(float sorted_array[], int length) {
    if (length % 2 == 1) {
        return sorted_array[length / 2];
    }

    float a = sorted_array[length/2];   // Правіше медіани
    float b = sorted_array[length/2-1]; // Лівіше медіани
    return (a + b) / 2;
}


int count_repeated_number(float arr[], int length, int start_index) {
    float number = arr[start_index];
    int repeat_count = 0;

    for (int i=start_index; i<length && arr[i]==number; i++)
        repeat_count++;

    return repeat_count;
}


float mode_single(float arr[], int length) {
    float mode = 0;
    int mode_repeat_count = 0;

    for (int count, i=0; i<length; i+=count) {
        count = count_repeated_number(arr, length, i);
        
        if (count > mode_repeat_count) {
            mode = arr[i];
            mode_repeat_count = count;
        }
    }

    return mode;
}


void modes_info(
    float arr[], int length, int& mode_count, int& mode_repeat_count
) {
    mode_count = 0;
    mode_repeat_count = 0;

    for (int count, i=0; i<length; i+=count) {
        count = count_repeated_number(arr, length, i);

        if (count == mode_repeat_count) {
            mode_count++;
        }

        if (count > mode_repeat_count) {
            mode_count = 1;
            mode_repeat_count = count;
        }
    }
}


void modes(
    float arr[], int length, float modes[], int mode_repeats
) {
    int mode_index = 0;

    for (int count, i=0; i<length; i+=count) {
        count = count_repeated_number(arr, length, i);

        if (count == mode_repeats)
            modes[mode_index++] = arr[i];
    }
}