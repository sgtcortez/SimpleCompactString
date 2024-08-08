#include <stdio.h>

#include "scs/scs.h"
#include <stdlib.h>
#include <string.h>

#define MEGABYTE 1024 * 1024

int main ( int argc, char **argv )
{

    char str[] =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce sit amet eleifend odio, "
        "nec luctus justo. Integer accumsan nunc non nisl dictum facilisis. Nunc tincidunt mauris "
        "id enim molestie sodales. Vivamus porttitor pellentesque elit vitae sodales. "
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas."
        "is, gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc"
        "Suspendisse ut sem nisi. Mauris ac justo nec tortor condimentum feugiat non ut urna. "
        "Mauris vestibulum orci ligula, et ultricies ante consectetur quis. Morbi semper placerat "
        "mauris, non dictum libero fringilla at. Pellentesque ultricies sem ac scelerisque "
        "rhoncus. Ut sollicitudin ullamcorper auctor. Donec accumsan non augue ornare vehicula. "
        "Sed viverra nunc ac mauris scelerisque, vel efficitur dolor sodales. Praesent sodales "
        "cursus sapien hendrerit convallis. Donec blandit maximus nulla quis mattis. Donec non "
        "enim iaculis neque viverra lobortis. Fusce at sem enim. Cras fermentum sit amet est nec "
        "porta."
        "Sed nisl sem, pellentesque vitae pharetra nec, aliquet a lectus. Donec lobortis "
        "pellentesque lacus ut sagittis. Morbi urna nulla, mattis sit amet nisi id, euismod "
        "interdum eros. Ut elementum risus sapien, eget molestie eros cursus quis. Fusce quis "
        "feugiat odio, vitae volutpat nulla. Nulla vel porttitor enim. Morbi vehicula lacinia sem "
        "at pretium. Phasellus sed odio ligula. Integer et rutrum mauris. Donec tristique "
        "ullamcorper maximus. Aenean sed turpis in velit porttitor condimentum. Donec mattis "
        "rhoncus pellentesque. Aliquam quis vulputate urna, sodales ultrices quam. Aenean "
        "ultricies pulvinar mauris, a molestie mauris vehicula at. Morbi posuere lectus id justo "
        "fringilla vehicula. Ut ac ex laoreet, fringilla ipsum ac, eleifend nulla."
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas."
        "is, gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc"
        "Suspendisse ut sem nisi. Mauris ac justo nec tortor condimentum feugiat non ut urna. "
        "Mauris vestibulum orci ligula, et ultricies ante consectetur quis. Morbi semper placerat "
        "mauris, non dictum libero fringilla at. Pellentesque ultricies sem ac scelerisque "
        "rhoncus. Ut sollicitudin ullamcorper auctor. Donec accumsan non augue ornare vehicula. "
        "Sed viverra nunc ac mauris scelerisque, vel efficitur dolor sodales. Praesent sodales "
        "cursus sapien hendrerit convallis. Donec blandit maximus nulla quis mattis. Donec non "
        "enim iaculis neque viverra lobortis. Fusce at sem enim. Cras fermentum sit amet est nec "
        "porta."
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas."
        "is, gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas.is, "
        "gravida ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc"
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "Quisque vel massa interdum, pretium urna eu, dignissim turpis. Proin ante mauris, gravida "
        "Quisque vel massa interdum, pretium urna eu"
        "ac cursus in, rhoncus ac nisi. Donec iaculis ante non nunc luctus egestas."
        "luctus egestas.";

    const size_t text_size = strlen ( str );

    char huge[MEGABYTE] = { 0 };

    for ( int index = 0; index < 250; index++ )
    {
        const unsigned int start = index * text_size;

        memcpy ( huge, str, text_size );
        memcpy ( huge + start, str, text_size );
    }

    const scs_t *scs = scs_from ( huge, strlen ( huge ) );
    const int equals = strcmp ( huge, scs );

    fprintf ( stdout, "Are they equals?(%s). String length: %lu\n",
              ( equals == 0 ? "yep" : "nope" ), strlen ( huge ) );

    return equals;
}
