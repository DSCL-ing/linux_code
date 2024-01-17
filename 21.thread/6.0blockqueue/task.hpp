#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <iostream>
#include <string>

class Task
{
public:
    Task()
    {
    }

    Task(int x, int y, char op)
        : _x(x), _y(y), _op(op), _result(0), _exit_code(0)
    {
    }

    ~Task()
    {
    }

    void operator()() // object(); === func();
    {
        switch (_op)
        {
        case '+':
            _result = _x + _y;
            break;
        case '-':
            _result = _x - _y;
            break;
        case '*':
            _result = _x * _y;
            break;
        case '/':
        {
            if (_y != 0)
                _result = _x / _y;
            else
                _exit_code = -1;
            break;
        }
        case '%':
        {
            if (_y != 0)
                _result = _x % _y;
            else
                _exit_code = -2;
            break;
        }
        default:
            break;
        }
    }

    std::string formatArg()
    {
        return std::to_string(_x) + _op + std::to_string(_y) + '=';
    }
    std::string formatRes()
    {
        return std::to_string(_result) + '(' + std::to_string(_exit_code) + ')';
    }

private:
    int _x;
    int _y;
    char _op;

    int _result;
    int _exit_code;
};

#endif