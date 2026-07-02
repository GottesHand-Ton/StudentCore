#include "StudentCore.h"
#include <iostream>

// создание массива студентов
Student* createGroup(int size) {
    if (size <= 0) return nullptr; // защита от пустой группы

    //с помощью new я прошу выделить памяти сколько в переменной студент записано
    Student* group = new Student[size];

    for (int i = 0; i < size; i++) {
        group[i].id = 0;
        group[i].fullName[0] = '\0'; 
        group[i].averageScore = 0.0;
        for (int j = 0; j < 5; j++) {
            group[i].scores[j] = 0;
        }
    }
    return group;
}

// ввод демо-данных
void initDemoData(Student* group, int size) {
    if (!group || size <= 0) return; 

    const char* names[] = {
        "Иванов Иван Иванович", "Петров Петр Петрович",
        "Сидоров Сидор Сидорович", "Смирнова Анна Алексеевна",
        "Кузнецов Олег Игоревич"
    };

    for (int i = 0; i < size; i++) {
        group[i].id = i + 1;
        strcpy_s(group[i].fullName, names[i % 5]);

        for (int j = 0; j < 5; j++) {
            group[i].scores[j] = 3 + (i + j) % 3;
        }


        if (i % 3 == 0) {
            group[i].scores[0] = 2;
        }

        group[i].averageScore = 0.0; //зануляю средний бал что б мусора не было 
    }
}

// Средний балл
void calculateAllAverages(Student* group, int size) {
    if (!group || size <= 0) return;

    for (int i = 0; i < size; i++) {
        double sum = 0;
        for (int j = 0; j < 5; j++) {
            sum += group[i].scores[j];
        }
        group[i].averageScore = sum / 5.0;
    }
}

// 4. Поиск лучшего студента
int findBestStudent(const Student* group, int size) {
    if (!group || size <= 0) return -1;

    double maxScore = group[0].averageScore;
    int bestId = group[0].id;

    for (int i = 1; i < size; i++) {
        if (group[i].averageScore > maxScore) {
            maxScore = group[i].averageScore;
            bestId = group[i].id;
        }
    }
    return bestId;
}

// 5. Поиск студентов с задолженностями
int countDebtors(const Student* group, int size) {
    if (!group || size <= 0) return 0;

    int debtorCount = 0;
    for (int i = 0; i < size; i++) {
        bool hasBadGrade = false;
        for (int j = 0; j < 5; j++) {
            if (group[i].scores[j] < 3) {
                hasBadGrade = true;
                break;
            }
        }
        if (hasBadGrade) {
            debtorCount++;
        }
    }
    return debtorCount;
}

// 6. Фильтрация по среднему баллу
Student* filterByAverage(const Student* group, int size, double threshold, int* outSize) {
    if (!group || size <= 0 || !outSize) {
        if (outSize) *outSize = 0;
        return nullptr;
    }
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (group[i].averageScore >= threshold) {
            count++;
        }
    }
    *outSize = count; 

    if (count == 0) {
        return nullptr;
    }

    Student* filteredGroup = new Student[count];
    int index = 0;

    // Копируем данные подходящих студентов
    for (int i = 0; i < size; i++) {
        if (group[i].averageScore >= threshold) {
            filteredGroup[index] = group[i];
            index++;
        }
    }

    return filteredGroup;
}

// 7. Сортировка по среднему баллу
void sortByAverage(Student* group, int size) {
    if (!group || size <= 0) return;

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (group[j].averageScore < group[j + 1].averageScore) {
                
                Student temp = group[j];
                group[j] = group[j + 1];
                group[j + 1] = temp;
            }
        }
    }
}

// 8. Освобождение памяти
void freeGroup(Student* group) {
    if (group) {
        delete[] group;
    }
}