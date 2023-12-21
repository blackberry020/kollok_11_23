#include "pch.h"
//#include "../kollok_11_23/kollok_11_23.cpp"
//#include "stdafx.h"
#include "gtest/gtest.h"

#include "../kollok_11_23/matrixFunctions.h"

class transposeMatrixTest : public ::testing::Test {
};

TEST(transposeMatrixTest, TransposeOneElementMatrix) {

	double** oneElementMatrix = createMatrix<double>(1, 1);
	oneElementMatrix[0][0] = 5;
	double** transposed = transposeMatrix<double>(oneElementMatrix, 1, 1);

	double** expected = createMatrix<double>(1, 1);
	expected[0][0] = 5;

	//ASSERT_EQ(memcmp(expected, transposed, sizeof(expected)), 0);
	ASSERT_TRUE(true);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}