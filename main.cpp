#include <iostream>
#include <chrono>
#include <future>
#include <mutex>

using namespace std;

void random_fill(int*, int);
void print_arr(int* arr, int size);
void merge(int* arr, int l, int m, int r);
void mergeSort(int* arr, int l, int r);
void threads_mergeSort(int* arr, int l, int r);
void time_spend(void (*func)(int*, int, int), int*, int, int);

int main()
{
    const int size = 100000000;
    int* array1 = new int[size];
    int* array2 = new int[size];

    random_fill(array1, size);
    for (auto i{ 0 }; i < size; i++)
    {
        array2[i] = array1[i];
    }

    cout << endl << "\033[32m mergeSort\033[0m" << endl;
    time_spend(mergeSort, array1, 0, size - 1);

    cout << endl << "\033[32m threads_mergeSort\033[0m" << endl;
    time_spend(threads_mergeSort, array2, 0, size - 1);

    delete[] array1;
    delete[] array2;

    system("pause");
    return 0;
}


void print_arr(int* arr, int size)
{
    printf("\n");

    for (int i = 0; i < size; i++)
        printf("array[%3d] = %3d\n", i, arr[i]);

    printf("\n");
}

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (auto i{ 0 }; i < nl; i++)
        left[i] = arr[l + i];
    for (auto j{ 0 }; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr)
    {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    // записываем оставшиеся элементы левой части
    while (i < nl)
        arr[k++] = left[i++];

    // записываем оставшиеся элементы правой части
    while (j < nr)
        arr[k++] = right[j++];

    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r)
{
    if (l >= r)
        return;

    int m = (l + r - 1) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    merge(arr, l, m, r);
}

void random_fill(int* arr, int size)
{
    for (auto i{ 0 }; i < size; i++)
        arr[i] = rand() % 10000;
}

void time_spend(void (*func)(int*, int, int), int* arr, int left, int right)
{
    auto begin = std::chrono::steady_clock::now();
    func(arr, left, right);
    auto end = std::chrono::steady_clock::now();

    auto seconds = chrono::duration_cast<chrono::seconds>(end - begin).count();
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    auto microseconds = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

    cout << "Time spended: seconds       = " << seconds << endl;
    cout << "              milliseconds  = " << milliseconds - seconds * 1000 << endl;
    cout << "              microseconds  = " << microseconds - milliseconds * 1000 << endl;
    cout << "              nanoseconds   = " << nanoseconds - microseconds * 1000 << endl;
}

void threads_mergeSort(int* arr, int l, int r)
{
    if (l >= r)
        return;

    int m = (l + r - 1) / 2;

    if (r - l > 50000) 
    {
        future<void> leftPart = async(
            launch::async, 
            [&]() {threads_mergeSort(arr, l, m);}
            );

        leftPart.wait();

        threads_mergeSort(arr, m + 1, r);
    }
    else
    {
        threads_mergeSort(arr, l, m);
        threads_mergeSort(arr, m + 1, r);
    }
    merge(arr, l, m, r);
}