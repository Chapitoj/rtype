/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** StringUtils
*/

#ifndef STRINGUTILS_HPP_
    #define STRINGUTILS_HPP_

    #include <algorithm>
    #include <cctype>
    #include <string>

namespace utils {
    class StringUtils {
        public:
            ~StringUtils() = default;

            static std::string toLower(const std::string &str)
            {
                std::string lowerStr = str;
                std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
                std::string::iterator _ = lowerStr.erase(std::remove(lowerStr.begin(), lowerStr.end(), ' '), lowerStr.end());
                return lowerStr;
            }

        private:
            StringUtils();
    };
}

#endif /* !STRINGUTILS_HPP_ */
