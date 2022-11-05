#include "global.h"

Logger Global::logger{std::make_shared<StdTimeSource>()};