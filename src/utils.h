
#ifndef EASYCC_ERROR_H
#define EASYCC_ERROR_H

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#define __ERROR
#define __ASSERT
#define __DEBUG
#define __WARN
#define __TRACE
#define __INFO

#ifdef __ASSERT
#define MyAssert(info, cond)                                           \
    do {                                                               \
        if (!(cond)) {                                                 \
            printf(RED "[%s:%d]" info RESET "\n", __FILE__, __LINE__); \
            exit(2);                                                   \
        }                                                              \
    } while (0)
#else
#define MyAssert(info, cond)
#endif

#ifdef __ERROR
#define ERROR(format, ...)                                                     \
    do {                                                                       \
        printf(RED "[%s:%d]" format RESET"\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(3);                                                               \
    } while (0)
#else
#define ERROR(format, ...)
#endif


#ifdef __WARN
#define WARNNING(format, ...) \
    do { \
        printf("\x1b[93m" format "\x1b[0m \n", ##__VA_ARGS__); \
        fflush(stdout); \
    } while(0)
#else
#define WARNNING(format, ...)
#endif


#ifdef __INFO
#define INFO(format, ...) \
    do {        \
        printf(BLUE "[%s:%d]" format RESET"\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                               \
    } while(0)
#else
#define INFO(format, ...)
#endif

#ifdef __TRACE
#define TRACE(format, ...) \
    do {        \
        printf(GREEN "[%s:%d]" format RESET"\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                               \
    } while(0)
#else
#define TRACE(format, ...)
#endif

enum class LOG_TYPE {
    ERROR,
    DEBUG,
    WARN,
    TRACE,
    INFO,
};


class logger {
private:
    LOG_TYPE log_type;
    std::string log_file_name;
    // 是否输出到文件
    bool to_file;
    // 是否输出到屏幕
    bool to_screen;
    // does logger print the caller position;
    bool print_file_position;
    FILE *f;

    void open_color() {

        switch (log_type) {
            case LOG_TYPE::ERROR:
                printf(RED);
                //                printf("ERROR|");
                break;
            case LOG_TYPE::DEBUG:
                printf(GREEN);
                //                printf("DEBUG|");
                break;
            case LOG_TYPE::WARN:
                printf(YELLOW);
                //                printf("WARN|");
                break;
            case LOG_TYPE::TRACE:
                printf(MAGENTA);
                //                printf("TRACE|");
                break;
            case LOG_TYPE::INFO:
                printf(BLUE);
                //                printf("INFO|");
                break;
        }
    }
    void close_color() {
        printf(RESET);
    }

public:
    explicit logger(const std::string &file_name, LOG_TYPE type, bool to_screen, bool to_file)
        : log_file_name(file_name), log_type(type), to_screen(to_screen), to_file(to_file), print_file_position(false) {
        std::string name = file_name + ".log";
        f = fopen(name.c_str(), "w+");
    }
    explicit logger(const std::string &file_name, LOG_TYPE type, bool to_screen)
        : logger(file_name, type, to_screen, false){};
    explicit logger(const std::string &file_name, LOG_TYPE type) : logger(file_name, type, true){};
    explicit logger(const std::string &file_name) : logger(file_name, LOG_TYPE::INFO){};
    void set_file(bool flag) { this->to_file = flag; }
    void set_screen(bool flag) { this->to_screen = flag; }
    void set_caller_position(bool flag) {
        this->print_file_position = flag;
    }
    void set_level(LOG_TYPE type) {
        this->log_type = type;
    }

    void print(const char *fmt, ...) {
        if (to_screen) {
            va_list args;
            va_start(args, fmt);

            open_color();

            if (print_file_position)
                printf("[%s:%d] ", __FILE__, __LINE__);
            printf(fmt, args);

            close_color();

            va_end(args);
        }
        if (to_file) {
            va_list args;
            va_start(args, fmt);
            fprintf(f, "[%s:%d]", __FILE__, __LINE__);
            fprintf(f, fmt, args);
            va_end(args);
        }
    }
    void println(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        print(fmt, args);
        printf("\n");
        va_end(args);
    }
    ~logger() { fclose(f); }
};
class error_log : public logger {
public:
    explicit error_log(const std::string &name) : logger(name, LOG_TYPE::ERROR){};
};
class debug_log : public logger {
public:
    explicit debug_log(const std::string &name) : logger(name, LOG_TYPE::DEBUG){};
};
class warn_log : public logger {
public:
    explicit warn_log(const std::string &name) : logger(name, LOG_TYPE::WARN){};
};
class info_log : public logger {
public:
    explicit info_log(const std::string &name) : logger(name, LOG_TYPE::INFO){};
};
class trace_log : public logger {
public:
    explicit trace_log(const std::string &name) : logger(name, LOG_TYPE::TRACE){};
};

//error_log test("errortest");
//test.println("hello world");
//
//info_log test2("errortest");
//test2.println("hello world");
//warn_log test3("errortest");
//test3.println("hello world");
//trace_log test4("errortest");
//test4.println("hello world");
//debug_log test5("errortest");
//test5.set_caller_position(false);
//test5.println("hello world");


#endif// EASYCC_ERROR_H
