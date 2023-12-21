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

#include "matrixFunctions.h"

CRITICAL_SECTION critical_section;

const int OUT_OF_RANGE = -1;

Element getFirstAvailableElement(funcArgument* info) {

    int i = 0, j = 0;
    bool flag = false;

    for (i = 0; i < info->rowsF; i++) {
        for (j = 0; j < info->resultColumns; j++)
            if (!info->calculated[i][j]) {
                flag = true;
                break;
            }
        if (flag)
            break;
    }

    if (i >= info->rowsF || j >= info->resultColumns) {
        return { -1, -1 };
    }

    return { i, j };
}

double calculateElement(funcArgument* arg) {

    funcArgument* info = static_cast<funcArgument*>(arg);

    while (1) {

        EnterCriticalSection(&critical_section);

        Element curElemet = getFirstAvailableElement(info);

        if (curElemet.i == OUT_OF_RANGE || curElemet.j == OUT_OF_RANGE) {
            std::cout << "no left spare elements" << std::endl;

            LeaveCriticalSection(&critical_section);

            return 0;
        }

        LeaveCriticalSection(&critical_section);

        double result = 0;

        double* firstArray = info->firstMatrix[curElemet.i];
        double* secondArray = info->secondMatrix[curElemet.j];

        for (int k = 0; k < info->columnsF; k++) {
            result += firstArray[k] * secondArray[k];
        }

        info->resultMatrix[curElemet.i][curElemet.j] = result;
        info->calculated[curElemet.i][curElemet.j] = true;
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

    double** secondMatrixRow = createMatrix<double>(rowsCntSecond, columnsCntSecond);
    std::cout << "enter second matrix" << std::endl;
    readMatrix(secondMatrixRow, rowsCntSecond, columnsCntSecond);

    double** secondMatrix = transposeMatrix(secondMatrixRow, rowsCntSecond, columnsCntSecond);
    
    int resultMatrixColumnsCnt = columnsCntSecond;
    std::swap(rowsCntSecond, columnsCntSecond);

    double** resultMatrix = createMatrix<double>(rowsCntFirst, resultMatrixColumnsCnt);

    bool** calculated = createMatrix<bool>(rowsCntFirst, resultMatrixColumnsCnt);
    for (int i = 0; i < rowsCntFirst; i++) {
        for (int j = 0; j < resultMatrixColumnsCnt; j++)
            calculated[i][j] = false;
    }

    int threadCnt;
    std::cout << "enter the thread amount" << std::endl;
    std::cin >> threadCnt;

    HANDLE* hThread = new HANDLE[threadCnt];
    DWORD* IDThread = new DWORD[threadCnt];

    for (int k = 0; k < threadCnt; k++) {

        funcArgument* curArg = new funcArgument(
            firstMatrix, secondMatrix,
            rowsCntFirst, columnsCntFirst, rowsCntSecond, columnsCntSecond, resultMatrixColumnsCnt,
            resultMatrix, calculated
        );

        hThread[k] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)calculateElement, (void*)curArg, 0, &IDThread[k]);

        if (hThread[k] == NULL) {
            std::cout << "could not create a thread" << std::endl;
        }
    }
    
    for (int i = 0; i < threadCnt; i++) {
        WaitForSingleObject(hThread[i], INFINITE);
        CloseHandle(hThread[i]);
    }

    std::cout << "result matrix is " << std::endl;

    for (int i = 0; i < rowsCntFirst; i++) {
        for (int j = 0; j < resultMatrixColumnsCnt; j++)
            std::cout << resultMatrix[i][j] << " ";

        std::cout << std::endl;
    }

    return 0;
}