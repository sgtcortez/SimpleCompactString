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
    scs_t string = scs_from_string ( "Hello" );
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

TEST ( restore_as_c_string, mustBeSuccess )
{
    char *input = "Hello, my name its Matheus!";
    const uint64_t input_size = strlen ( input );

    const scs_t string = scs_from_string ( input );
    const char *restore = scs_to_string ( string );

    ASSERT_TRUE ( strncmp ( input, restore, input_size ) == 0 );

    scs_free ( string );
}