#include "AverageFilter.h"

AverageFilter::AverageFilter(int size):m_filterSize(size)
{
}

AverageFilter::~AverageFilter()
{
}

float AverageFilter::update(float value)
{
    if(m_valueVec.size()>=m_filterSize)
    {
        m_valueVec.pop_back();
    }
    m_valueVec.insert(m_valueVec.begin(),value/1000);
    
    float _r=0;
    
    for (size_t i = 0; i < m_valueVec.size(); i++)
    {
        _r += m_valueVec.at(i);
    }
    
    if(_r != 0)
    {
        _r /= m_valueVec.size();
    }

    return _r*1000;
}