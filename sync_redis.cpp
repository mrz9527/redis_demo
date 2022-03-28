/*
    hiredis同步使用
    // 连接redis
    redisContext* context   = redisConnet(ip, port);
    // 发送命令，返回buf
    redisReply* reply       = redisCommand(context, cmd);
    // 释放buf
    freeReplyObject(reply);
    // 断开连接
    redisFree(context);
*/

#include <cstdlib>
#include <cstring>
#include "stdio.h"

#include "hiredis.h"

void RedisContextDeleter(redisContext *con)
{
    redisFree(con);
}

void RedisReplyDeleter(redisReply *reply)
{
    freeReplyObject(reply);
}

redisContext *context;
redisReply *reply;

bool RedisConnect(char *ip, int port)
{
    context = redisConnect(ip, port);
    if (!context) {
        printf("redisConnect(%s, %d) failed. context = nullptr\n", ip, port);
        return false;
    }
    if (context->err) { // context->err:0表示成功，非0，表示连接失败)
        printf("redisConnect(%s, %d) failed. errstr = %s\n", ip, port, context->errstr);
        return false;
    }

    printf("连接Redis %s:%d 成功\n", ip, port);
    return true;
}

//int main(int argc, char **argv)
//{
//    // 参数合法性
//    if (argc != 3) {
//        printf("Usage:./myredis ip port\n");
//        return -1;
//    }
//
//    char *ip = argv[1];
//    int port = atoi(argv[2]);
//
//    // 连接redis数据库
//    if (!RedisConnect(ip, port)) {
//        return -1;
//    }
//
//    // 发送登录密码
//    char *passwd = "xinmei365";
//    reply = (redisReply *) redisCommand(context, "AUTH %s", passwd);
//    freeReplyObject(reply);
//
//    // gettimeofday
//    struct  timeval  s_timeval, e_timeval;
//    gettimeofday(&s_timeval,nullptr);
//
//    // redis业务，测试10w次写数据
//    int count = 100000;
//    char cmdText[50] = {0};
//    for (int i = 0; i < count; ++i) {
//        memset(cmdText, 0, sizeof(cmdText));
//        sprintf(cmdText, "set c++k%d %d", i, i + 5); // 值偏移5
//        printf("-> %s\n", cmdText);
//        reply = (redisReply *) redisCommand(context, cmdText);
//        if (!reply) {
//            printf("%s\n", context->errstr);    // 一旦发生错误，这个 ontext 就不能继续使用，必须建立新的连接。
//            break;
//        }
//
//        if (reply->type == REDIS_REPLY_STATUS) { // redis返回的是状态，比如set命令，一般返回状态
//            printf("[REDIS_REPLY_STATUS]: %s\n", reply->str);
//        } else if (reply->type == REDIS_REPLY_ERROR) {
//            printf("[REDIS_REPLY_ERROR]: %s\n", reply->str);
//        } else if (reply->type == REDIS_REPLY_INTEGER) {
//            printf("[REDIS_REPLY_INTEGER]: %lld\n", reply->integer);
//        } else if (reply->type == REDIS_REPLY_NIL) {
//            printf("[REDIS_REPLY_NIL]\n");
//        } else if (reply->type == REDIS_REPLY_STRING) {
//            printf("[REDIS_REPLY_STRING]: %s\n", reply->str);
//        } else if (reply->type == REDIS_REPLY_DOUBLE) {
//            printf("[REDIS_REPLY_DOUBLE]: %f %s\n", reply->dval, reply->str);
//        } else if (reply->type == REDIS_REPLY_BOOL) {
//            printf("[REDIS_REPLY_BOOL]: %lld\n", reply->integer);
//        } else if (reply->type == REDIS_REPLY_ARRAY) {
//            printf("[REDIS_REPLY_ARRAY]: number of elements: %lu\n", reply->elements);
//            for (int j = 0; j < reply->elements; ++j) {
//                printf("[%d] %s\n", j, reply->element[j]->str);
//            }
//        } else if (reply->type == REDIS_REPLY_MAP) {
//            printf("[REDIS_REPLY_MAP]: number of elements: %lu\n", reply->elements);
//            for (int j = 0; j < reply->elements; ++j) {
//                printf("[%d] %s\n", j, reply->element[j]->str);
//            }
//        } else if (reply->type == REDIS_REPLY_SET) {
//            printf("[REDIS_REPLY_SET]: number of elements: %lu\n", reply->elements);
//            for (int j = 0; j < reply->elements; ++j) {
//                printf("[%d] %s\n", j, reply->element[j]->str);
//            }
//        } else if (reply->type == REDIS_REPLY_BIGNUM) {
//            printf("[REDIS_REPLY_BIGNUM]: %s\n", reply->str);
//        } else {
//            printf("[other]\n");
//        }
//
//        freeReplyObject(reply);
//    }
//
//    gettimeofday(&e_timeval,nullptr);
//    double costtime = (double)(e_timeval.tv_sec-s_timeval.tv_sec) + (e_timeval.tv_usec-s_timeval.tv_usec)/1000000;
//    printf("timeval : %f s\n", costtime);
//
//    // 断开连接
//    redisFree(context);
//}


// 写10w条数据
void redis_write_10w()
{
    int count = 100000;
    char cmd[50] = {0};
    for (int i = 0; i < count; ++i) {
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, "set c++k%d %d", i, i + 5);
        reply = (redisReply *) redisCommand(context, cmd);
        freeReplyObject(reply);
    }
}

void redis_flushdb()
{
    reply = (redisReply *) redisCommand(context, "flushdb");
    freeReplyObject(reply);
}

int main(int argc, char **argv)
{
    char *ip = argv[1];
    int port = atoi(argv[2]);
    // redis 连接
    context = redisConnect(ip, port);

    // gettimeofday
    struct timeval s_timeval, e_timeval;
    gettimeofday(&s_timeval, nullptr);
    redis_write_10w();
    gettimeofday(&e_timeval, nullptr);
    double costtime =
            (double) (e_timeval.tv_sec - s_timeval.tv_sec) + (e_timeval.tv_usec - s_timeval.tv_usec) / 1000000;
    printf("timeval : %f s\n", costtime);

    // 断开连接
    redisFree(context);
}