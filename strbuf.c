#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"strbuf.h"

// Part 2A
// TASK
// 实现字符串缓冲区类 strbuf 简单的初始化，填充，释放，交换，比较，清空等操作。

// 初始化 sb 结构体，容量为 alloc
void strbuf_init(struct strbuf *sb, size_t alloc){

    sb->len=0;
    sb->alloc=0;
    sb->buf=NULL;

    if(alloc <= 0){
       printf("Hey,your alloc should be a positive number!\n");
        return;
    }

    sb->alloc = alloc;
    sb->buf = (char*)malloc(alloc+1);

    if(sb->buf != NULL){
        sb->buf[0] = '\0';
    }else{
        sb->alloc=0;
    }

}


// 将字符串填充到 sb 中，长度为 len，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc){
   
    if(sb->alloc > 0 && sb->buf!=NULL){
        free(sb->buf);
    }
    
    strbuf_init(sb,alloc);

    sb->buf=(char*)str;
    sb->alloc=alloc;
    sb->buf[len]='\0';
    sb->len = len;

}


// 释放 sb 结构体的内存
void strbuf_release(struct strbuf *sb){

    free(sb->buf);

    sb->alloc=0;
    sb->len=0;
    sb->buf=NULL;

}


// 交换两个 strbuf
void strbuf_swap(struct strbuf *a, struct strbuf *b){

    if (a==NULL || b==NULL ){
        printf("Oh no.\n");
        return;
    }

    struct strbuf temp = *a;
    *a=*b;
    *b=temp;
}


// 将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小
char *strbuf_detach(struct strbuf *sb, size_t *sz){

    *sz = sb->alloc;
    char* Raw_memory = sb->buf; 
    return Raw_memory;

}


// 比较两个 strbuf 的内存是否相同
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second){
   
    if(first->alloc!=second->alloc){
        return 0;
    }else{
        return 1;
    }

}
//I don't think I'm wrong.


// 清空 sb
void strbuf_reset(struct strbuf *sb){
    
    sb->len=0;
    sb->buf[0]='\0';

}









//Part 2B
//实现字符串缓冲区类 strbuf 扩容，(追加 | 插入)字符，字符串等操作。

// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用
void strbuf_grow(struct strbuf* sb, size_t extra) {

    if (sb == NULL) {
       printf("?");
        return;
    }

    size_t need = sb->len + extra;

    if (sb->alloc >= need) {
      printf("Yes,bro,the space is enough.\n");
      return;
    } else printf("No,the space isnt enough.\n",
    "No problem,i will lend a hand. :)\n" );

    char* new_memory=(char*)realloc(sb->buf, need);
    
    sb->buf=new_memory;
    sb->alloc = need;

}

// 向 sb 追加长度为 len 的数据 data
void strbuf_add(struct strbuf* sb, const void* data, size_t len) {

    // if (sb == NULL || data == NULL) {
    //     printf("?");
    //     return;
    // }

    strbuf_grow(sb, len+1);

    printf("Ok,bro.\n");
    memcpy(sb->buf + sb->len, data, len);
    printf("here\n");
    sb->len += len;
    sb->buf[sb->len]='\0';

}

// 向 sb 追加一个字符 c
void strbuf_addch(struct strbuf* sb, int c) {
    
    if (sb == NULL) {
        printf("?");
        return;
    }

    strbuf_grow(sb, 2);
    
    printf("sad\n");
    sb->buf[sb->len] = (char)c;
    sb->len += 1;
    sb->buf[sb->len] = '\0';

}


// 向 sb 追加一个字符串 s
void strbuf_addstr(struct strbuf* sb, const char* s) {

    if (sb == NULL || s == NULL) {
        printf("?");
        return;
    }

    size_t len = strlen(s);

    strbuf_grow(sb, len+1);

    strcat(sb->buf + sb->len, s);
    sb->len += len;

}

// 向一个 sb 追加另一个 strbuf 的数据
void strbuf_addbuf(struct strbuf* sb, const struct strbuf* sb2) {

    if (sb == NULL || sb2 == NULL) {
        printf("?");
        return;
    }

    strbuf_grow(sb, sb2->len+1  );

    strbuf_add(sb, sb2->buf, sb2->len);

}

