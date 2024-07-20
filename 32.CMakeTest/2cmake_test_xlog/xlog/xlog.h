//xlog.h

#ifndef XLOG_H
#define XLOG_H



#ifdef _WIN32 //WIN32环境
    #ifdef XLOG_EXPORTS //需要导出(动态库)
        #define XCPP_API _declspec(dllexport)
    #else  //否则导入
        #define XCPP_API _declspec(dllimport)
    #endif
#else  //linux mac环境
    #define XCPP_API //非WIN32环境则为空
#endif


class XCPP_API Xlog{
    public:
       Xlog();
};
#endif
