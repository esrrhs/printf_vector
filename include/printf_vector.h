#pragma once

#include <string>
#include <vector>
#include "string.h"

namespace printf_vector {

class input_interface {
public:
    virtual ~input_interface() {}

    virtual void reset_index() = 0;

    virtual long long get_int(int &err) = 0;

    virtual double get_double(int &err) = 0;

    virtual const char *get_string(int &err) = 0;

    virtual void *get_pointer(int &err) = 0;
};

class vector_input;

class input_var {
public:
    friend class vector_input;

    input_var(int i) : ll_(i) {}

    input_var(long long ll) : ll_(ll) {}

    input_var(double d) : d_(d) {}

    input_var(const char *s) : s_(s) {}

    input_var(void *p) : p_(p) {}

private:
    long long ll_ = 0;
    double d_ = 0;
    const char *s_ = "";
    void *p_ = nullptr;
};

class vector_input : public input_interface {
public:
    vector_input() : index_(0) {
    }

    void add(int i) {
        args_.push_back(input_var(i));
    }

    void add(long long ll) {
        args_.push_back(input_var(ll));
    }

    void add(double d) {
        args_.push_back(input_var(d));
    }

    void add(const char *s) {
        args_.push_back(input_var(s));
    }

    void add(void *p) {
        args_.push_back(input_var(p));
    }

    virtual void reset_index() override {
        index_ = 0;
    }

    virtual long long get_int(int &err) override {
        if (index_ >= args_.size()) {
            err = -1;
            return 0;
        }
        return args_[index_++].ll_;
    }

    virtual double get_double(int &err) override {
        if (index_ >= args_.size()) {
            err = -1;
            return 0;
        }
        return args_[index_++].d_;
    }

    virtual const char *get_string(int &err) override {
        if (index_ >= args_.size()) {
            err = -1;
            return "";
        }
        return args_[index_++].s_;
    }

    virtual void *get_pointer(int &err) override {
        if (index_ >= args_.size()) {
            err = -1;
            return 0;
        }
        return args_[index_++].p_;
    }

private:
    std::vector<input_var> args_;
    size_t index_ = 0;
};

class output_interface {
public:
    virtual ~output_interface() {}

    virtual void write_string(const char *str, size_t len) = 0;

    virtual size_t get_length() = 0;
};

#define ADVANCE_IN_FORMAT_STRING(cptr_) do { (cptr_)++; if (!*(cptr_)) return -1; } while(0)

#define CHECK_ERR_IN_FORMAT_STRING(err) do { if (err) return err; } while(0)

typedef unsigned int printf_flags_t;

// internal flag definitions
static const int FLAGS_SIGNED = (1U << 14U);

template<typename T>
static inline void
format_string(output_interface *output, const char *fmt, bool has_width_input, bool has_precision_input,
              long long width_input, long long precision_input, T t) {
    if (has_width_input && has_precision_input) {
        int sz = std::snprintf(nullptr, 0, fmt, width_input, precision_input, t);
        std::vector<char> buf(sz + 1);
        std::snprintf(&buf[0], buf.size(), fmt, width_input, precision_input, t);
        output->write_string(&buf[0], sz);
    } else if (has_precision_input) {
        int sz = std::snprintf(nullptr, 0, fmt, precision_input, t);
        std::vector<char> buf(sz + 1);
        std::snprintf(&buf[0], buf.size(), fmt, precision_input, t);
        output->write_string(&buf[0], sz);
    } else if (has_width_input) {
        int sz = std::snprintf(nullptr, 0, fmt, width_input, t);
        std::vector<char> buf(sz + 1);
        std::snprintf(&buf[0], buf.size(), fmt, width_input, t);
        output->write_string(&buf[0], sz);
    } else {
        int sz = std::snprintf(nullptr, 0, fmt, t);
        std::vector<char> buf(sz + 1);
        std::snprintf(&buf[0], buf.size(), fmt, t);
        output->write_string(&buf[0], sz);
    }
}

static inline int format_string_loop(output_interface *output, const char *format, input_interface *args) {

    int err = 0;

    char tmp_format_buffer[strlen(format)];
    const char *tmp_format_start = 0;
    const char *tmp_format_end = 0;

    while (*format) {
        if (*format != '%') {
            // A regular content character
            output->write_string(format, 1);
            format++;
            continue;
        }

        tmp_format_start = format;

        // We're parsing a format specifier: %[flags][width][.precision][length]
        ADVANCE_IN_FORMAT_STRING(format);

        printf_flags_t flags = 0;

        bool has_width_input = false;
        long long width_input = 0;

        // evaluate width field
        if (*format == '*') {
            has_width_input = true;
            width_input = args->get_int(err);
            CHECK_ERR_IN_FORMAT_STRING(err);
            ADVANCE_IN_FORMAT_STRING(format);
        }

        bool has_precision_input = false;
        long long precision_input = 0;

        // evaluate precision field
        if (*format == '.') {
            ADVANCE_IN_FORMAT_STRING(format);
            if (*format == '*') {
                has_precision_input = true;
                precision_input = args->get_int(err);
                CHECK_ERR_IN_FORMAT_STRING(err);
                ADVANCE_IN_FORMAT_STRING(format);
            }
        }

        // evaluate length field
        switch (*format) {
            case 'I' : {
                ADVANCE_IN_FORMAT_STRING(format);
                // Greedily parse for size in bits: 8, 16, 32 or 64
                switch (*format) {
                    case '8':
                        ADVANCE_IN_FORMAT_STRING(format);
                        break;
                    case '1':
                        ADVANCE_IN_FORMAT_STRING(format);
                        if (*format == '6') {
                            format++;
                        }
                        break;
                    case '3':
                        ADVANCE_IN_FORMAT_STRING(format);
                        if (*format == '2') {
                            ADVANCE_IN_FORMAT_STRING(format);
                        }
                        break;
                    case '6':
                        ADVANCE_IN_FORMAT_STRING(format);
                        if (*format == '4') {
                            ADVANCE_IN_FORMAT_STRING(format);
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            case 'l' :
                ADVANCE_IN_FORMAT_STRING(format);
                if (*format == 'l') {
                    ADVANCE_IN_FORMAT_STRING(format);
                }
                break;
            case 'h' :
                ADVANCE_IN_FORMAT_STRING(format);
                if (*format == 'h') {
                    ADVANCE_IN_FORMAT_STRING(format);
                }
                break;
            case 't' :
                ADVANCE_IN_FORMAT_STRING(format);
                break;
            case 'j' :
                ADVANCE_IN_FORMAT_STRING(format);
                break;
            case 'z' :
                ADVANCE_IN_FORMAT_STRING(format);
                break;
            default:
                break;
        }

        // evaluate specifier
        switch (*format) {
            case 'd' :
            case 'i' :
            case 'u' :
            case 'x' :
            case 'X' :
            case 'o' :
            case 'b' : {
                if (*format == 'd' || *format == 'i') {
                    flags |= FLAGS_SIGNED;
                }

                format++;

                if (flags & FLAGS_SIGNED) {
                    // A signed specifier: d, i or possibly I + bit size if enabled
                    memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start);
                    tmp_format_buffer[format - tmp_format_start] = '\0';

                    format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                                  precision_input, args->get_int(err));
                    CHECK_ERR_IN_FORMAT_STRING(err);
                } else {
                    // An unsigned specifier: u, x, X, o, b
                    memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start);
                    tmp_format_buffer[format - tmp_format_start] = '\0';

                    format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                                  precision_input, (unsigned long long) args->get_int(err));
                    CHECK_ERR_IN_FORMAT_STRING(err);
                }
                break;
            }
            case 'f' :
            case 'F' :
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start + 1);
                tmp_format_buffer[format - tmp_format_start + 1] = '\0';

                format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                              precision_input, args->get_double(err));
                CHECK_ERR_IN_FORMAT_STRING(err);

