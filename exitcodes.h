#ifndef EXIT_CODES_H_
#define EXIT_CODES_H_

typedef enum{
    SUCCESS,
    UNKNOWN_ERROR,
    SEGMENTATION_FAULT,
    NOT_YET_IMPLEMENTED,
    FAILED_TO_READ_FILE,
    OPCODE_NOT_EXIST
} ERROR_CODES;

#endif