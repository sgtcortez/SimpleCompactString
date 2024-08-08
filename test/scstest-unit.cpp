#include "scs/scs.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>

int main ( int argc, char **argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS ();
}

TEST ( create_from_string, mustBeSuccess )
{
    scs_t string = scs_from ( "Hello", 5 );
    ASSERT_NE ( string, nullptr );
    scs_free ( string );
}

TEST ( create_from_array, mustBeSuccess )
{
    char array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 55, 19 };
    const uint64_t size = sizeof ( array ) / sizeof ( array[0] );
    scs_t ar = scs_from ( array, size );
    ASSERT_EQ ( scs_size ( ar ), size );
}

TEST ( compare_cstrings, mustBeSuccess )
{
    char *input = "Hello, my name its Matheus!";
    const uint64_t input_size = strlen ( input );

    const scs_t string = scs_from ( input, strlen(input) );

    ASSERT_TRUE ( strncmp ( input, string, input_size ) == 0 );

    scs_free ( string );
}

TEST ( compare_length, mustBeSuccess )
{
    // For binary strings, we can not depend of the null byte, thats why we need the `scs_size` function
    scs_t string = scs_from ( "Hello", 5 );
    ASSERT_EQ ( 5, scs_size ( string ) );

    scs_free ( string );

    char other[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    scs_t string2 = scs_from ( other, 10 );
    ASSERT_EQ ( 10, scs_size ( string2 ) );

    scs_free ( string2 );
}
