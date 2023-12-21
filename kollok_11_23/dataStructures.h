#pragma once


struct funcArgument {

    funcArgument(double** fMatrix, double** sMatrix,
        int rf, int cf, int rs, int cs, int resColCnt,
        double** result, bool** calc) :
        firstMatrix(fMatrix), secondMatrix(sMatrix),
        rowsF(rf), columnsF(cf),
        rowsS(rs), columnsS(cs),
        resultMatrix(result),
        calculated(calc), resultColumns(resColCnt) {}

    int rowsF = 0;
    int columnsF = 0;

    int rowsS = 0;
    int columnsS = 0;

    int resultColumns = 0;

    double** firstMatrix = nullptr;
    double** secondMatrix = nullptr;
    double** resultMatrix = nullptr;

    bool** calculated;
};

struct Element {
    int i;
    int j;
};
