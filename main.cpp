#include <stdio.h>
#include <iostream>
#include <unsupported/Eigen/MatrixFunctions>

namespace Eigen {

template<typename MatrixType>
void mySchurMatrix(const MatrixType& m)
{
    MatrixType result = MatrixType::Random(m.rows(), m.rows());

    std::cout << "random matrix: " << std::endl << result << std::endl;

    RealSchur<MatrixType> schur(result);
    MatrixType T = schur.matrixT();
    MatrixType U = schur.matrixU();

    const Index size = result.cols();

    for (Index i=0; i < size; ++i) {
      if (i == size - 1 || T.coeff(i+1,i) == 0)
        T.coeffRef(i,i) = std::abs(T.coeff(i,i));
      else
        ++i;
    }
    MatrixType mfinal = U * T * U.transpose();

    std::cout << "schur matrix: " << std::endl << mfinal << std::endl;
}

}

int main()
{
    unsigned int seed;

    //  ppc64le and x86 print the same values
    //  ('result' and 'schur' matrix are equal)
    seed = 1633528798;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    //  ppc64le and x86 print different values
    //  ('result' is equal, but 'schur' is different)
    seed = 1633528802;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    //  ppc64le and x86 print different values
    //  ('result' is equal, but 'schur' has only -nan on ppc64le)
    seed = 1633528806;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    return 0;
}

