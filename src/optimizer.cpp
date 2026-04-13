#include "../include/math/optimizer.h"
#include "../include/math/numericalDiff.h"
#include "../include/math/Matrix.h"
#include <cmath>

std::vector<double> SQP(std::function<double (std::vector<int>, std::vector<double>)> f,
    std::function<double (std::vector<int>, std::vector<double>)> g, std::vector<double> s,
    std::vector<int> sdof, int iter){

        size_t sNum = s.size();
        Matrix<double> H(sNum,sNum,0.0);

        for(size_t i = 0; i < sNum; i++){

            H(i,i) = 1.0;
        }

        size_t m = sNum + 1;
        size_t n = sNum + 1;

        Matrix<double> LHS(m,n,0.0);
        std::vector<double> RHS(n);
        double mu = 1.0;
        std::vector<double> d(m);

        int ii = 0;
        while (ii < iter){

            for(size_t i = 0; i < sNum; i++)
            {
                for (size_t j = 0; j < sNum; j++)
                {
                    LHS(i,j) = H(i,j);
                }
            }

        double fVal = f(sdof, s);
        std::vector<double> fGrad = centralDifference(sdof, s, f);

        double norm = 0.0;
        for(size_t i = 0; i < sNum; i++)
        {
            norm = norm + fGrad[i]*fGrad[i];
        }
        norm = std::sqrt(norm);
        if (norm < 1E-6)
        {break;};

        double gVal = g(sdof, s);
        std::vector<double> gGrad = centralDifference(sdof, s, g);

            for(size_t i = 0; i < sNum ; i++)
            {
                for(size_t j = sNum; j < n; j++){

                    LHS(i,j) = gGrad[i];
                }
            }

            for(size_t i = sNum; i < m ; i++)
            {
                for(size_t j = 0; j < sNum; j++){

                    LHS(i,j) = gGrad[j];
                }
            }

            for(size_t i = 0; i < n-1; i++)
            {
                RHS[i] = fGrad[i] + mu*gGrad[i];
            }
            RHS[n-1] = gVal;

            // You need a better solver! SQP matrices are not necessarily symmetric positive def.!
            Matrix<double> T = LHS.cho();
            Matrix<double> inv = T.L_inverse();
            std::vector<double> d = inv.mVm(RHS);

            //Update parameters
            for(size_t i = 0; i < sNum; i++){
                s[i] = s[i] - d[i];
            }
            mu = mu - d[sNum];

            //Update Hessian
            //

        };

        std::vector<double> A{};
        return A;
};
