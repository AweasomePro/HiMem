/**
 * Copyright 2004-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#define ANDROID_VERSION_NUM 900

struct string_t {
    const char *data;
    size_t length;
};

typedef bool (*unwind_callback_t)(uintptr_t, void *);

#if ANDROID_VERSION_NUM >= 700

#if defined(__aarch64__)
#define __get_tls()                             \
  ({                                            \
    void** __val;                               \
    __asm__("mrs %0, tpidr_el0" : "=r"(__val)); \
    __val;                                      \
  })
#elif defined(__arm__)
#define __get_tls()                                      \
  ({                                                     \
    void** __val;                                        \
    __asm__("mrc p15, 0, %0, c13, c0, 3" : "=r"(__val)); \
    __val;                                               \
  })
#elif defined(__mips__)
#define __get_tls()                                                      \
  /* On mips32r1, this goes via a kernel illegal instruction trap that's \
   * optimized for v1. */                                                \
  ({                                                                     \
    register void** __val asm("v1");                                     \
    __asm__(                                                             \
        ".set    push\n"                                                 \
        ".set    mips32r2\n"                                             \
        "rdhwr   %0,$29\n"                                               \
        ".set    pop\n"                                                  \
        : "=r"(__val));                                                  \
    __val;                                                               \
  })
#elif defined(__i386__)
#define __get_tls()                           \
  ({                                          \
    void** __val;                             \
    __asm__("movl %%gs:0, %0" : "=r"(__val)); \
    __val;                                    \
  })
#elif defined(__x86_64__)
#define __get_tls()                          \
  ({                                         \
    void** __val;                            \
    __asm__("mov %%fs:0, %0" : "=r"(__val)); \
    __val;                                   \
  })
#else
#error unsupported architecture
#endif

#endif

void *getThreadInstance();

uintptr_t get_art_thread();

__attribute__((always_inline)) inline uint32_t CountShortyRefs(
        string_t shorty) {
    uint32_t result = 0;
    for (size_t i = 0; i < shorty.length; i++) {
        if (shorty.data[i] == 'L') {
            result++;
        }
    }
    return result;
}

__attribute__((always_inline)) inline string_t String(string_t data) {
    return data;
}

__attribute__((always_inline)) inline string_t
String(uintptr_t ptr, const char *, const char *, uint32_t length) {
    return string_t{.data = (const char *) ptr, .length = length};
}

__attribute__((always_inline)) inline uint64_t GetMethodTraceId(
        uint32_t dex_id,
        uint32_t method_id) {
    return (static_cast<uint64_t>(method_id) << 32) | dex_id;
}

__attribute__((always_inline)) inline uint8_t Read1(uintptr_t addr) {
    return *reinterpret_cast<uint8_t *>(addr);
}

__attribute__((always_inline)) inline uint16_t Read2(uintptr_t addr) {
    return *reinterpret_cast<uint16_t *>(addr);
}

__attribute__((always_inline)) inline uint32_t Read4(uintptr_t addr) {
    return *reinterpret_cast<uint32_t *>(addr);
}

__attribute__((always_inline)) inline uint64_t Read8(uintptr_t addr) {
    return *reinterpret_cast<uint64_t *>(addr);
}

__attribute__((always_inline)) inline uintptr_t AccessField(
        uintptr_t addr,
        uint32_t offset) {
    return addr + offset;
}

__attribute__((always_inline)) inline uintptr_t
AccessArrayItem(uintptr_t addr, uint32_t item_size, uint32_t item) {
    return addr + (item_size * item);
}

__attribute__((always_inline)) inline uintptr_t AdvancePointer(
        uintptr_t ptr,
        uint32_t offset) {
    return ptr + offset;
}

