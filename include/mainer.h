#ifndef __MAINER__
#define __MAINER__

#include <string>
#include <memory>
#include <unordered_map>

#include "Polinom.h"
#include "m_leksich_analizer.h"
#include "m_sintaksis_analizer.h"
#include "m_translator_polski.h"
#include "SortedMap.h"
#include "BlackRedTree.h"
#include "Hash.h"
#include "UnsortedMap.h"
#include "AVLTree.h"
#include "HashMap.h"

class Calculator
{
    std::unique_ptr<LeksichAnalizer> leksich;
    std::unique_ptr<SintaksisAnalizer> sintaksis;
    std::unique_ptr<TranslatorPolski> translator;

public:
    Calculator();   

    template<typename ContainerType>
    Polinom ArithmeticCalculator(const std::string& expression, ContainerType& perem)
    {
        //Leksika
        leksich = std::make_unique<LeksichAnalizer>(expression);
        std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
        //Sintaksis
        sintaksis->isCorrect(tokens, perem);
        //PolskayaZapis
        std::vector<std::unique_ptr<Token>> polishTokens = translator->toPolishNotation(tokens);
        //Calculation
        Polinom result = translator->calculate(polishTokens, perem);
        return result;
    }
};


#endif

