#pragma once
#include <memory>

class IUpdatable {
public:
	virtual void Update() = 0;
};

typedef std::shared_ptr<IUpdatable> UpdatablePtr;
