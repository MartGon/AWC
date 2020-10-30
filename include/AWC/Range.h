#pragma once

struct Range
{
public:
    Range(unsigned int maxRange, unsigned int minRange = 0) : maxRange{maxRange}, minRange{minRange} {};

    unsigned int minRange;
    unsigned int maxRange;    
};
