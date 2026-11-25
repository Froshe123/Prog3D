#pragma once

#include <cstdarg>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>

// glm  operator<< overload, pour la fonction print
template <typename GLMType, typename = decltype(glm::to_string(std::declval<GLMType>()))> // demonic type fuckery
std::ostream& operator<< (std::ostream& stream, const GLMType& obj) {
    return stream << glm::to_string(obj);
}
template <typename T>
std::ostream& operator<< (std::ostream& stream, const std::vector<T> & obj){
    stream << "[";

    if (obj.empty()){
        stream << "]";
        return stream;
    }

    for (int i = 0; i < obj.size()-1; ++i){
        stream << obj[i] << ", ";
    }
    stream << obj.back() << "]";
    return stream;
}

namespace Utils
{
    // I'm not yet fluent enough in the dark arts to understand it (https://stackoverflow.com/questions/27375089/what-is-the-easiest-way-to-print-a-variadic-parameter-pack-using-stdostream)
    // update: now i am. Adding comments: 
    template <typename Arg, typename... Args> // variadic template arguments
    void print(Arg&& arg, Args&&... args){ // passing everything as generic r-values. When called on an lvalue, those are cast to V&, V = T&. (see "perfect forwarding rule")
        std::cout << std::forward<Arg>(arg); // std::forward casts the argument to either an lvalue reference, or an rvalue reference. See https://stackoverflow.com/questions/28828159/usage-of-stdforward-vs-stdmove)
        ((std::cout << ' ' << std::forward<Args>(args)), ...); // see "fold expression"
        std::cout << std::endl;
    }


    inline int posmod(int i, int n)
    {
        return (i % n + n) % n;
    }

    template <const char name>
    inline void time(const std::string msg = "", const bool restart = false)
    {
        static bool started = false;
        static std::chrono::steady_clock::time_point last_time;

        std::chrono::steady_clock::time_point new_time = std::chrono::steady_clock::now();

        if (!started or restart)
        {
            last_time = new_time;
            started = true;
            return;
        }
        std::cout << "Delta for timer '" << name << "' : " << std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_time).count() << "ms";
        if (msg != "")
            std::cout << " (with message '" << msg << "')";

        std::cout << std::endl;
    }
    template <const char name>
    inline void time(const bool restart) {time<name>("", restart);}


void printShaderData(unsigned int ProgramID);

// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replaceInString(std::string& str, const std::string& from, const std::string& to);
}