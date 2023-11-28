/*
#2
Разработайте многопоточное приложение, выполняющее вычисление произведения 
матриц A (m×n) и B (n×k). Элементы cij матрицы произведения С = A×B вычисляются
параллельно p однотипными потоками. Если некоторый поток уже вычисляет элемент cij матрицы C, 
то следующий приступающий к вычислению поток выбирает для расчета элемент cij+1, если j<k, и ci+1k, 
если j=k. Выполнив вычисление элемента матрицы-произведения, поток проверяет, нет ли элемента, 
который еще не рассчитывается. Если такой элемент есть, то приступает к его расчету. В противном 
случае отправляет (пользовательское) сообщение о завершении своей работы и приостанавливает своё 
выполнение. Главный поток, получив сообщения о завершении вычислений от всех потоков, выводит 
результат на экран и запускает поток, записывающий результат в конец файла-протокола. В каждом потоке 
должна быть задержка в выполнении вычислений (чтобы дать возможность поработать всем потокам). 
Синхронизацию потоков между собой организуйте через критическую секцию или мьютекс. Желательно покрыть
код Unit Test-ами.
*/

#include <iostream>
#include <windows.h>
#include <vector>
#include <set>

CRITICAL_SECTION critical_section;

struct funcArgument {

    funcArgument(int _arraySize, double** result, double* fArray, double* sArray, int row, int column) : 
        arraySize(_arraySize), resultMatrix(result),
        firstArray(fArray), secondArray(sArray),
        curRow(row), curColumn(column) {}

    int arraySize = 0;
    int curRow = 0;
    int curColumn = 0;

    double* firstArray = nullptr;
    double* secondArray = nullptr;
    double** resultMatrix = nullptr;
};

template<class T>
T** createMatrix(int n, int m) {
    T** matrix = new T* [n];

    for (int i = 0; i < n; i++) {
        matrix[i] = new T[m];
    }

    return matrix;
}

template<class T>
void readMatrix(T** a, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            std::cin >> a[i][j];
    }

    return;
}

template<class T>
T** transposeMatrix(T** a, int n, int m) {
    T** result = createMatrix(m, n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[j][i];
        }
    }

    return result;
}

double calculateElement(funcArgument* arg) {

    funcArgument* info = static_cast<funcArgument*>(arg);

    while (1) {
        double result = 0;

        for (int i = 0; i < info->arraySize; i++) {
            result += info->firstArray[i] * info->secondArray[i];
            Sleep(100);
        }

        info->resultMatrix[info->curRow][info->curColumn] = result;

        //wait for the next element available
    }

    return 0;
}

int main()
{
    InitializeCriticalSection(&critical_section);

    int rowsCntFirst, columnsCntFirst, columnsCntSecond;
    std::cout << "enter rows amount and columns amount for the first matrix" << std::endl;
    std::cin >> rowsCntFirst >> columnsCntFirst;

    double** firstMatrix = createMatrix<double>(rowsCntFirst, columnsCntFirst);
    std::cout << "enter first matrix" << std::endl;
    readMatrix(firstMatrix, rowsCntFirst, columnsCntFirst);

    std::cout << "enter columns amount for the second matrix" << std::endl;
    std::cin >> columnsCntSecond;
    int rowsCntSecond = columnsCntFirst;

    double** secondMatrixRaw = createMatrix<double>(rowsCntSecond, columnsCntSecond);
    std::cout << "enter second matrix" << std::endl;
    readMatrix(secondMatrixRaw, rowsCntSecond, columnsCntSecond);

    double** secondMatrix = transposeMatrix(secondMatrixRaw, rowsCntSecond, columnsCntSecond);
    std::swap(rowsCntSecond, columnsCntSecond);

    double** resultMatrix = createMatrix<double>(rowsCntFirst, columnsCntSecond);

    bool** calculated = createMatrix<bool>(rowsCntFirst, columnsCntSecond);
    for (int i = 0; i < rowsCntFirst; i++) {
        for (int j = 0; j < columnsCntSecond; j++)
            calculated[i][j] = false;
    }

    int threadCnt;
    std::cout << "enter the thread amount" << std::endl;
    std::cin >> threadCnt;

    HANDLE* hThread = new HANDLE[threadCnt];
    DWORD* IDThread = new DWORD[threadCnt];

    for (int k = 0; k < threadCnt; k++) {

        int i, j;

        for (i = 0; i < rowsCntFirst; i++) {
            for (j = 0; j < columnsCntSecond; j++)
                if (!calculated[i]) {
                    break;
                }
        }

        funcArgument* curArg = new funcArgument(
            columnsCntFirst, resultMatrix,
            firstMatrix[i], secondMatrix[j],
            i, j
        );

        hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)calculateElement, (void*)curArg, 0, &IDThread[i]);

        if (hThread[i] == NULL) {
            std::cout << "could not create a thread" << std::endl;
        }
    }
    
    for (int i = 0; i < threadCnt; i++) {
        WaitForSingleObject(hThread[i], INFINITE);
        CloseHandle(hThread[i]);
    }

    return 0;
}