// 设置 sb 的长度 len
void strbuf_setlen(struct strbuf* sb, size_t len) {
    
    if (sb == NULL) {
        printf("?");
        return;
    }

    // if (len > sb->len) {
    //     printf("No,bro,you cant do this ");
    //     return;
    // }

    sb->len = len;
    sb->buf[len]='\0';


}

// 计算 sb 目前仍可以向后追加的字符串长度
size_t strbuf_avail(const struct strbuf* sb) {

    if (sb == NULL) {
        printf("?");
        return 0;
    }

    size_t leisure = sb->alloc - sb->len - 1;
    return leisure;

}

// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data
void strbuf_insert(struct strbuf* sb, size_t pos, const void* data, size_t len) {

    if (sb == NULL || data == NULL) {
        printf("?");
        return;
    }

    strbuf_grow(sb, len+1);

    memmove(sb->buf + len + pos, sb->buf+pos, sb->len-pos+1);
    printf("what");
    memcpy(sb->buf + pos, data, len);
    sb->len += len;

}










// Part 2C
// 实现字符串缓冲区类 strbuf 删除部分内容等功能。

// 去除 sb 缓冲区左端的所有空格、制表符和'\t'字符
void strbuf_ltrim(struct strbuf *sb){

    if (sb == NULL) {
        printf("?");
        return;
    }

    int saki=0;
    for(int i=0;i<sb->len;i++){
        if(sb->buf[i] != ' ' && sb->buf[i] !='\t'){
        saki=i;
        break;
        }
    }

    if(saki==0){
        return;
    }

    memmove(sb->buf,sb->buf+saki,sb->len-saki+1);
    printf("\nsoyorin?\n");

    sb->len-=saki;
    sb->buf[sb->len]='\0';

}
 
// 去除 sb 缓冲区右端的所有空格、制表符和'\t'字符
void strbuf_rtrim(struct strbuf *sb){

    if (sb == NULL) {
        printf("?");
        return;
    }

    int soyorin=0;
    for(int i=sb->len-1;i>0;i--){
        if(sb->buf[i] != ' ' && sb->buf[i] != '\t'){
            soyorin=i;
            break;
        }
    }

    sb->buf[soyorin+1]='\0';
    sb->len = soyorin+1;

}
 
// 删除 sb 缓冲区从 pos 坐标开始长度为 len 的内容
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){

    if (sb == NULL) {
        printf("?");
        return;
    }

    if(pos+len>sb->len){
        sb->buf[pos]='\0';
        sb->len=pos;
        return;
    }

    memmove(sb->buf[pos],sb->buf[pos+len],sb->len-len+1);
    sb->buf[sb->len-len]='\0';
    sb->len-=len;

}






// Part 2D

//在我们使用 C FILE API 读取一个小文件的时候经常会有这样的一个困惑，
//我们为什么不能直接一次性读完整个文件到一个大缓冲区呢? 而 fread or read 总是
//用一个将一个文件中的内容反复读到一个缓冲区中，然后我们从这个缓冲区中取出内容, 但是
//为什么我们不能直接将一个文件读到一个缓冲区中呢?

//在我们想要从文件或者终端读取一行数据的时候经常有这样的疑惑，我应该用什么函数去读？
//C++ 的 cin.getline()? C++ 的 getline()? C 的 getline()? Python 的 readl
//ine()? 正如 Stack Overflow 的网友的总结Stack Overflow 

//c 版本的 getline() 效率是最高的。那么问题来了, C 的 getline() 每次都得我去指定
//缓冲区和长度… 有什么好的方法让用户可以直接调用一个 strbuf_getline() 无脑的从缓冲区
//中拿到想要的内容呢？

// 将文件描述符为 fd 的所有文件内容追加到 sb 中，sb 增长 hint ? hint : 8192 大小
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint){
    return 0;
}
 
// 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb
int strbuf_getline(struct strbuf *sb, FILE *fp){
    return 0;
}














struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max){
    return NULL;
}
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen){
    return 0;
}
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len){
    return NULL;
}
