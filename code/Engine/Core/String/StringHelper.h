#pragma once
#include <cstring>
#include <vector>
#include <string>

namespace Future {

class StringHelper {
public:
    static int SplitsLines(const std::string& str, std::vector<std::string>& outLines);
    static int Tokenize(const std::string& str, const char* delims, char fence, std::vector<std::string>& outTokens);
    static std::string Format(const char* fmt_str, ...);
    static std::string FormatVaList(const char* fmt_str, va_list args);

    static std::string& FormatToString(std::string& out_str, const char* fmt_str, ...);

    static std::string &ToLower(std::string& str);
    static std::string &ToUpper(std::string& str);

    static std::string &LTrim(std::string &s);
    static std::string &RTrim(std::string &s);
    static std::string &Trim(std::string &s);

    static bool EqualsIgnoreCase(const std::string &s1, const std::string &s2);

    static bool StartsWith(const std::string &s, const std::string &prefix) {
        return prefix.length() > 0 && !std::strncmp(s.c_str(), prefix.c_str(), prefix.length());
    }

    static bool ReadFileToString(std::string& fileData, const std::string& from);

    static std::string& ReplaceStringInline(std::string &str, const std::string& from, const std::string& to);

    static std::string DumpToHexString(const void *p, int size);

    static bool Match(char const *needle, char const *haystack) {
        for (; *needle != '\0'; ++needle) {
            switch (*needle) {
            case '?': ++haystack;
                break;
            case '*': {
                size_t max = strlen(haystack);
                if (needle[1] == '\0' || max == 0)
                    return true;
                for (size_t i = 0; i < max; i++)
                    if (Match(needle + 1, haystack + i))
                        return true;
                return false;
            }
            default:
                if (*haystack != *needle)
                    return false;
                ++haystack;
            }
        }
        return *haystack == '\0';
    }

#if FUTURE_WINDOWS
    // Do not assert in this function since it is used by the asssertion code!
    static std::wstring SysMultiByteToWide(const std::string& mb, unsigned int code_page);

    // Do not assert in this function since it is used by the asssertion code!
    static std::string SysWideToMultiByte(const std::wstring& wide, unsigned code_page);

#endif
};

} //namespace Future