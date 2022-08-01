
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
#define __LSRA_WARN
#define __LSRA_SHOW
#define __WARN
//#define __TRACE
//#define __INFO


enum EXIT_CODE {
    virtual_reg_full_error,
    CMPOPRANDREGERROR,
    DIVOPRANDREGERROR,
    RetReturnRegERROR,
    SUBOPRANDREGERROR,
    ADDOPRANDREGERROR,
    CmpOperandFpError,
    MULOPRANDREGERROR,
    FpNotInstrError,
    GetNegRegError,
    LoadOffsetOperandNotOne,
    ToManyArgsInBr,
    StorerOffsetOperandNotOne,
    UnknownIrInstruction,
    ConfiglictRegNotInStack,
    PassArgsError,
    GetIntRegsIsZero,
    CmpOpInValid,
    EXIT_CODE_ERROR_300,
    EXIT_CODE_ERROR_301,
    EXIT_CODE_ERROR_302,
    EXIT_CODE_ERROR_303,
    EXIT_CODE_ERROR_304,
    EXIT_CODE_ERROR_305,
    EXIT_CODE_ERROR_306,
    EXIT_CODE_ERROR_307,
    EXIT_CODE_ERROR_308,
    EXIT_CODE_ERROR_309,
    EXIT_CODE_ERROR_310,
    EXIT_CODE_ERROR_311,
    EXIT_CODE_ERROR_312,
    EXIT_CODE_ERROR_313,
    EXIT_CODE_ERROR_314,
    EXIT_CODE_ERROR_315,
    EXIT_CODE_ERROR_316,
    EXIT_CODE_ERROR_317,
    EXIT_CODE_ERROR_318,
    EXIT_CODE_ERROR_319,
    EXIT_CODE_ERROR_320,
    EXIT_CODE_ERROR_321,
    EXIT_CODE_ERROR_322,
    EXIT_CODE_ERROR_323,
    EXIT_CODE_ERROR_324,
    EXIT_CODE_ERROR_325,
    EXIT_CODE_ERROR_326,
    EXIT_CODE_ERROR_327,
    EXIT_CODE_ERROR_328,
    EXIT_CODE_ERROR_329,
    EXIT_CODE_ERROR_330,
    EXIT_CODE_ERROR_331,
    EXIT_CODE_ERROR_332,
    EXIT_CODE_ERROR_333,
    EXIT_CODE_ERROR_334,
    EXIT_CODE_ERROR_335,
    EXIT_CODE_ERROR_336,
    EXIT_CODE_ERROR_337,
    EXIT_CODE_ERROR_338,
    EXIT_CODE_ERROR_339,
    EXIT_CODE_ERROR_340,
    EXIT_CODE_ERROR_341,
    EXIT_CODE_ERROR_342,
    EXIT_CODE_ERROR_343,
    EXIT_CODE_ERROR_344,
    EXIT_CODE_ERROR_345,
    EXIT_CODE_ERROR_346,
    EXIT_CODE_ERROR_347,
    EXIT_CODE_ERROR_348,
    EXIT_CODE_ERROR_349,
    EXIT_CODE_ERROR_350,
    EXIT_CODE_ERROR_351,
    EXIT_CODE_ERROR_352,
    EXIT_CODE_ERROR_353,
    EXIT_CODE_ERROR_354,
    EXIT_CODE_ERROR_355,
    EXIT_CODE_ERROR_356,
    EXIT_CODE_ERROR_357,
    EXIT_CODE_ERROR_358,
    EXIT_CODE_ERROR_359,
    EXIT_CODE_ERROR_360,
    EXIT_CODE_ERROR_361,
    EXIT_CODE_ERROR_362,
    EXIT_CODE_ERROR_363,
    EXIT_CODE_ERROR_364,
    EXIT_CODE_ERROR_365,
    EXIT_CODE_ERROR_366,
    EXIT_CODE_ERROR_367,
    EXIT_CODE_ERROR_368,
    EXIT_CODE_ERROR_369,
    EXIT_CODE_ERROR_370,
    EXIT_CODE_ERROR_371,
    EXIT_CODE_ERROR_372,
    EXIT_CODE_ERROR_373,
    EXIT_CODE_ERROR_374,
    EXIT_CODE_ERROR_375,
    EXIT_CODE_ERROR_376,
    EXIT_CODE_ERROR_377,
    EXIT_CODE_ERROR_378,
    EXIT_CODE_ERROR_379,
    EXIT_CODE_ERROR_380,
    EXIT_CODE_ERROR_381,
    EXIT_CODE_ERROR_382,
    EXIT_CODE_ERROR_383,
    EXIT_CODE_ERROR_384,
    EXIT_CODE_ERROR_385,
    EXIT_CODE_ERROR_386,
    EXIT_CODE_ERROR_387,
    EXIT_CODE_ERROR_388,
    EXIT_CODE_ERROR_389,
    EXIT_CODE_ERROR_390,
    EXIT_CODE_ERROR_391,
    EXIT_CODE_ERROR_392,
    EXIT_CODE_ERROR_393,
    EXIT_CODE_ERROR_394,
    EXIT_CODE_ERROR_395,
    EXIT_CODE_ERROR_396,
    EXIT_CODE_ERROR_397,
    EXIT_CODE_ERROR_398,
    EXIT_CODE_ERROR_399,
    EXIT_CODE_ERROR_400,
    EXIT_CODE_ERROR_401,
    EXIT_CODE_ERROR_402,
    EXIT_CODE_ERROR_403,
    EXIT_CODE_ERROR_404,
    EXIT_CODE_ERROR_405,
    EXIT_CODE_ERROR_406,
    EXIT_CODE_ERROR_407,
    EXIT_CODE_ERROR_408,
    EXIT_CODE_ERROR_409,
    EXIT_CODE_ERROR_410,
    EXIT_CODE_ERROR_411,
    EXIT_CODE_ERROR_412,
    EXIT_CODE_ERROR_413,
    EXIT_CODE_ERROR_414,
    EXIT_CODE_ERROR_415,
    EXIT_CODE_ERROR_416,
    EXIT_CODE_ERROR_417,
    EXIT_CODE_ERROR_418,
    EXIT_CODE_ERROR_419,
    EXIT_CODE_ERROR_420,
    EXIT_CODE_ERROR_421,
    EXIT_CODE_ERROR_422,
    EXIT_CODE_ERROR_423,
    EXIT_CODE_ERROR_424,
    EXIT_CODE_ERROR_425,
    EXIT_CODE_ERROR_426,
    EXIT_CODE_ERROR_427,
    EXIT_CODE_ERROR_428,
    EXIT_CODE_ERROR_429,
    EXIT_CODE_ERROR_430,
    EXIT_CODE_ERROR_431,
    EXIT_CODE_ERROR_432,
    EXIT_CODE_ERROR_433,
    EXIT_CODE_ERROR_434,
    EXIT_CODE_ERROR_435,
    EXIT_CODE_ERROR_436,
    EXIT_CODE_ERROR_437,
    EXIT_CODE_ERROR_438,
    EXIT_CODE_ERROR_439,
    EXIT_CODE_ERROR_440,
    EXIT_CODE_ERROR_441,
    EXIT_CODE_ERROR_442,
    EXIT_CODE_ERROR_443,
    EXIT_CODE_ERROR_444,
    EXIT_CODE_ERROR_445,
    EXIT_CODE_ERROR_446,
    EXIT_CODE_ERROR_447,
    EXIT_CODE_ERROR_448,
    EXIT_CODE_ERROR_449,
    EXIT_CODE_ERROR_450,
    EXIT_CODE_ERROR_451,
    EXIT_CODE_ERROR_452,
    EXIT_CODE_ERROR_453,
    EXIT_CODE_ERROR_454,
    EXIT_CODE_ERROR_455,
    EXIT_CODE_ERROR_456,
    EXIT_CODE_ERROR_457,
    EXIT_CODE_ERROR_458,
    EXIT_CODE_ERROR_459,
    EXIT_CODE_ERROR_460,
    ERROR_TYPE,
    DYNAMIC_CAST_NULL_PTR,
    ERROR_DEFUALT,
};

