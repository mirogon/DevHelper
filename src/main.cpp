#include "cxxopts.hpp"
#include "Puller.h"
#include "Pusher.h"

/* Requirements
* General: DevHelper is a system wide executable (using PATH) that can do stuff to help and ease development
* 
* 1. DevHelper pullzip <dir> : (continuously) Automatically execute git stash, git pull in dir every 10 secs
* 2. DevHelper pushzip <dir>: (once) Create a zip containing all files in dir, add, commit, push the zip
*/

int main(int argc, char* argv[]) {
	Logger logger{ make_shared<StdTimeSource>() };
	logger.AddLogDestination(make_shared<ConsoleLogDestination>());

	cxxopts::Options options("DevHelper", "Development auxiliary tool");
	options.add_options()
		("pullzip", "Continuously pull newest commit from origin master and unzip it", cxxopts::value<string>())
		("pushzip", "Create a zip containing all files in dir, add, commit, push the zip", cxxopts::value<string>());

	auto result = options.parse(argc, argv);

	std::vector<UpdatablePtr> services = std::vector<UpdatablePtr>();

	if (result.count("pushzip") > 0) {
		string pushDir = result["pushzip"].as<string>();
		Pusher p(pushDir);
	}

	if (result.count("pullzip") > 0) {
		string pullDir = result["pullzip"].as<string>();
		services.push_back( std::shared_ptr<Puller>( new Puller(pullDir, "origin", "master") ) );
	}

	while (true) {
		for (auto s : services) {
			s->Update();
		}
	}

	return 0;
}