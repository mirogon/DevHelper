#pragma once
#include "global.h"

class Pusher{
private:
	string dir;
	string binariesZipPath;
public:
	Pusher() = delete;
	Pusher(string dir);

private:
	void DeleteZip();

};