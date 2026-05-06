#include "../include/math/mathflb.h"
#include "../include/math/Matrix.h"
#include <stdexcept>

std::vector<double> gaussSeidel(const Matrix<double>& A,
                                const std::vector<double> b)
{

    size_t sysSize = b.size();
    std::vector<double> x(sysSize); 

    bool convergence = false;
    double sigma = 0.0;
    double tolerance = 1E-8;
    int maxIter = 10000;
    int iter = 0;

    do {

        double maxDiff = 0.0;
        
        for(size_t i = 0; i < sysSize; i++){

            sigma = 0.0;
            
            for(size_t j = 0; j < sysSize; j++){
                
                if(i != j){

                    sigma = sigma + A(i,j)*x[j];
                    
                }            
            }


            double x_old = x[i];

            if (std::abs(A(i,i)) < 1E-12) {
                std::cerr <<
                "Warning: Zero or non-zero diagonal element \n";
            }
            
            x[i] = (b[i]-sigma)/(double)A(i,i);


            maxDiff = std::max(maxDiff,
                                      std::abs(x[i]-x_old));
            
            // std::cout << x[0] << std::endl;
             
            // std::cout << x[1] << std::endl;

            // std::cout << x[2] << std::endl;
        }


        convergence = (maxDiff < tolerance);

        iter++;
                 
    } while (!convergence && iter < maxIter);    

    return x;
};


std::vector<double> frwdSubstitution(const Matrix<double>& L,
                                     const std::vector<double>& b){

    size_t r = b.size();
    std::vector<double> y(r);
    double sum = 0.0;
    for(size_t i = 0; i < r; ++i)
    {
        sum = 0.0;
        for(size_t j = 0; j < i; ++j)
        {
            sum += L(i,j)*y[j];
        }
        if(std::abs(L(i,i)) < 1E-6)
        {
            throw std::runtime_error("Division by zero! "
                                     "frwdSubstitution \n");
        }
        y[i] = (b[i] - sum) / L(i,i);
    }

    return y;
};



std::vector<double> bckwrdSubstitution(const Matrix<double>& L,
                                     const std::vector<double>& y){

    
    size_t r = y.size();
    std::vector<double> x(r);

    for (size_t ii = 0; ii < r; ++ii) {
        size_t i = r - 1 - ii;    
        double sum = 0.0;
        for (size_t j = i + 1; j < r; ++j) {
            sum += L(j,i) * x[j];  
        }
        if(std::abs(L(i,i)) < 1E-6)
        {
            throw std::runtime_error("Division by zero! "
                                     "bckwrdSubstitution \n");
        }
        x[i] = (y[i] - sum) / L(i,i);
    }

    return x;
    
}


std::vector<double> choleskySolver(const Matrix<double>& A,
                                   const std::vector<double>& b){

    size_t r = b.size();

    if(r != A.getSize()[0] || r != A.getSize()[1])
    {
        throw std::invalid_argument("Given sizes do not match!" 
                                    " choleskySolver \n");
    }

    Matrix<double> L(r, r, 0.0);

    for (size_t i = 0; i < r; ++i){
        for (size_t j = 0; j <= i; ++j){
            double sum = 0.0;
            for (size_t k = 0; k < j; ++k){
                sum += L(i,k)*L(j,k);
            };
               if(i==j){
                   L(j,j) = std::sqrt(A(j,j)-sum);
               }
               else{
                 L(i,j) = (A(i,j)-sum)/L(j,j);
               };

        };
    };

    std::vector<double> y = frwdSubstitution(L, b);
    std::vector<double> x = bckwrdSubstitution(L, y);
    
    return x;
};

    
