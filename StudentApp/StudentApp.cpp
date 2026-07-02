#include <iostream>
#include <windows.h>
#include "../StudentCore/StudentCore.h"

using namespace std;

typedef Student* (*CreateGroupFunc)(int);
typedef void (*InitDemoDataFunc)(Student*, int);
typedef void (*CalculateAllAveragesFunc)(Student*, int);
typedef int (*FindBestStudentFunc)(const Student*, int);
typedef int (*CountDebtorsFunc)(const Student*, int);
typedef Student* (*FilterByAverageFunc)(const Student*, int, double, int*);
typedef void (*SortByAverageFunc)(Student*, int);
typedef void (*FreeGroupFunc)(Student*);

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HMODULE hDll = NULL;          // Дескриптор
    Student* currentGroup = nullptr; 
    int groupSize = 0;            

    // зануляем
    CreateGroupFunc dllCreateGroup = nullptr;
    InitDemoDataFunc dllInitDemoData = nullptr;
    CalculateAllAveragesFunc dllCalculateAllAverages = nullptr;
    FindBestStudentFunc dllFindBestStudent = nullptr;
    CountDebtorsFunc dllCountDebtors = nullptr;
    FilterByAverageFunc dllFilterByAverage = nullptr;
    SortByAverageFunc dllSortByAverage = nullptr;
    FreeGroupFunc dllFreeGroup = nullptr;

    int choice = -1;

    while (choice != 0) {
        cout << "=== СИСТЕМА УПРАВЛЕНИЯ СТУДЕНТАМИ (DLL) ===\n";
        cout << "Статус DLL: " << (hDll ? "ЗАГРУЖЕНА" : "НЕ ЗАГРУЖЕНА") << "\n";
        cout << "Студентов в памяти: " << groupSize << "\n";
        cout << "---------------------------------------------\n";
        cout << "1. Загрузить DLL\n";
        cout << "2. Создать группу студентов (ввод размера)\n";
        cout << "3. Заполнить демо-данными\n";
        cout << "4. Показать всех студентов\n";
        cout << "5. Рассчитать средние баллы\n";
        cout << "6. Найти лучшего студента\n";
        cout << "7. Показать количество должников\n";
        cout << "8. Отфильтровать по баллу (>= 4.0)\n";
        cout << "9. Отсортировать по среднему баллу\n";
        cout << "10. Выгрузить DLL\n";
        cout << "0. Выход\n";
        cout << "---------------------------------------------\n";
        cout << "Выберите пункт меню: ";
        cin >> choice;

        // защита если библиотека не загружена
        if (choice > 1 && choice < 10 && hDll == NULL) {
            cout << "\nДействие невозможно! Сначала загрузите DLL (Пункт 1).\n";
            continue;
        }

        switch (choice) {
        case 1:
            if (hDll != NULL) {
                cout << "\nБиблиотека уже находится в памяти устройства.\n";
                break;
            }
           
            hDll = LoadLibrary(L"StudentDLL.dll");
            if (hDll == NULL) {
                cout << "\nНе удалось загрузить StudentDLL.dll!\n";
                cout << "Убедитесь, что файл .dll лежит в одной папке с этой программой.\n";
            }
            else {
                cout << "\nДинамическая библиотека успешно загружена!\n";

                // Связываем указатели с адресами функций внутри DLL с помощью GetProcAddress
                dllCreateGroup = (CreateGroupFunc)GetProcAddress(hDll, "createGroup");
                dllInitDemoData = (InitDemoDataFunc)GetProcAddress(hDll, "initDemoData");
                dllCalculateAllAverages = (CalculateAllAveragesFunc)GetProcAddress(hDll, "calculateAllAverages");
                dllFindBestStudent = (FindBestStudentFunc)GetProcAddress(hDll, "findBestStudent");
                dllCountDebtors = (CountDebtorsFunc)GetProcAddress(hDll, "countDebtors");
                dllFilterByAverage = (FilterByAverageFunc)GetProcAddress(hDll, "filterByAverage");
                dllSortByAverage = (SortByAverageFunc)GetProcAddress(hDll, "sortByAverage");
                dllFreeGroup = (FreeGroupFunc)GetProcAddress(hDll, "freeGroup");
            }
            break;

        case 2: // Выделение памяти под группу
            if (currentGroup != nullptr) {
                dllFreeGroup(currentGroup);
                currentGroup = nullptr;
                groupSize = 0;
            }
            cout << "Введите количество студентов в группе: ";
            cin >> groupSize;
            currentGroup = dllCreateGroup(groupSize);
            if (currentGroup != nullptr) {
                cout << "Выделена динамическая память под массив из " << groupSize << " структур.\n";
            }
            break;

        case 3: // Заполнение демо-данными
            if (currentGroup == nullptr) {
                cout << "Сначала создайте группу (Пункт 2)!\n";
            }
            else {
                dllInitDemoData(currentGroup, groupSize);
                cout << "Массив заполнен стандартными студенческими данными.\n";
            }
            break;

        case 4: // Вывод списка на экран
            if (currentGroup == nullptr) {
                cout << "Массив студентов пуст!\n";
            }
            else {
                cout << "\n--- ТЕКУЩИЙ СПИСОК ГРУППЫ ---\n";
                for (int i = 0; i < groupSize; i++) {
                    cout << "ID: " << currentGroup[i].id << " | ФИО: " << currentGroup[i].fullName
                        << " | Оценки: ";
                    for (int j = 0; j < 5; j++) cout << currentGroup[i].scores[j] << " ";
                    cout << " | Ср. балл: " << currentGroup[i].averageScore << "\n";
                }
            }
            break;

        case 5: // Расчет среднего балла
            if (currentGroup == nullptr) {
                cout << "Группа не создана!\n";
            }
            else {
                dllCalculateAllAverages(currentGroup, groupSize);
                cout << "[УСПЕХ] Средний балл для каждого студента успешно рассчитан.\n";
            }
            break;

        case 6: // Поиск лучшего студента
            if (currentGroup == nullptr) {
                cout << "Группа не создана!\n";
            }
            else {
                int bestId = dllFindBestStudent(currentGroup, groupSize);
                if (bestId == -1) cout << "Не удалось определить лучшего студента.\n";
                else cout << "ID лучшего студента (макс. средний балл): " << bestId << "\n";
            }
            break;

        case 7: // Подсчет должников
            if (currentGroup == nullptr) {
                cout << "Группа не создана!\n";
            }
            else {
                int debtors = dllCountDebtors(currentGroup, groupSize);
                cout << "Количество студентов, имеющих хотя бы одну двойку: " << debtors << "\n";
            }
            break;

        case 8: // Фильтрация по среднему баллу
            if (currentGroup == nullptr) {
                cout << "Группа не создана!\n";
            }
            else {
                int filteredSize = 0;
                double threshold = 4.0;
                Student* filtered = dllFilterByAverage(currentGroup, groupSize, threshold, &filteredSize);

                cout << "\n--- СТУДЕНТЫ С БАЛЛОМ >= 4.0 (ФИЛЬТРАЦИЯ) ---\n";
                if (filtered == nullptr || filteredSize == 0) {
                    cout << "Студенты с таким баллом не найдены.\n";
                }
                else {
                    for (int i = 0; i < filteredSize; i++) {
                        cout << "ID: " << filtered[i].id << " | ФИО: " << filtered[i].fullName
                            << " | Ср. балл: " << filtered[i].averageScore << "\n";
                    }
                    dllFreeGroup(filtered); //освобождаем отфильтрованный массив
                }
            }
            break;

        case 9: // сортировка по сред баллу
            if (currentGroup == nullptr) {
                cout << "Группа не создана!\n";
            }
            else {
                dllSortByAverage(currentGroup, groupSize);
                cout << "Студенты отсортированы по убыванию среднего балла.\n";
            }
            break;

        case 10: // Выгрузка DLL из памяти
            if (hDll == NULL) {
                cout << "\nБиблиотека не была загружена\n";
                break;
            }
            if (currentGroup != nullptr) {
                dllFreeGroup(currentGroup);
                currentGroup = nullptr;
                groupSize = 0;
            }
            FreeLibrary(hDll); // Освобождаем память от DLL
            hDll = NULL;
            cout << "\nДинамическая библиотека успешно выгружена из ОЗУ.\n";
            break;

        case 0: // выход из приложения
            if (currentGroup != nullptr) {
                dllFreeGroup(currentGroup);
            }
            if (hDll != NULL) {
                FreeLibrary(hDll);
            }
            break;

        default:
            cout << "\nНеверный пункт меню! Попробуйте еще раз.\n";
            break;
        }
    }
    return 0;
}