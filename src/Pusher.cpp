#include "Pusher.h"
#include "Poco/Zip/Compress.h"

Pusher::Pusher(string dir) {
	//Compress files
	std::ofstream zipFile("binaries.zip", std::ios::binary);
	Poco::Zip::Compress c(zipFile, true);
	Poco::Path p(dir);
	c.addRecursive(p);
	c.close();

	string cmd = "git add --all -C " + dir;
	system(cmd.c_str());
	cmd = "git commit -m \"update\" -C " + dir;
	system(cmd.c_str());
	cmd = "git push origin master -C " + dir;
	system(cmd.c_str());
}
