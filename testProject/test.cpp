#include "pch.h"
#include "gtest/gtest.h"

#include "../kollok_11_23/matrixFunctions.h"

class transposeMatrixTest : public ::testing::Test {
};

TEST(transposeMatrixTest, TransposeOneElementMatrix) {

	int n = 1, m = 1;

	double** oneElementMatrix = createMatrix<double>(n, m);
	oneElementMatrix[0][0] = 5;
	double** transposed = transposeMatrix<double>(oneElementMatrix, n, m);

	double** expected = createMatrix<double>(n, m);
	expected[0][0] = 5;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			EXPECT_EQ(oneElementMatrix[i][j], transposed[i][j]);
	}
}

//TEST(transposeMatrixTest, TransposeTwoByThreeMatrix) {
//
//	int n = 2, m = 3;
//
//	
//
//	double** oneElementMatrix = createMatrix<double>(n, m);
//	oneElementMatrix[0][0] = 5;
//	double** transposed = transposeMatrix<double>(oneElementMatrix, n, m);
//
//	double** expected = createMatrix<double>(n, m);
//	expected[0][0] = 5;
//
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < m; j++)
//			EXPECT_EQ(oneElementMatrix[i][j], transposed[i][j]);
//	}
//}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}