#include <stdio.h>
#include <iostream>
#include <unsupported/Eigen/MatrixFunctions>

namespace Eigen {

template<typename MatrixType>
struct processTriangularMatrix
{
  static void run(MatrixType& m, MatrixType& T, const MatrixType& U)
  {
    const Index size = m.cols();

    for (Index i=0; i < size; ++i) {
      if (i == size - 1 || T.coeff(i+1,i) == 0)
        T.coeffRef(i,i) = std::abs(T.coeff(i,i));
      else
        ++i;
    }
    m = U * T * U.transpose();
  }
};


template <typename MatrixType>
struct generateTestMatrix
{
  static void run(MatrixType& result, typename MatrixType::Index size)
  {
    result = MatrixType::Random(size, size);
    RealSchur<MatrixType> schur(result);
    MatrixType T = schur.matrixT();
    processTriangularMatrix<MatrixType>::run(result, T, schur.matrixU());
  }
};


template<typename MatrixType>
void testGeneral(const MatrixType& m, const typename MatrixType::RealScalar& tol)
{
    typedef typename MatrixType::RealScalar RealScalar;
    MatrixType m1, m2, m3, m4, m5;
    RealScalar x, y;

    int iter = 10;

    for (int i=0; i < iter; ++i) {
        std::cout << "iter " << i << std::endl;
        generateTestMatrix<MatrixType>::run(m1, m.rows());
        MatrixPower<MatrixType> mpow(m1);

        std::cout << "m1: " << m1 << std::endl;

        x = internal::random<RealScalar>();
        y = internal::random<RealScalar>();
        m2 = mpow(x);
        m3 = mpow(y);

        m4 = mpow(x+y);
        m5.noalias() = m2 * m3;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        std::cout << "m4: " << m4 << std::endl;
        std::cout << "m5: " << m5 << std::endl;
        std::cout << "tol: " << tol << std::endl;
        std::cout << m4.isApprox(m5, tol) << std::endl;
        //VERIFY(m4.isApprox(m5, tol));

        m4 = mpow(x*y);
        m5 = m2.pow(y);
        std::cout << m4.isApprox(m5, tol) << std::endl;
        //VERIFY(m4.isApprox(m5, tol));

        m4 = (std::abs(x) * m1).pow(y);
        m5 = std::pow(std::abs(x), y) * m3;
        std::cout << m4.isApprox(m5, tol) << std::endl;
        //VERIFY(m4.isApprox(m5, tol));
    }
}

template<typename MatrixType>
void generateRandomMatrix(const MatrixType& m)
{
    MatrixType result = MatrixType::Random(m.rows(), m.rows());

    std::cout << "result: " << result << std::endl;

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

    std::cout << "mfinal: " << mfinal << std::endl;
}

typedef Matrix<long double,3,3> Matrix3e;

}

int main()
{
    // function testGeneral:
    //  Iter 0, 1, 2, 3 pass
    //  Iter 4 never finish
    //
    // function generateRandomMatrix (only one iter):
    //  ppc64le and x86 print the same values
    //  ('result' and 'mfinal' matrix are equal)
    unsigned int seed = 1633528798;

    // function testGeneral:
    //  Iter 0 fails
    //  Iter 1 pass
    //  Iter 2 never finish
    //
    // function generateRandomMatrix (only one iter):
    //  ppc64le and x86 print different values
    //  ('result' is equal, but 'mfinal' is different)
    //unsigned int seed = 1633528802;

    // function testGeneral:
    //  Iter 0 never finish
    //
    // function generateRandomMatrix (only one iter):
    //  ppc64le and x86 print different values
    //  ('result' is equal, but 'mfinal' has only -nan on ppc64le)
    //unsigned int seed = 1633528806;

    srand(seed);

    Eigen::generateRandomMatrix(Eigen::Matrix3e());
    //Eigen::testGeneral(Eigen::Matrix3e(), 1e-13L);
    return 0;
}

