#include "scs/scs.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>
#include <stdint.h>

int main ( int argc, char **argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    return RUN_ALL_TESTS ();
}

TEST ( split_string, mustBeSuccess )
{
    const char *str = "Hello my name is Matheus!";

    const scs_t string = scs_from ( str, strlen ( str ) );

    scs_iterator_t iterator = scs_split ( string, " ", 1 );
    scs_t value = nullptr;

    value = scs_iterator_next ( iterator );

    ASSERT_STREQ ( "Hello", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "my", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "name", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "is", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Matheus!", value );
    scs_free ( value );

    // Reached the end, must return empty
    value = scs_iterator_next ( iterator );
    ASSERT_EQ ( nullptr, value );

    // Check if subsequent calls still return null
    value = scs_iterator_next ( iterator );
    ASSERT_EQ ( nullptr, value );

    scs_free ( string );
    scs_iterator_free ( iterator );
}

TEST ( split_http_packet, mustBeSuccess )
{
    const char *str =
        "POST /path/to/my/application?queryparam1&queryparam2 HTTP/1.1\r\nHost: "
        "localhost:9000\r\nAccept: text/plain; application/json; application/xml\r\nClient-Agent: "
        "myapp123\r\nContent-Length:5\r\n\r\nHello";

    const scs_t string = scs_from ( str, strlen ( str ) );

    scs_iterator_t iterator = scs_split ( string, "\r\n", 2 );
    scs_t value = nullptr;

    value = scs_iterator_next ( iterator );

    ASSERT_STREQ ( "POST /path/to/my/application?queryparam1&queryparam2 HTTP/1.1", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Host: localhost:9000", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Accept: text/plain; application/json; application/xml", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Client-Agent: myapp123", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Content-Length:5", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Hello", value );
    scs_free ( value );

    // Reached the end, must return empty
    value = scs_iterator_next ( iterator );
    ASSERT_EQ ( nullptr, value );

    scs_free ( string );
    scs_iterator_free ( iterator );
}

TEST ( split_not_found_delimiter, mustReturnWholeString )
{

    const char *str = "Hello, my name is sgtcortez";
    const scs_t string = scs_from ( str, strlen ( str ) );

    scs_iterator_t iterator = scs_split ( string, "Matheus", strlen ( "Matheus" ) );
    scs_t value = scs_iterator_next ( iterator );
    ASSERT_STREQ ( "Hello, my name is sgtcortez", value );
    scs_free ( value );

    value = scs_iterator_next ( iterator );
    ASSERT_EQ ( nullptr, value );

    scs_free ( string );
    scs_iterator_free ( iterator );
}