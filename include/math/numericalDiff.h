#ifndef NUMERICALDIFF_H
#define NUMERICALDIFF_H

#include <vector>
#include <functional>

std::vector<double> centralDifference(const std::vector<int> designVariableDOF,
    const std::vector<double> designVariables,
    const std::function<double(std::vector<int>, std::vector<double>)> func);


#endif
