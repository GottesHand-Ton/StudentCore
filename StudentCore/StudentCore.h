#ifndef STUDENT_CORE_H
#define STUDENT_CORE_H
#include <cstring>

struct Student {
    int id;                 // уникальный номер (1...N)
    char fullName[100];     // ФИО
    int scores[5];          // оценки по 5 предметам (2-5)
    double averageScore;    // средний балл
};

// Создание массива студентов
Student* createGroup(int size);

// Инициализация демо-данными (ручной ввод или генерация)
void initDemoData(Student* group, int size);

// Расчет среднего балла для каждого студента
void calculateAllAverages(Student* group, int size);

// Поиск лучшего студента (по среднему баллу)
int findBestStudent(const Student* group, int size); // возвращает id

// Поиск студентов с задолженностями (оценка < 3 хотя бы по одному предмету)
int countDebtors(const Student* group, int size);    // возвращает количество

// Фильтрация: создает новый массив студентов с баллом >= threshold
Student* filterByAverage(const Student* group, int size, double threshold, int* outSize);

// Сортировка студентов по среднему баллу (по убыванию)
void sortByAverage(Student* group, int size);

// Освобождение памяти
void freeGroup(Student* group);

#endif 