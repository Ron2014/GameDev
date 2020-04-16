#pragma once
#include <string>
#include <vector>

namespace Future {
class Path {
public:
    static std::string NormalizePath(const std::string& path);

    static bool DeepCreateDirectory(const std::string& path);
    static bool DeepSearchDirectory(const std::string& path, std::vector<std::string> &fileList, std::vector<std::string> &dirList);

	static bool FileExists(const std::string& path);
    static bool DirExists(const std::string& path);

	static bool FileRemove(const std::string& path);
	static bool DirRemove(const std::string& path);

	static unsigned int GetLastFileWriteTime(const std::string& path);
    static unsigned int GetFileSize(const std::string& path);

	static bool CollapseRelativeDirectories(std::string& path);

	static std::string& RemoveDuplicateSlashes(std::string& path);


	static std::string GetExtension(const std::string& path, bool includeDot = false);
	static std::string GetCleanFilename(const std::string& path);
	static std::string GetBaseFilename(const std::string& path, bool removePath = true);
	static std::string GetPath(const std::string& path);

	template<typename... Args>
	static std::string Combine(Args&&... args) {
		const char* pathes[] {args ...};
		std::string out;
		CombineInternal(out, pathes, sizeof(pathes)/sizeof(pathes[0]));
		return out;
	}

private:
	static void CombineInternal(std::string& out, const char** paths, int numPaths);
};
};