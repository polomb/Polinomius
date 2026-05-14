#include "mainer.h"
#include <stdexcept>

Calculator::Calculator()
{
    sintaksis = std::make_unique<SintaksisAnalizer>();
    translator = std::make_unique<TranslatorPolski>();
}

