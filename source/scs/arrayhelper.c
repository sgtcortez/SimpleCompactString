#include "scs/arrayhelper.h"

void array_store ( char *array, uint64_t number, uint8_t bytes )
{
    for ( uint8_t index = 0; index < bytes; index++ )
    {
        uint8_t byte = number & 0xFF;
        array[index] = byte;
        number = number >> 8;
    }
}

uint64_t array_restore ( const char *array, uint8_t bytes )
{
    uint64_t result = 0;
    for ( uint8_t index = 0; index < bytes; index++ )
    {
        // Since we receive a signed char, we need to convert it to unsigned,
        // before performing operations
        const uint8_t temp = array[index];

        // Store the temp value inside a unsigned 64 bits variable, so we can
        // avoid overflows
        const uint64_t byte = temp;
        uint64_t value = byte << ( 8 * index );
        result = result | value;
    }
    return result;
}

uint8_t count_bytes ( uint64_t number )
{
    uint8_t total = 0;
    do
    {
        total++;
        number >>= 8;
    } while ( number > 0 );
    return total;
}