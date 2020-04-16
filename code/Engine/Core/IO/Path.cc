#include "StdFuture.h"
#include "Path.h"

#if FUTURE_WINDOWS
#include <windows.h>
#include <io.h>
#else
#include <errno.h>
#include <dirent.h>
#endif

#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>

#include "Core/String/StringHelper.h"

namespace Future {

static std::string&
ReplaceString(std::string& str, const std::string& from, const std::string& to) {
	std::string::size_type pos = str.find(from);
	while (pos != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos = str.find(from);
	}

	return str;
}


std::string 
Path::NormalizePath(const std::string& path) {
	std::string result(path);
	ReplaceString(result, "\\", "/");

	while (result.find("//") != std::string::npos) {
		ReplaceString(result, "//", "/");
	}
    return result;
}


static bool 
DeepCreateDirectory_(const std::string &path) {
    if (path.length() == 0) {
        return true;
    }
    else if (path.c_str()[path.length() - 1] == ':') {
        return true;
    }
    else if (strcmp(path.c_str(), "..") == 0
		|| strcmp(path.c_str(), ".") == 0) {
        return true;
    }

	std::size_t slash = path.find_last_of("/");
	if (slash != std::string::npos) {
		bool result = DeepCreateDirectory_(path.substr(0, slash));
        if (!result) {
            return false;
        }
    }

#if FUTURE_WINDOWS
	DWORD dwAttr = GetFileAttributes(path.c_str());
    if ((dwAttr == (DWORD)-1 && GetLastError() == ERROR_FILE_NOT_FOUND)) {
		if (!::CreateDirectory(path.c_str(), NULL))
            return false;
    }
    else if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        return false;
    }

    return true;
#else
    struct stat fileStat;
	if (-1 == stat(path.c_str(), &fileStat)) {
        if (ENOENT == errno) {
			if (-1 == mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP)) {
                return false;
            }
        }
    }
    return true;
#endif
}

bool 
Path::DeepCreateDirectory(const std::string& path) {
	std::string normalizePath = NormalizePath(path);
	return DeepCreateDirectory_(normalizePath);
}

bool
Path::DeepSearchDirectory(const std::string& path, std::vector<std::string> &fileList, std::vector<std::string> &dirList) {
#if FUTURE_WINDOWS
    _finddata_t file;
    std::string filepath = Path::Combine(path.c_str(), "*");
    intptr_t handle;

    if ( (int)(handle = _findfirst(filepath.c_str(), &file)) == -1 ) {
        return false;
    }

    do {
        if (file.attrib & _A_SUBDIR) {
            std::string s = file.name;
            if (s == "." || s == "..")
                continue;

            std::string dirname = Path::Combine(path.c_str(), file.name);
            dirList.push_back(dirname);
            DeepSearchDirectory(dirname.c_str(), fileList, dirList);

        } else {
            std::string filename = Path::Combine(path.c_str(), file.name);
            fileList.push_back(filename);
        }

    } while( _findnext(handle, &file) == 0 );

    _findclose(handle);

#else

    DIR *p_dir;
    struct dirent *p_dirent;

    if((p_dir = opendir(path.c_str())) == NULL) {
        return false;
    }

    while((p_dirent = readdir(p_dir))) {
        if (DT_REG == p_dirent->d_type) {
            // normal type file
			std::string filepath = Path::Combine(path.c_str(), p_dirent->d_name);
            fileList.push_back(filepath);

        }else if (DT_DIR == p_dirent->d_type) {
            // directory
            std::string s = p_dirent->d_name;
            if (s == "." || s == "..")
                continue;
                
            std::string dirpath = Path::Combine(path.c_str(), p_dirent->d_name);
            dirList.push_back(dirpath);

            DeepSearchDirectory(dirpath.c_str(), fileList, dirList);
        }
    }

    closedir(p_dir);

#endif
    return true;
}

bool
Path::FileExists(const std::string& path) {
#if FUTURE_WINDOWS
	DWORD dwAttr = GetFileAttributes(path.c_str());
	if (dwAttr == (DWORD)-1) {
		return false;
	} else if ( !(dwAttr & FILE_ATTRIBUTE_ARCHIVE) ) {
		return false;
	} else {
		return true;
	}
#else
	struct stat file;
	int res = stat(path.c_str(), &file);
	if (-1 != res && S_ISREG(file.st_mode)) {
		return true;
	}
	else {
		return false;
	}
#endif
}


bool 
Path::DirExists(const std::string& path) {
#if FUTURE_WINDOWS
    DWORD dwAttr = GetFileAttributes(path.c_str());
    if ((dwAttr == (DWORD)-1 && (GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_FILE_NOT_FOUND))) {
        return false;
    }
    else if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        return false;
    }

    return true;
#else
    struct stat fileStat;
    if (0 == stat(path.c_str(), &fileStat) && S_ISDIR(fileStat.st_mode)) {
        return true;
    }
    return false;
#endif
}

bool
Path::FileRemove(const std::string& path) {
	bool ret = true;
#if FUTURE_WINDOWS
	ret = DeleteFile(path.c_str());
#else
	ret = remove(path.c_str()) == 0;
#endif
	return ret;
}

