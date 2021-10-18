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

    int iter = 1;

    for (int i=0; i < iter; ++i) {
        std::cout << "iter " << i << std::endl;
        generateTestMatrix<MatrixType>::run(m1, m.rows());
        MatrixPower<MatrixType> mpow(m1);

        x = internal::random<RealScalar>();
        y = internal::random<RealScalar>();
        m2 = mpow(x);
        m3 = mpow(y);

        m4 = mpow(x+y);
        m5.noalias() = m2 * m3;
        std::cout << "m4: " << m4 << std::endl;
        std::cout << "m5: " << m5 << std::endl;
        std::cout << "tol: " << tol << std::endl;
        std::cout << m4.isApprox(m5, tol) << std::endl;
        //VERIFY(m4.isApprox(m5, tol));

        //m4 = mpow(x*y);
        //m5 = m2.pow(y);
        //std::cout << m4.isApprox(m5, tol) << std::endl;
        ////VERIFY(m4.isApprox(m5, tol));

        //m4 = (std::abs(x) * m1).pow(y);
        //m5 = std::pow(std::abs(x), y) * m3;
        //std::cout << m4.isApprox(m5, tol) << std::endl;
        ////VERIFY(m4.isApprox(m5, tol));
    }
}

typedef Matrix<long double,3,3> Matrix3e;

}

int main()
{
    // Iter 0, 1, 2, 3 pass
    // Iter 4 never finish
    unsigned int seed = 1633528798;

    // Iter 0 fails
    // Iter 1 pass
    // Iter 2 never finish
    //unsigned int seed = 1633528802;

    // Iter 0 never finish
    //unsigned int seed = 1633528806;

    srand(seed);

    //printf("Matrix2d():\n");
    //Eigen::testGeneral(Eigen::Matrix2d(), 1e-13);
    printf("Matrix3e():\n");
    Eigen::testGeneral(Eigen::Matrix3e(), 1e-13L);
    return 0;
}

