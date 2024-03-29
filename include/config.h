#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define CONFIG_REQUEST_MAX 200 // max len of request
#define CONFIG_SERVER_LOCATION "3000"
#define CONFIG_THREAD_COUNT 5

#define CONFIG_UNI_START 1072 // codepoint of small russian 'a'.

#define CONFIG_MAX_TIME      5    // max bruteforce time
#define CONFIG_MAX_RESPONCE  2000 // max number of sent results (or browser will die)

#define CONFIG_DICT_PATH "dict.utf8"

#define CONFIG_ALPH_SIZE 32 // size of alphabet

#define CONFIG_DEFAULT_WORDS_MIN 1
#define CONFIG_DEFAULT_WORDS_MAX 10
#define CONFIG_LIMIT_WORDS_MIN 1
#define CONFIG_LIMIT_WORDS_MAX 10

#define CONFIG_DEFAULT_SYMB_MIN 1
#define CONFIG_DEFAULT_SYMB_MAX 99
#define CONFIG_LIMIT_SYMB_MIN 1
#define CONFIG_LIMIT_SYMB_MAX 99

#define CONFIG_REMAP {{1105, 1077}, /*'e:'  -> 'e' */ \
                      {44, 32},     /* ','  -> ''  */ \
                      {45, 32},     /* '-'  -> ''  */ \
                      {46, 32},     /* '.'  -> ''  */ \
                      {59, 32},     /* ';'  -> ''  */ \
                      {58, 32},     /* ':'  -> ''  */ \
                      {34, 32},     /* '\"' -> ''  */ \
                      {39, 32},     /* '\'' -> ''  */ \
                      { 9, 32},     /* '\"' -> ''  */ }

#endif
