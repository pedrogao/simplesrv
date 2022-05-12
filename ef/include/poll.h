// Copyright (c) 2018-2020 The EFramework Project
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _POLL_HEADER_
#define _POLL_HEADER_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * poll 是对 select, poll, epoll, kqueue, iouring 的抽象
     */
    typedef struct _ef_event ef_event_t;
    typedef struct _ef_poll ef_poll_t;

    typedef ef_poll_t *(*create_func_t)(int cap);

    // 注意：这里的 fd 其实是 index 序号
    typedef int (*associate_func_t)(ef_poll_t *p, int fd, int events, void *ptr, int fired);
    typedef int (*dissociate_func_t)(ef_poll_t *p, int fd, int fired, int onclose);
    typedef int (*unset_func_t)(ef_poll_t *p, int fd, int events);
    typedef int (*wait_func_t)(ef_poll_t *p, ef_event_t *evts, int count, int millisecs);
    typedef int (*free_func_t)(ef_poll_t *p);

    /*
     * 事件
     */
    struct _ef_event
    {
        int events;
        void *ptr;
    };

    /*
     * 轮训
     */
    struct _ef_poll
    {
        associate_func_t associate;   // 添加事件
        dissociate_func_t dissociate; // 解除关联
        unset_func_t unset;           // 删除
        wait_func_t wait;             // 等待
        free_func_t free;             // 释放
    };

    /*
     *新建
     */
    extern create_func_t ef_create_poll;

/*
 * the macros are equal in poll and epoll
 */
#define EF_POLLIN 0x001  // read
#define EF_POLLOUT 0x004 // write
#define EF_POLLERR 0x008 // err
#define EF_POLLHUP 0x010 // hup

#ifdef __cplusplus
}

#endif

#endif
