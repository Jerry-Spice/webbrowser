#include <string>
#ifndef key_value_h
#define key_value_h

/**
 * Since everything in an HTTP Header is just this form:
 * KEY: VALUE
 * I've abstracted it to just be a key_value pair struct.
 * They're also always strings and if not they can be reinterpreted by the class that uses that header.
 */
typedef struct key_value {
    std::string key;
    std::string value;
} key_value;

#endif
