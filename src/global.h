#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <fstream>
#include <M1/Logger/Logger.h>
#include <M1/Timer/Timer.h>

using std::cout;
using std::exception;
using std::shared_ptr;
using std::make_shared;
using std::string;
using namespace m1;

class Global {
public:
	static Logger logger;
};
