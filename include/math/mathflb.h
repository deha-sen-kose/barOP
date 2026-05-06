#ifndef MATHFLB_H
#define MATHFLB_H

#include "Matrix.h"

std::vector<double> gaussSeidel(const Matrix<double>& A,
                                const std::vector<double> b);

std::vector<double> frwdSubstitution(const Matrix<double>& L,
                                     const std::vector<double>& b);

std::vector<double> bckwrdSubstitution(const Matrix<double>& L,
                                     const std::vector<double>& y);

std::vector<double> choleskySolver(const Matrix<double>& A,
                                   const std::vector<double>& b);

#endif
