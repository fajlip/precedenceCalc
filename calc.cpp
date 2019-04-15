#include "calc.h"

TokenInfo getToken(const char * tokenString);
void parseArgs(int argc, char **argv, std::vector<std::string> &arguments);

Calculator::Calculator(float value = 0.0) : m_value (value) {}

Calculator& Calculator::add(float value) { m_value += value; return *this; }
Calculator& Calculator::sub(float value) { m_value -= value; return *this; }
Calculator& Calculator::mul(float value) { m_value *= value; return *this; }
Calculator& Calculator::div(float value)
{ 
if (value != 0)
	m_value /= value;
else
	std::cout << "You cannot divide number by zero, not gonna happen. Sorry sorry." << std::endl;

return *this;
}

void Calculator::displayCalcValue(void) { std::cout << "Calculator value is: " << m_value << std::endl; }
float  Calculator::returnCalcValue (void) { return m_value; }
void Calculator::setCalcValue(float value) { m_value = value; }


float calculateIt(float number2, Calculator& (Calculator::*calculationFunction) (float value), Calculator calcObject)
{
	(calcObject.*calculationFunction)(number2);
	return calcObject.returnCalcValue();
}


int getIndexToFuncArray(char inputOperator)
{
	const int START_OF_ASCII_OPERATORS = 42;
	return inputOperator - START_OF_ASCII_OPERATORS;
}

typedef Calculator& (Calculator::*FunctionPointer) (float a);

float operatorPrecedenceCalculation(std::vector<std::string> &arguments, FunctionPointer arrayOfCalcFunctions[], Calculator calcObject)
{
	std::vector<std::string> stack;
		
	//check this
	size_t leftBracePos = -1;
	std::string tempNumb1;

	float tempResult = 0.0;
	for (int i = 0; i < arguments.size(); ++i)
	{
		switch(arguments.at(i)[0])
		{
			
			case '(':
				leftBracePos = i;
				break;
			case ')':
				tempResult = 0.0;
				calcObject.setCalcValue(std::stof(stack.at(leftBracePos)));
				
				for (int y = leftBracePos; y < (i - 1); ++y)
				{
					if (stack.at(y)[0] == '+' || stack.at(y)[0] == '-')
					{
						//accumulated result in the brackets
						tempResult = calculateIt(std::stof(stack.at(y - 1)), arrayOfCalcFunctions[getIndexToFuncArray(stack.at(y)[0])], calcObject);
						calcObject.setCalcValue(tempResult);		
				
						//skip number, already used
						y++;
					}
				}
				
				
				stack.resize(leftBracePos);
				stack.push_back(std::to_string(tempResult));
			

				if (((stack.at(leftBracePos - 1))[0] == '*') || stack.at(leftBracePos - 1)[0] == '/')
				{
					std::string number2 = stack.back();
					stack.pop_back();

					std::string operation = stack.back();
					stack.pop_back();

					std::string number1 = stack.back();
					stack.pop_back();
				
					calcObject.setCalcValue(std::stof(number1));
	                                stack.push_back(std::to_string(calculateIt(std::stof(number2), arrayOfCalcFunctions[getIndexToFuncArray(operation[0])], calcObject)));
				}		

				//reset position counter
				leftBracePos = -1;
				
				break;
			
			case '*':
			case '/':
				if (arguments.at(i + 1)[0] == '(')
					stack.push_back(arguments.at(i));

				else
				{
					tempNumb1 = stack.back();
					stack.pop_back();

					calcObject.setCalcValue(std::stof(tempNumb1));
					stack.push_back(std::to_string(calculateIt(std::stof(arguments.at(i + 1)), arrayOfCalcFunctions[getIndexToFuncArray(arguments.at(i)[0])], calcObject)));	
					//skip next number
					i++;
				}

				break;
			
			case '+':
            case '-':
			//numbers
			default:
				stack.push_back(arguments.at(i));
                                break;
					
		
		}	
	
	}

	
	//stack is full of easy operations, which can be performed step by step
	float result = 0.0;

	calcObject.setCalcValue(std::stof(stack.at(0)));	
	for (int i = 0; i < stack.size(); ++i)
	{
		 if (stack.at(i)[0] == '+' || stack.at(i)[0] == '-')
		 {
			//accumulated result in the brackets
			result = calculateIt(std::stof(stack.at(i + 1)), arrayOfCalcFunctions[getIndexToFuncArray(stack.at(i)[0])], calcObject);                                            	     calcObject.setCalcValue(result);
			
			//skip number, already used
			i++;
		 }
		 
	}	
	return result;
	

}

int main (int argc, char **argv)
{
	std::vector<std::string> arguments;
	parseArgs(argc, argv, arguments);

	Calculator calcObject(std::stof(arguments.at(0)));

	//array of basic mathematical functions, defined in class Calculator	
       	FunctionPointer arrayOfCalcFunctions[] =
	{
		&Calculator::mul,
		&Calculator::add,
		nullptr, //preskoc ascii ,
		&Calculator::sub,
		nullptr, //preskoc ascii .
		&Calculator::div	
	};

	std::cout << "result:" << operatorPrecedenceCalculation(arguments, arrayOfCalcFunctions, calcObject) << std::endl;

	
	return 0;
}
