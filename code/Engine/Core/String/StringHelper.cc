#include "StdFuture.h"

#include <memory>
#include <cstdio>
#include <stdarg.h>
#include <cctype>
#include <locale>
#include <functional> 
#include <fstream>
#include <algorithm>
#include <cstring>


#include "StringHelper.h"

namespace Future {

int
StringHelper::SplitsLines(const std::string& str, std::vector<std::string>& outLines) {
    outLines.clear();
    if (!str.empty()) {
        const char* ptr = str.c_str();
        const char* end = ptr + str.size();

        const char* c = ptr;

        while (*ptr) {
            if (*ptr == '\r' && *(ptr + 1) == '\n') {
                outLines.push_back(std::string(c, ptr));
                c = ptr + 2;
                ptr += 2;
            }
            else if (*ptr == '\n' || *ptr == '\r') {
                outLines.push_back(std::string(c, ptr));
                c = ptr + 1;
                ptr++;
            }
            else {
                ptr++;
            }
        }

        if (c != ptr) {
            outLines.push_back(std::string(c, ptr));
        }
    }
    return (int)outLines.size();
}

int
StringHelper::Tokenize(const std::string& str, const char* delims, char fence, std::vector<std::string>& outTokens) {
    outTokens.clear();
    if (!str.empty()) {
        const char* ptr = str.c_str();
        const char* end = ptr + str.size();
        while (ptr < end) {
            const char* c;

            // skip white space
            while (*ptr && std::strchr(delims, *ptr)) {
                ptr++;
            }
            if (*ptr) {
                // check for fenced area
                if ((fence == *ptr) && (0 != (c = std::strchr(++ptr, fence)))) {
                    outTokens.emplace_back(ptr, c);
                    ptr = c + 1;
                }
                else if (0 != (c = std::strpbrk(ptr, delims))) {
                    outTokens.emplace_back(ptr, c);
                    ptr = c;
                }
                else {
                    outTokens.emplace_back(ptr);
                    break;
                }
            }
        }
    }
    return (int)outTokens.size();
}

std::string
StringHelper::Format(const char* fmt_str, ...) {
    va_list	args;

    va_start(args, fmt_str);
    size_t size = std::vsnprintf(nullptr, 0, fmt_str, args) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    va_end(args);
    va_start(args, fmt_str);
    std::vsnprintf(buf.get(), size, fmt_str, args);
    va_end(args);

    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::string
StringHelper::FormatVaList(const char* fmt_str, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);

    size_t size = std::vsnprintf(nullptr, 0, fmt_str, argsCopy) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    std::vsnprintf(buf.get(), size, fmt_str, args);
    va_end(argsCopy);

    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::string&
StringHelper::FormatToString(std::string& out_str, const char* fmt_str, ...) {
    va_list	args;

    va_start(args, fmt_str);
    size_t size = std::vsnprintf(nullptr, 0, fmt_str, args) + 1; // Extra space for '\0'

    auto oldStrSize = out_str.size();
    out_str.resize(size + oldStrSize);

    va_end(args);
    va_start(args, fmt_str);
    std::vsnprintf((char*)&out_str.data()[oldStrSize], size, fmt_str, args);
    va_end(args);

    out_str.resize(size + oldStrSize - 1);// We don't want the '\0' inside
    return out_str;
}

std::string&
StringHelper::ToLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string&
StringHelper::ToUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string&
StringHelper::LTrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string&
StringHelper::RTrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string &
StringHelper::Trim(std::string &s) {
    return LTrim(RTrim(s));
}

bool
StringHelper::EqualsIgnoreCase(const std::string &s1, const std::string &s2) {
    std::size_t count = s1.length();
    if (count != s2.length()) {
        return false;
    }

    const char* str1 = s1.c_str();
    const char* str2 = s2.c_str();

    for (; (*str1 || *str2) && count > 0; str1++, str2++, count--) {
        char char1 = std::tolower(*str1);
        char char2 = std::tolower(*str2);
        if (char1 != char2) {
            return false;
        }
    }
    return true;
}


bool
StringHelper::ReadFileToString(std::string& fileData, const std::string& from) {
    std::fstream fs;
    fs.open(from, std::fstream::in | std::fstream::binary);

    if (!fs.is_open() || !fs.good()) {
        fs.close();
        return false;
    }
    fileData.clear();

    fileData.append((std::istreambuf_iterator<char>(fs)),
                    std::istreambuf_iterator<char>());
    fs.close();
    return true;
}

std::string&
StringHelper::ReplaceStringInline(std::string &str, const std::string& from, const std::string& to) {
    std::string::size_type pos = str.find(from);
    while (pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos = str.find(from);
    }
    return str;
}


std::string 
StringHelper::DumpToHexString(const void *p, int size) {
    std::string ret;
    char buf[4];
    for (int i = 0; i < size; i++) {
        char c = ((char *)p)[i];
        if (isalnum(c) || isprint(c)) {
            ret.append(1, c);
        }
        else {
            switch (c) {
            case '\r':
                ret.append("\\r", 2);
                break;
            case '\n':
                ret.append("\\n", 2);
                break;
            default:
                std::sprintf(buf, "\\%02x", (unsigned char)c);
                ret.append(buf, 3);
            }
        }
    }
    return ret;
}

#if FUTURE_WINDOWS
std::wstring 
StringHelper::SysMultiByteToWide(const std::string& mb, unsigned int code_page) {
    if (mb.empty())
        return std::wstring();

    int mb_length = static_cast<int>(mb.length());
    // Compute the length of the buffer.
    int charcount = MultiByteToWideChar(code_page, 0,
                                        mb.data(), mb_length, NULL, 0);
    if (charcount == 0)
        return std::wstring();

    std::wstring wide;
    wide.resize(charcount);
    MultiByteToWideChar(code_page, 0, mb.data(), mb_length, &wide[0], charcount);

    return wide;
}


std::string 
StringHelper::SysWideToMultiByte(const std::wstring& wide, unsigned code_page) {
    int wide_length = static_cast<int>(wide.length());
    if (wide_length == 0)
        return std::string();

    // Compute the length of the buffer we'll need.
    int charcount = WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
                                        NULL, 0, NULL, NULL);
    if (charcount == 0)
        return std::string();

    std::string mb;
    mb.resize(charcount);
    WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
                        &mb[0], charcount, NULL, NULL);

    return mb;
}
#endif

} //namespace Future