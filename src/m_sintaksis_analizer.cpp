#include "m_sintaksis_analizer.h"

#include <string>//For string and +
#include <vector>//For vector and +
#include <stdexcept>
#include <cctype> //For isdigit and isalpha
#include <stack> //For stack and +
#include <algorithm> //For find

#include "token.h"
#include "t_polinom.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

SintaksisAnalizer::SintaksisAnalizer() {}

