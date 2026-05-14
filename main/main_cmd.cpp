#include <iostream>
#include <string>

#include "mainer.h"

int main()
{
    Calculator Polsha;
    SortedMap<std::string, Polinom> peremSortedMap;
    BlackRedTree<std::string, Polinom> peremBlackRedTree;
    VectorHash<std::string, Polinom> peremHash;
    UnsortedMap<std::string, Polinom> peremUnsortedMap;
    AVLTree<std::string, Polinom> peremAVLTree;
    HashMap<std::string, Polinom> peremHashMap;
    int whatToUse = 1; //1 - SortedMap, 2 - BlackRedTree, 3 - Hash, 4 - UnsortedMap, 5 - AVLTree, 6 - HashMap
    while (true)
    {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        //
        if (input.empty())
        {
            continue;
        }
        //
        try
        {
            Polinom result;
            switch (whatToUse)
            {
            case 1:
                result = Polsha.ArithmeticCalculator(input, peremSortedMap);
                break;
            case 2:
                result = Polsha.ArithmeticCalculator(input, peremBlackRedTree);
                break;
            case 3:
                result = Polsha.ArithmeticCalculator(input, peremHash);
                break;
            case 4:
                result = Polsha.ArithmeticCalculator(input, peremUnsortedMap);
                break;
            case 5:
                result = Polsha.ArithmeticCalculator(input, peremAVLTree);
                break;
            case 6:
                result = Polsha.ArithmeticCalculator(input, peremHashMap);
                break;
            default:
                break;
            }
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& excep)
        {
            std::cout << "Error: " << excep.what() << std::endl;
        }
    }
    return 0;
}