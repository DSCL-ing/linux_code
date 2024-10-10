#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <chrono>
#include <openssl/ssl.h>
#include <openssl/err.h>



#define TARGET_HOST "www.fhxww.cn"
#define TARGET_PORT 80
#define SOURCE_IP "192.168.2.5" //云服务器,使用INADDR_ANY


// 初始化SSL库
void init_openssl() {
    //载入所有错误信息
    SSL_load_error_strings();
    //载入所有ssl算法
    OpenSSL_add_ssl_algorithms();
}

// 清理SSL库
void cleanup_openssl() {
    EVP_cleanup();
}

// 创建SSL上下文
SSL_CTX* create_ssl_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_client_method();  // 可以处理多个SSL/TLS协议版本
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[4096];
    std::string request = "GET / HTTP/1.1\r\nHost: " TARGET_HOST "\r\nConnection: close\r\n\r\n";


    // 初始化SSL库
    init_openssl();
    SSL_CTX *ctx = create_ssl_context();


    // DNS 解析
    std::chrono::high_resolution_clock::time_point begin1  = std::chrono::high_resolution_clock::now();
    server = gethostbyname(TARGET_HOST);
    auto end1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> cost1 = end1-begin1;
    std::cout<<"\n"<<">>>>>>>>>>>>>>> DNS解析时间: "<<cost1.count()<<std::endl<<std::endl;

    if (server == nullptr) {
      std::cerr << "Error: DNS resolution failed.\n";
      SSL_CTX_free(ctx);
      cleanup_openssl();
      return 1;
    }

    // 创建 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      std::cerr << "Error: Failed to open socket.\n";
      SSL_CTX_free(ctx);
      cleanup_openssl();
      return 1;
    }

    // 设置源 IP
    struct sockaddr_in source_addr;
    memset(&source_addr, 0, sizeof(source_addr));
    source_addr.sin_family = AF_INET;
    source_addr.sin_addr.s_addr = inet_addr(SOURCE_IP);
    source_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&source_addr, sizeof(source_addr)) < 0) {
      std::cerr << "Error: Failed to bind source IP.\n";
      SSL_CTX_free(ctx);
      cleanup_openssl();
      close(sockfd);
      return 1;
    }

    // 设置目标服务器信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TARGET_PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // 创建SSL对象并绑定到套接字
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    // 连接服务器
    std::chrono::high_resolution_clock::time_point begin2  = std::chrono::high_resolution_clock::now();

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      std::cerr << "Error: Failed to connect to the server.\n";
      close(sockfd);
      SSL_CTX_free(ctx);
      cleanup_openssl();
      return 1;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cost2 = end2-begin2;
    std::cout<<"\n"<<">>>>>>>>>>>>>>> 三次握手时间: "<<cost2.count()<<std::endl<<std::endl;


    // 发送 HTTP 请求
    if (send(sockfd, request.c_str(), request.length(), 0) < 0) {
      std::cerr << "Error: Failed to send HTTP request.\n";
      close(sockfd);
      return 1;
    }


    // 接收服务器响应


    std::chrono::high_resolution_clock::time_point begin3  = std::chrono::high_resolution_clock::now();
    int received_bytes;
    while ((received_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
      buffer[received_bytes] = '\0';  // 确保缓冲区是以 null 结尾的字符串
      std::cout << buffer;
    }

    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cost3 = end3-begin3;
    std::cout<<"\n"<<">>>>>>>>>>>>>>> get请求到相应所需时间: "<<cost3.count()<<std::endl<<std::endl;

    
    // 关闭 socket
    close(sockfd);

    // 关闭SSL和套接字
    SSL_free(ssl);
    close(sockfd);

    // 释放SSL上下文
    SSL_CTX_free(ctx);
    cleanup_openssl();


    std::cout<<"\n"<<">>>>>>>>>>>>>>> 整个交互所需时间: "<<cost3.count()+cost1.count()+cost2.count()<<std::endl<<std::endl;

    return 0;
}

