#pragma once
#include <vector>
class AverageFilter
{
private:
    int m_filterSize=0;
    std::vector<float> m_valueVec;
public:
    AverageFilter(int size);
    ~AverageFilter();
    float update(float value);
};


