#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <string>
#include <vector>

enum TokenType
{
    INTEGER,
    DOUBLE,
    OPERATOR,
    OTHER
};

struct TokenInfo
{
    TokenType token_type;
    std::string token_value;
};


class Calculator
{
private:
        float m_value;

public:
        Calculator (float default_value);
        Calculator& add(float value);
        Calculator& sub(float value);
        Calculator& mul(float value);
        Calculator& div(float value);

        void displayCalcValue(void);
        void setCalcValue(float value);
	float  returnCalcValue(void);
};

#endif
