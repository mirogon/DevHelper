#include "Pusher.h"
#include "Poco/Zip/Compress.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Pipe.h"
#include "Poco//Process.h"

Pusher::Pusher(string dir) {
	this->dir = dir;
	this->binariesZipPath = dir + "/binaries.zip";

	DeleteZip();

	std::ofstream zipFile(binariesZipPath, std::ios::binary);
	Poco::Zip::Compress c(zipFile, true);

	Poco::DirectoryIterator it(dir);
	Poco::DirectoryIterator end;
	while (it != end) {
		std::cout << it->path() << std::endl;
		if (it->isFile()) {
			if (!it->isDirectory()) {
				if (it->path().find(".zip") == string::npos) {
					Poco::Path p(it->path());
					c.addFile(it->path(), p.getFileName());
				}
			}
		}
		++it;
	}

	c.close();
	zipFile.flush();
	zipFile.close();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::vector<std::string> args = { "-C", dir, "add", dir + "/binaries.zip"};
	Poco::Pipe outPipe;
	Poco::ProcessHandle ph = Poco::Process::launch("git", args, 0, &outPipe, 0);
	ph.wait();
	args = { "-C", dir, "commit","-m", "\"update\""};
	Poco::ProcessHandle ph2 = Poco::Process::launch("git", args, 0, &outPipe, 0);
	ph2.wait();
	args = { "-C", dir, "push","origin", "master"};
	Poco::ProcessHandle ph3 = Poco::Process::launch("git", args, 0, &outPipe, 0);
	ph3.wait();
}

void Pusher::DeleteZip() {
	Poco::File f(binariesZipPath);
	if (f.exists()) {
		f.remove();
	}
}