#ifdef __ASSERT
#define MyAssert(info, cond, exit_code)                                           \
    do {                                                               \
        if (!(cond)) {                                                 \
            fprintf(stderr,"[%s:%d]" info RESET "\n", __FILE__, __LINE__); \
            exit(exit_code);                                                   \
        }                                                              \
    } while (0)
#else
#define MyAssert(info, cond, exit_code)
#endif

#ifdef __ERROR
#define ERROR(format, exit_code, ...)                                                          \
    do {                                                                            \
        fprintf(stderr,"[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(exit_code);                                                                    \
    } while (0)
#else
#define ERROR(format, exit_code, ...)
#endif


#ifdef __WARN
#define WARNNING(format, ...)                                  \
    do {                                                                             \
        printf(BLUE "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define WARNNING(format, ...)
#endif


#ifdef __LSRA_WARN
#define LSRA_WARNNING(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[LSRA:%d]" format RESET "\n", __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define LSRA_WARNNING(format, ...)
#endif

#ifdef __LSRA_SHOW
#define LSRA_SHOW(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW  format RESET , ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define LSRA_SHOW(format, ...)
#endif

#ifdef __INFO
#define INFO(format, ...)                                                            \
    do {                                                                             \
        printf(BLUE "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define INFO(format, ...)
#endif

#ifdef __TRACE
#define TRACE(format, ...)                                                            \
    do {                                                                              \
        printf(GREEN "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                               \
    } while (0)
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
        : log_type(type), log_file_name(file_name), to_file(to_file), to_screen(to_screen), print_file_position(false) {
        std::string name = file_name + ".log";
        if(to_file)
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

// #define SYSY_BUILDER_
#ifdef SYSY_BUILDER_
#define SYSY_BUILDER(format, ...)                                  \
    // do {                                                                             \
    //     printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    //     fflush(stdout);                                                              \
    // } while (0)
#else
#define SYSY_BUILDER(format, ...)
#endif

#endif// EASYCC_ERROR_H
