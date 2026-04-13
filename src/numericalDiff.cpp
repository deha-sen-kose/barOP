#include "../include/math/numericalDiff.h"
#include <vector>
#include <functional>

std::vector<double> centralDifference(const std::vector<int> designVariableDOF,
    const std::vector<double> designVariables,
    const std::function<double(std::vector<int>, std::vector<double>)> func){

        double h = 1E-8;
        int size = designVariables.size();
        std::vector<double> gradient(size);

        for(int i = 0; i < size; i++)
        {

            std::vector<double> designVariableTemp = designVariables;
            designVariableTemp[i] = designVariables[i] + h;
            double funcValue1 = func(designVariableDOF, designVariableTemp);
            designVariableTemp[i] = designVariables[i] - h;
            double funcValue2 = func(designVariableDOF, designVariableTemp);

            gradient[i] = 0.5*(funcValue1 - funcValue2)/h;

        }

        return gradient;
    };
