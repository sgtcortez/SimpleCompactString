#include "scs/scs.h"
#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const static struct option long_options[] = {
    { "port", required_argument, 0, 'p' },
    { "verbose", no_argument, 0, 'v' },
    { 0, 0, 0, 0 },
};

const static char *short_options = "vp:";

const static char *HTTP_DELIMITER = "\r\n";

typedef struct
{
    const uint16_t port;
    const bool verbose;

} ServerConfig;

typedef struct
{
    const ServerConfig config;
    int32_t file_descriptor;

} ServerSocket;

ServerConfig parse_server_config ( const int argc, char **argv );
ServerSocket create_socket ( const ServerConfig config );
void accept_connection ( const ServerSocket server );
void parse_request ( const char *buffer, size_t size );

int main ( int argc, char **argv )
{

    ServerConfig config = parse_server_config ( argc, argv );
    fprintf ( stdout, "Server Port: %d, verbose: %s\n", config.port,
              ( config.verbose ? "true" : "false" ) );

    ServerSocket server_socket = create_socket ( config );
    accept_connection ( server_socket );

    close ( server_socket.file_descriptor );

    return 0;
}

ServerConfig parse_server_config ( const int argc, char **argv )
{
    int opt;
    int option_index = 0;
    uint16_t port = 9000;
    bool verbose = false;
    while ( ( opt = getopt_long ( argc, argv, short_options, &long_options, &option_index ) ) !=
            -1 )
    {
        switch ( opt )
        {
        case 'p':
            port = atoi ( optarg );
            break;
        case 'v':
            verbose = true;
            break;
        default:
            exit ( 1 );
        }
    }
    ServerConfig server = { .port = port, .verbose = verbose };
    return server;
}

ServerSocket create_socket ( const ServerConfig config )
{
    int socket_fd = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( socket_fd < 1 )
    {
        perror ( "socket" );
        exit ( 1 );
    }

    const struct sockaddr_in server = { .sin_family = AF_INET,
                                        .sin_port = htons ( config.port ),
                                        .sin_addr.s_addr = inet_addr ( "127.0.0.1" ) };

    int one = 1;

    if ( setsockopt ( socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof ( one ) ) < 0 )
    {
        perror ( "setsocketopt reuseaddress" );
        exit ( 1 );
    }
    if ( setsockopt ( socket_fd, SOL_SOCKET, SO_REUSEPORT, &one, sizeof ( one ) ) < 0 )
    {
        perror ( "setsocketopt reuseport" );
        exit ( 1 );
    }

    if ( bind ( socket_fd, (const struct sockaddr *)&server, sizeof ( server ) ) < 0 )
    {
        perror ( "bind" );
        close ( socket_fd );
        exit ( 1 );
    }

    if ( listen ( socket_fd, 10 ) < 0 )
    {
        perror ( "listen" );
        close ( socket_fd );
        exit ( 1 );
    }

    ServerSocket socket = { .file_descriptor = socket_fd, .config = config };
    return socket;
}

void accept_connection ( const ServerSocket server )
{
    printf (
        "Open a terminal and try: \"curl -X POST 'http://localhost:%d/path/my/application' "
        "--header 'Accept: text/plain' --data-raw 'Hello World'\"\n",
        server.config.port );

    struct sockaddr client;
    socklen_t size = sizeof ( struct sockaddr );
    int client_fd = accept ( server.file_descriptor, &client, &size );
    if ( client_fd < 0 )
    {
        perror ( "accept" );
        exit ( 1 );
    }
    char buffer[4096];
    int bytes = read ( client_fd, buffer, sizeof ( buffer ) );
    parse_request ( buffer, bytes );

    char *response = "HTTP/1.1 200 OK\r\n\r\nYour connection was accepted!";

    write ( client_fd, response, strlen ( response ) );
}

void parse_request ( const char *buffer, size_t size )
{
    scs_t request_packet = scs_from ( buffer, size );

    scs_iterator_t http_iterator =
        scs_split ( request_packet, HTTP_DELIMITER, strlen ( HTTP_DELIMITER ) );

    scs_t http_information_line = scs_iterator_next ( http_iterator );

    printf ( "Http Request Line: %s\n", http_information_line );

    bool has_headers = false;
    do
    {

        const scs_t header = scs_iterator_next ( http_iterator );
        has_headers = strcmp ( header, "" ) != 0;

        if ( has_headers )
        {
            scs_iterator_t header_line_iterator = scs_split ( header, ":", 1 );
            scs_t header_name = scs_iterator_next ( header_line_iterator );
            scs_t header_value = scs_iterator_next ( header_line_iterator );

            printf ( "Header name: %s, header value: %s\n", header_name, header_value );

            scs_free ( header_name );
            scs_free ( header_value );
            scs_iterator_free ( header_line_iterator );
        }

        scs_free ( header );
    } while ( has_headers );

    const scs_t body = scs_iterator_next ( http_iterator );
    if ( body != NULL && scs_size ( body ) > 0 )
    {
        // has body
        puts ( body );
        scs_free ( body );
    }
    scs_free ( http_information_line );
    scs_iterator_free ( http_iterator );
    scs_free ( request_packet );
}
