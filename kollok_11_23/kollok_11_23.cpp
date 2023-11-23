#include <iostream>
#include <windows.h>
#include <vector>
#include <set>

struct funcArgument {

    funcArgument(int _arraySize) : arraySize(_arraySize) {}

    int arraySize = 0;
    double* firstArray = nullptr;
    double* secondArray = nullptr;
};

int main()
{
    int rowsCnt, columnsCntFirst, columnsCntSecond;
    std::cout << "enter rows amount and columns amount for the first matrix" << std::endl;
    std::cin >> rowsCnt >> columnsCntFirst;
    std::cout << "enter columns amount for the second matrix" << std::endl;
    std::cin >> columnsCntSecond;

    int threadCnt;
    std::cout << "enter the thread amount" << std::endl;
    std::cin >> threadCnt;

    double** firstMatrix = new double* [rowsCnt];

    for (int i = 0; i < rowsCnt; i++) {
        firstMatrix[i] = new double[columnsCntFirst];
    }

    std::cout << "enter first matrix" << std::endl;

    for (int i = 0; i < rowsCnt; i++) {
        for (int j = 0; j < columnsCntFirst; j++)
            std::cin >> firstMatrix[i][j];
    }

    double** secondMatrix = new double* [rowsCnt];

    for (int i = 0; i < rowsCnt; i++) {
        secondMatrix[i] = new double[columnsCntSecond];
    }

    std::cout << "enter second matrix" << std::endl;

    for (int i = 0; i < rowsCnt; i++) {
        for (int j = 0; j < columnsCntSecond; j++)
            std::cin >> secondMatrix[i][j];
    }

    std::set<int> numbers;

    for (int i = 0; i < columnsCntFirst; i++) {
        numbers.insert(i);
    }

    HANDLE* hThread = new HANDLE[threadCnt];
    DWORD* IDThread = new DWORD[threadCnt];

    funcArgument curArg(columnsCntFirst);

    for (int i = 0; i < threadCnt; i++) {

        curArg.firstArray;

        hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)funcToCall, (void*)curArg, 0, &IDThread[i]);

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