                format++;
                break;
            case 'c' : {
                memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start + 1);
                tmp_format_buffer[format - tmp_format_start + 1] = '\0';

                format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                              precision_input, args->get_int(err));
                CHECK_ERR_IN_FORMAT_STRING(err);

                format++;
                break;
            }

            case 's' : {
                const char *p = args->get_string(err);
                CHECK_ERR_IN_FORMAT_STRING(err);
                if (p == NULL) {
                    output->write_string("(null)", 6);
                } else {
                    memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start + 1);
                    tmp_format_buffer[format - tmp_format_start + 1] = '\0';

                    format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                                  precision_input, p);
                }
                format++;
                break;
            }

            case 'p' : {
                void *value = args->get_pointer(err);
                CHECK_ERR_IN_FORMAT_STRING(err);
                if (value == NULL) {
                    output->write_string("(nil)", 5);
                } else {
                    memcpy(tmp_format_buffer, tmp_format_start, format - tmp_format_start + 1);
                    tmp_format_buffer[format - tmp_format_start + 1] = '\0';

                    format_string(output, tmp_format_buffer, has_width_input, has_precision_input, width_input,
                                  precision_input, value);
                }
                format++;
                break;
            }

            case '%' :
                output->write_string("%", 1);
                format++;
                break;

            default :
                output->write_string(format, 1);
                format++;
                break;
        }
    }

    return 0;
}

// internal vsnprintf - used for implementing _all library functions
static inline int vsnprintf_impl(output_interface *output, const char *format, input_interface *args) {
    format_string_loop(output, format, args);

    int len = (int) output->get_length();

    // termination
    output->write_string("\0", 1);

    // return written chars without terminating \0
    return len;
}

class stdout_output : public output_interface {
public:
    virtual void write_string(const char *s, size_t n) override {
        printf("%.*s", (int) n, s);
        _length += n;
    }

    virtual size_t get_length() override { return _length; }

private:
    size_t _length = 0;
};

static inline int printfv(const char *fmt, input_interface *args) {
    stdout_output output;
    return vsnprintf_impl(&output, fmt, args);
}

class buffer_output : public output_interface {
public:
    buffer_output(char *buffer, size_t size) : _buffer(buffer), _size(size) {}

    virtual void write_string(const char *s, size_t n) override {
        if (_length >= _size) {
            return;
        }

        if (_length + n > _size) {
            n = _size - _length;
        }

        memcpy(_buffer + _length, s, n);
        _length += n;
    }

    virtual size_t get_length() override { return _length; }

private:
    char *_buffer;
    size_t _size;
    size_t _length = 0;
};

static inline int snprintfv(char *s, size_t n, const char *format, input_interface *args) {
    buffer_output output(s, n);
    return vsnprintf_impl(&output, format, args);
}

}
