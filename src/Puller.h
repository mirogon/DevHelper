#include "global.h"
#include <sstream>
#include "IUpdatable.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Pipe.h"
#include "Poco/Process.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/StreamCopier.h"
#include "Poco/PipeStream.h"

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
		if (pullTimer.ElapsedSec() >= 5 ) {
			if (NewUpdateAvailable()) {
				PullUpdates(remoteName, branchName);
			}
		}
	}

private:
	void PullUpdates(const string& remoteName, const string& branchName) {
		RemoveFiles();

		std::vector<string> args = { "-C", dir, "stash" };
		Poco::ProcessHandle phGitStash = Poco::Process::launch("git", args, 0, 0, 0);
		phGitStash.wait();

		args = { "-C", dir, "pull", remoteName, branchName };
		Poco::ProcessHandle phGitPull = Poco::Process::launch("git", args, 0, 0, 0);
		phGitPull.wait();

		//Unzip
		std::ifstream zipStream(dir + "/binaries.zip", std::ios::binary);
		Poco::Zip::Decompress d(zipStream, dir);
		d.decompressAllFiles();
		zipStream.close();

		pullTimer.Restart();
	}
	bool NewUpdateAvailable() {
		std::vector<string> args = { "-C", dir, "fetch"};
		Poco::ProcessHandle phFetch = Poco::Process::launch("git", args, 0, 0, 0);
		phFetch.wait();

		args = { "-C", dir, "log", "HEAD..origin/master", "--oneline"};
		Poco::Pipe gitLogPipe;
		Poco::ProcessHandle phLog = Poco::Process::launch("git", args, 0, &gitLogPipe, 0);
		Poco::PipeInputStream inputStream(gitLogPipe);
		std::stringstream stringStream;
		Poco::StreamCopier::copyStream(inputStream, stringStream);
		phLog.wait();

		if (stringStream.str().length() > 3) {
			return true;
		}
		return false;
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
			++it;
		}

	}
};

