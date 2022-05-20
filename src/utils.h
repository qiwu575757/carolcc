
#ifndef EASYCC_ERROR_H
#define EASYCC_ERROR_H

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>

#define __DEBUG 
#define __WARN
#define __TRACE
#define __INFO

#ifdef __DEBUG
#define ERROR(format, ...) \
        do{\
                printf("\x1b[32m""%s:%d " format "\x1b[0m\n",__FILE__,__LINE__,##__VA_ARGS__);\
                exit(1);\
        }\
        while(0)
#else 
#define ERROR(format, ...) 
#endif

#ifdef __WARN
#define WARNNING(format, ...) \
            printf("\x1b[93m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define WARNNING(format, ...) 
#endif


#ifdef __INFO
#define INFO(format, ...) \
            printf("\x1b[34m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define INFO(format, ...) 
#endif

#ifdef __TRACE
#define TRACE(format, ...) \
            printf("\x1b[90m" format "\x1b[0m",##__VA_ARGS__); 
#else 
#define TRACE(format, ...) 
#endif
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
class logger {
private:
  std::string log_file_name;
  // 是否输出到文件
  bool to_file;
  // 是否输出到屏幕
  bool to_screen;
  FILE *f;

public:
  void set_file(bool flag) { this->to_file = flag; }
  void set_screen(bool flag) { this->to_screen = flag; }
  explicit logger(const std::string &file_name, bool to_screen, bool to_file)
      : log_file_name(file_name), to_screen(to_screen), to_file(to_file) {
    std::string name = file_name + ".log";
    f = fopen(name.c_str(), "w+");
  }
  explicit logger(const std::string &file_name, bool to_screen)
      : logger(file_name, to_screen, false){};
  explicit logger(const std::string &file_name) : logger(file_name, true){};

  void print(const char *fmt, ...) {
    if (to_screen) {
      va_list args;
      va_start(args, fmt);
      printf(RED);
      printf("[%s:%d]", __FILE__,__LINE__);
      printf(fmt, args);
      printf(RESET);
      va_end(args);
    }
    if (to_file) {
      va_list args;
      va_start(args, fmt);
      fprintf(f, "[%s:%d]", __FILE__,__LINE__);
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

#endif // EASYCC_ERROR_H
