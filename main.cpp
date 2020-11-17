#include <iostream>
#include <vector>
#include <thread>
#include <ctime>
#include <utility>

using namespace std;

// Возваращет размер массива, введенный пользователем с проверкой на неверные данные.
int getSizeOfArray() {
    // Цикл продолжается до тех пор, пока пользователь не введет корректное значение.
    while (true)
    {
        cout << "Введите размер массива > 1000: ";
        int a;
        cin >> a;

        // Проверка на предыдущее извлечение
        if (cin.fail() || a <= 1000) // если предыдущее извлечение оказалось неудачным,
        {
            cin.clear(); // то возвращаем cin в 'обычный' режим работы
            cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
            cout << "Oops, that input is invalid.  Please try again.\n";
        }
        else
        {
            cin.ignore(32767, '\n'); // удаляем лишние значения

            return a;
        }
    }
}

// Возваращет размер массива, введенный пользователем с проверкой на неверные данные.
int getCountOfThreads(int len) {
    // Цикл продолжается до тех пор, пока пользователь не введет корректное значение.
    while (true)
    {
        cout << "Введите количество потоков: ";
        int a;
        cin >> a;

        // Проверка на предыдущее извлечение
        if (cin.fail() || a >= (len / 2) || a <= 0) // если предыдущее извлечение оказалось неудачным,
        {
            cin.clear(); // то возвращаем cin в 'обычный' режим работы
            cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
            cout << "Oops, that input is invalid.  Please try again.\n";
        }
        else
        {
            cin.ignore(32767, '\n'); // удаляем лишние значения

            return a;
        }
    }
}

// Заполняет массив случайными целочисленными.
void getRandomArray(vector<int>& vec, int length) {
    srand(time(nullptr));
    for (int i = 0; i < length; ++i) {
        vec.at(i) = rand();
        cout << vec.at(i) << ", ";
    }
    cout << "\n";
}

// Поиск нужной последовательности.
pair<int, int> searchSequence(vector<int> array, int maxIndex, int minIndex) {
    cout << "Поток " << this_thread::get_id() << " начал работу." << "\n";

    int buffer = 1, maxBuffer = 0, max = 0, maxElement = 0;
    for (int i = minIndex; i < maxIndex; i++)
    {
        if (array.at(i + 1) > array.at(i))
        {
            buffer++;
            max = buffer;
            if (maxBuffer < max)
            {
                maxBuffer = max;
                maxElement = i + 1;
            }
        }
        if (array.at(i + 1) <= array.at(i)) {
            max = buffer;
            if (maxBuffer < max) {
                maxBuffer = max;
                maxElement = i + 1;
            }
            buffer = 1;
        }
    }

    cout << "Поток " << this_thread::get_id() << " завершил работу." << "\n";

    return pair<int, int>(maxElement, maxBuffer);
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Создаем массив чисел.
    int len = getSizeOfArray();
    vector<int> vec(len, 0);

    getRandomArray(vec, len);

    cout << "Сгенерирован массив типа int размером " << len << ", заполненный случайными числами.\n";

    // Получаем кол-во потоков.
    int count = getCountOfThreads(len);
    vector<thread> threads(count);

    // Делим массив на части. Каждый поток будет работать с отдельной частью.
    int temp = len / count;
    vector<int> bounds(count, 0);

    int maxIndex = temp;
    for (int i = 0; i < count; ++i) {
        bounds.at(i) = maxIndex;
        maxIndex += temp;
    }

    // Вектор результатов выполнения функций в потоках.
    // (макс. индекс последовательности, длина последовательности)
    vector< pair<int, int> > results(count, pair<int, int>(0, 0));

    // Поиск нужной последовательности многопоточонстью.
    for (int i = 0; i < count; ++i) {
        threads.at(i) = thread(
            [&results, vec, i, temp, bounds]()
            { results.at(i) = searchSequence(vec, bounds.at(i) - 1, bounds.at(i) - temp); }
        );
    }

    // Дожидаемся всех потоков.
    for (int i = 0; i < count; ++i) {
        threads[i].join();
    }

    int maxBuffer = 0;
    int maxElement = 0;

    // Ищем наилучший результат после работы всех потоков.
    for (int i = 0; i < count; ++i) {
        if (results.at(i).second > maxBuffer) {
            maxBuffer = results.at(i).second;
            maxElement = results.at(i).first;
        }
    }

    // Вывод длины последовательности.
    cout << "\nМаксимальная последовательность: " << maxBuffer << endl << endl;

    // Высчитывается минимальный элемент последовательности.
    int minElement = (maxElement + 1) - maxBuffer;

    //  Вывод последоавтельности.
    cout << "Саммая длинная последовательности чисел: ";
    for (int i = minElement; i <= maxElement; i++)
    {
        cout << "[" << i << "]" << vec.at(i) << " < ";
    }

    // Завершение программы.
    return 0;
}