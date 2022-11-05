#include "global.h"
#include "IUpdatable.h"
#include "Poco/Zip/Decompress.h"

class Puller : public IUpdatable{
private:
	string dir;
	string remoteName;
	string branchName;
	Timer pullTimer;

public:

	Puller() = delete;

	Puller(string dir, const string& remoteName, const string& branchName) {
		this->dir = dir;
		this->remoteName = remoteName;
		this->branchName = branchName;
		pullTimer.Restart();
	}

	void Update() {
		if (pullTimer.ElapsedSec() >= 10) {
			PullUpdates(remoteName, branchName);
			pullTimer.Restart();
		}
	}

private:
	void PullUpdates(const string& remoteName, const string& branchName) {
		string pullCmd = "git -C " + dir + " " + remoteName + " " + branchName;
		system(pullCmd.c_str());
		std::this_thread::sleep_for(std::chrono::seconds(5));
		pullCmd = "git pull " + remoteName + " " + branchName + " -C " + dir;
		system(pullCmd.c_str());

		//Unzip
		std::ifstream zipStream("binaries.zip", std::ios::binary);
		Poco::Zip::Decompress d(zipStream, dir);
		d.decompressAllFiles();
	}
};

