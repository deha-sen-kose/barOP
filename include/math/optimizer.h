#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <functional>
#include <vector>

std::vector<double> SQP(std::function<double (std::vector<int>, std::vector<double>)> f,
    std::function<double (std::vector<int>, std::vector<double>)> g, std::vector<double> s,
    std::vector<int> sdof);

#endif