bool
Path::DirRemove(const std::string& path) {
	bool ret = true;
#if FUTURE_WINDOWS
	ret = RemoveDirectory(path.c_str());
#else
	ret = remove(path.c_str()) == 0;
#endif
	return ret;
}

unsigned int
Path::GetLastFileWriteTime(const std::string& path) {
	struct stat buf;
	int result;
	result = stat(path.c_str(), &buf);
	if (result == 0) {
		return (long)buf.st_mtime;
	}
	return 0;
}

unsigned int 
Path::GetFileSize(const std::string& path) {
    std::fstream fs;

    fs.open(path, std::fstream::in | std::fstream::binary);
    if (!fs.is_open() || !fs.good()) {
        return 0;
    }

    fs.seekg(0, std::ios::end);
    return (unsigned int)fs.tellg();

    fs.close();
}


static std::string::size_type 
FindMax(std::string::size_type a, std::string::size_type b) {
	if (a == std::string::npos) {
		return b;
	}

	if (b == std::string::npos) {
		return a;
	}

	return a > b ? a : b;
}

bool 
Path::CollapseRelativeDirectories(std::string& path) {
	const char parentDir[] = "/..";
	const int parentDirLength = sizeof(parentDir)/ sizeof(parentDir[0]) - 1;

	for (;;) {
		// An empty path is finished
		if (path.empty())
			break;

		// Consider empty paths or paths which start with .. or /.. as invalid
		if (StringHelper::StartsWith(path, "..") || StringHelper::StartsWith(path, parentDir))
			return false;

		// If there are no "/.."s left then we're done
		const std::string::size_type index = path.find(parentDir);
		if (index == std::string::npos)
			break;

		std::string::size_type previousSeparatorIndex = index;
		for (;;) {
			// Find the previous slash
			previousSeparatorIndex = FindMax(0, path.rfind("/", previousSeparatorIndex - 1));

			// Stop if we've hit the start of the string
			if (previousSeparatorIndex == 0)
				break;

			// Stop if we've found a directory that isn't "/./"
			if ((index - previousSeparatorIndex) > 1 && (path[previousSeparatorIndex + 1] != '.' || path[previousSeparatorIndex + 2] != '/'))
				break;
		}

		// If we're attempting to remove the drive letter, that's illegal
		std::string::size_type colon = path.find(":", previousSeparatorIndex);
		if (colon != std::string::npos && colon < index)
			return false;

		path.erase(previousSeparatorIndex, index - previousSeparatorIndex + parentDirLength);

	}

	StringHelper::ReplaceStringInline(path, "./", "");

	return true;
}

std::string& 
Path::RemoveDuplicateSlashes(std::string& path) {
	while (path.find("//") != std::string::npos) {
		StringHelper::ReplaceStringInline(path, "//", "/");
	}
	return path;
}


static void CombineString(std::string &to, const std::string &from) {
    if (to.size() > 0 && to[to.length() - 1] != '/' && to[to.length() - 1] != '\\') {
		to += "/";
	}
	to += from;
}

void 
Path::CombineInternal(std::string& out, const char** paths, int numPaths) {
	f_assert(paths != nullptr && numPaths > 0);

	std::size_t outSize = 0;

	for (int i = 0; i < numPaths; ++i) {
		outSize += strlen(paths[i]) + 1;
	}

	out.clear();
	out.reserve(outSize);

	out += paths[0];

	for (int i = 1; i < numPaths; ++i) {
		CombineString(out, paths[i]);
	}
}


std::string 
Path::GetExtension(const std::string& path, bool includeDot) {
	const std::string fileName = GetCleanFilename(path);
	std::string::size_type dotPos = fileName.rfind(".");
	if (dotPos != std::string::npos) {
		return fileName.substr(dotPos + (includeDot ? 0 : 1));
	}

	return "";
}

std::string 
Path::GetCleanFilename(const std::string& path) {
	std::string::size_type pos = path.rfind("/");
	// in case we are using backslashes on a platform that doesn't use backslashes
	pos = FindMax(pos, path.rfind("\\"));

	if (pos != std::string::npos) {
		// if it was a trailing one, cut it (account for trailing whitespace?) and try removing path again
		if (pos == path.length() - 1) {
			return GetCleanFilename(path.substr(0, pos));
		}
		return path.substr(pos + 1);
	}
	return path;
}

std::string 
Path::GetBaseFilename(const std::string& path, bool removePath) {
	std::string str = removePath ? GetCleanFilename(path) : path;

	// remove the extension
	std::string::size_type pos = str.rfind(".");
	if (pos != std::string::npos) {
        return str.substr(0, pos);
	}
	return str;
}

std::string 
Path::GetPath(const std::string& path) {
	std::string::size_type pos = path.rfind("/");
	// in case we are using backslashes on a platform that doesn't use backslashes
	pos = FindMax(pos, path.rfind("\\"));
	if (pos != std::string::npos) {
		return path.substr(0, pos);
	}
	return "";
}

} // namespace Future