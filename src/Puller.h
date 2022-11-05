#include "global.h"
#include "IUpdatable.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Pipe.h"
#include "Poco/Process.h"
#include "Poco/DirectoryIterator.h"

class Puller : public IUpdatable{
private:
	string dir;
	string remoteName;
	string branchName;
	Timer pullTimer;
	bool firstUpdate;

public:

	Puller() = delete;

	Puller(string dir, const string& remoteName, const string& branchName) {
		this->dir = dir;
		this->remoteName = remoteName;
		this->branchName = branchName;
		firstUpdate = true;
		pullTimer.Restart();
	}

	void Update() {
		if (pullTimer.ElapsedSec() >= 50 || firstUpdate) {
			firstUpdate = false;
			Global::logger.Log("Update...");
			PullUpdates(remoteName, branchName);
		}
	}

private:
	void PullUpdates(const string& remoteName, const string& branchName) {
		Global::logger.Log("PullUpdates");
		//RemoveFiles();
		Global::logger.Log("Removed Files");

		std::vector<string> args{ "-C", dir, "stash" };
		Poco::ProcessHandle ph = Poco::Process::launch("git", args, 0, 0, 0);
		ph.wait();
		Global::logger.Log("Stashed");
		args = { "-C", dir, "pull", remoteName, branchName };
		Poco::ProcessHandle ph2 = Poco::Process::launch("git", args, 0, 0, 0);
		ph2.wait();

		//Unzip
		std::ifstream zipStream(dir + "/binaries.zip", std::ios::binary);
		Poco::Zip::Decompress d(zipStream, dir);
		d.decompressAllFiles();
		zipStream.close();

		pullTimer.Restart();
	}
	void RemoveFiles() {
		Poco::DirectoryIterator it(dir);
		Poco::DirectoryIterator end;
		while (it != end) {
			Poco::Path path(it->path());
			Poco::File file(*it);

			if (file.exists()) {
				if (file.isFile()) {
					if (!file.isDirectory()) {
						Global::logger.Log("Removing " + file.path());
						file.remove();
					}
				}
			}
		}

	}
};

