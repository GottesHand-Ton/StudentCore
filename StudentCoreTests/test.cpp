#include "gtest/gtest.h"
#include "StudentCore.h"
#include <cstring>

// Проверка расчета среднего балла
TEST(StudentCoreTest, AverageCalculation) {
    const int size = 1;
    Student* group = createGroup(size);

    group[0].scores[0] = 5;
    group[0].scores[1] = 4;
    group[0].scores[2] = 5;
    group[0].scores[3] = 4;
    group[0].scores[4] = 2; 
    // средний должен быть 4.0

    calculateAllAverages(group, size);

    // макрос для точной проверки double
    EXPECT_DOUBLE_EQ(group[0].averageScore, 4.0);
    freeGroup(group);
}

// Проверка сортировки по убыванию среднего балла 
TEST(StudentCoreTest, SortByAverageCalculation) {
    const int size = 3;
    Student* group = createGroup(size);

    if (group != nullptr) {
        group[0].id = 1; group[0].averageScore = 3.4;
        group[1].id = 2; group[1].averageScore = 4.9; // Этот должен стать первым
        group[2].id = 3; group[2].averageScore = 4.1; // Этот должен стать вторым

        sortByAverage(group, size);

    //макрос проверябщий строгое равенство
        EXPECT_EQ(group[0].id, 2);
        EXPECT_DOUBLE_EQ(group[0].averageScore, 4.9);

        EXPECT_EQ(group[1].id, 3);
        EXPECT_DOUBLE_EQ(group[1].averageScore, 4.1);

        EXPECT_EQ(group[2].id, 1);
        EXPECT_DOUBLE_EQ(group[2].averageScore, 3.4);
    }

    freeGroup(group);
}

// Подсчет должников
TEST(StudentCoreTest, DebtorsCount) {
    const int size = 2;
    Student* group = createGroup(size);

    for (int j = 0; j < 5; j++) group[0].scores[j] = 4;
    group[1].scores[0] = 5;
    group[1].scores[1] = 2;
    group[1].scores[2] = 4;
    group[1].scores[3] = 3;
    group[1].scores[4] = 4;

    int debtors = countDebtors(group, size);
    EXPECT_EQ(debtors, 1);

    freeGroup(group);
}

// Безопасная обработка nullptr 
TEST(StudentCoreTest, NullPointerHandling) {

    EXPECT_EQ(findBestStudent(nullptr, 5), -1);
    EXPECT_EQ(countDebtors(nullptr, 5), 0);
    EXPECT_EQ(filterByAverage(nullptr, 5, 4.0, nullptr), nullptr);

    calculateAllAverages(nullptr, 5);
    sortByAverage(nullptr, 5);
    freeGroup(nullptr);
}