#include <stdio.h>
#include <iostream>
#include <unsupported/Eigen/MatrixFunctions>

namespace Eigen {

template<typename MatrixType>
void mySchurMatrix(const MatrixType& m)
{
    MatrixType result = MatrixType::Random(m.rows(), m.rows());

    //std::cout << "random matrix: " << std::endl << result << std::endl;

    RealSchur<MatrixType> schur(result);
    MatrixType T = schur.matrixT();
    std::cout << "T: " << std::endl << T << std::endl;
    MatrixType U = schur.matrixU();
    //std::cout << "U: " << std::endl << U << std::endl;

    const Index size = result.cols();

    for (Index i=0; i < size; ++i) {
      if (i == size - 1 || T.coeff(i+1,i) == 0)
        T.coeffRef(i,i) = std::abs(T.coeff(i,i));
      else
        ++i;
    }
    MatrixType mfinal = U * T * U.transpose();

    //std::cout << "schur matrix: " << std::endl << mfinal << std::endl;
}

}

int main()
{
    unsigned int seed;

    //std::cout << "__DBL_EPSILON__:" << __DBL_EPSILON__ << std::endl;
    //std::cout << "std::epsilon double: " << std::numeric_limits<double>::epsilon() << std::endl;
    //std::cout << "__LDBL_EPSILON__:" << __LDBL_EPSILON__ << std::endl;
    //std::cout << "std::epsilon long double: " << std::numeric_limits<long double>::epsilon() << std::endl;

    //std::cout << "__DBL_MIN__: " << __DBL_MIN__ << std::endl;
    //std::cout << "__LDBL_MIN__: " << __LDBL_MIN__ << std::endl;
    //std::cout << "size: " << sizeof(double) << " min double: " << std::numeric_limits<double>::min() << std::endl;
    //std::cout << "size: " << sizeof(long double) << " min long double: " << std::numeric_limits<long double>::min() << std::endl;

    //  ppc64le and x86 print the same values
    //  ('random' and 'schur' matrix are equal)
    seed = 1633528798;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    //  ppc64le and x86 print different values
    //  ('random' is equal, but 'schur' is different)
    seed = 1633528802;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    //  ppc64le and x86 print different values
    //  ('random' is equal, but 'schur' has only -nan on ppc64le)
    seed = 1633528806;
    srand(seed);
    std::cout << "Seed: " << seed << std::endl;
    Eigen::mySchurMatrix(Eigen::Matrix<long double,3,3>());
    std::cout << std::endl;

    return 0;
}

