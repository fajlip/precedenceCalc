#include <string.h> 
#include <cctype>
#include <algorithm>
#include "calc.h"

TokenInfo getToken(const char *tokenString)
{
	TokenInfo ourToken;

	//empty string is not correct option
	if (tokenString[0] == '\0')
	{
		ourToken.token_type = OTHER;
		return ourToken;
	}

	ourToken.token_value = tokenString;

	if (strlen(tokenString) == 1 && (tokenString[0] == '+' || tokenString[0] == '-' || tokenString[0] == '*' || tokenString[0] == '/'))
	{
		ourToken.token_type = OPERATOR;
		return ourToken;
	}

	bool dot_used = false;
	for (int i = 0; i < strlen(tokenString); ++i)
	{
		if ((!isdigit(tokenString[i]) && tokenString[i] != '.')
		|| (tokenString[i] == '.' && (dot_used || i == 0 || i + 1 == strlen(tokenString))))

		{
			ourToken.token_type = OTHER;
			return ourToken;
		}

		else if (tokenString[i] == '.')
			dot_used = true; 
	}

	if (dot_used)
		ourToken.token_type = DOUBLE;
	else
		ourToken.token_type = INTEGER;

	return ourToken;
}

void parseArgs(int argc, char **argv, std::vector<std::string> &arguments)
{
        if (argc != 2)
        {
                std::cout << "Wrong arguments, you can use -help or -h. All input must be in quotes, because of asterix interpretation by shell." << std::endl;
                exit(EXIT_FAILURE);
        }

        else if (argc == 2 && ((strcmp(argv[1], "-help") == 0) || strcmp(argv[1], "-h") == 0))
        {
                std::cout << "This is help for you:\nWrite what you want to calculate. With style:\nNumber Operation Number\nBrackets have not been implemented yet.\n\n";
                exit(EXIT_SUCCESS);
        }

	std::string input = argv[1];
	//remove all whitespaces
	input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());


	int pos = 0;
	bool operatorMode = false;
	for (int i = 0; i < input.length(); ++i)
	{
		if (isdigit(input[i]) || (input[i] == '.'))
		{
			if (operatorMode)
			{
				arguments.push_back(input.substr(pos, i - pos));
			        pos = i;
				operatorMode = false;
			}
		
		}     
		
		else
		{
			if (!operatorMode)
			{
				//string going from start to this position was number
				arguments.push_back(input.substr(pos, i - pos));
				pos = i;
				operatorMode = true;
			}
		}		
	
	
	}	
	
	//input was ended with operator
	if (operatorMode)
	{	
		std::cout << "Expresion was not ended with number" << std::endl;
		exit(EXIT_FAILURE);
	}

	arguments.push_back(input.substr(pos));

        
	//check those arguments that have been isolated
	TokenInfo testToken;

	for (int i = 0; i < arguments.size(); ++i)
	{
		if ((i % 2) == 0 || i == 0)
		{
			testToken = getToken(arguments.at(i).c_str());

        		if (testToken.token_type != DOUBLE && testToken.token_type != INTEGER)
        		{
        		        std::cout << "First argument is wrong, you have to put number here." << std::endl;
        		        exit(EXIT_FAILURE);
        		}
        	}

		else
		{
			testToken = getToken(arguments.at(i).c_str());

       			if (testToken.token_type != OPERATOR)
       	 		{
       	       			 std::cout << "Second argument is wrong, you have to put number here." << std::endl;
       			         exit(EXIT_FAILURE);
        		}

		}
	} 
	
	return;
}
