#include <stdint.h>
// Структура index_record для хранения временной метки и номера записи
typedef struct {
    double time_mark;
    uint64_t recno;
} index_record;
// Структура index_hdr_s для хранения количества записей и массива index_record
typedef struct {
    uint64_t records;
    index_record * idx;
} index_hdr_s;
