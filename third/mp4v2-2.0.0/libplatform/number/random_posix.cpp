#include "libplatform/impl.h"
#include <stdlib.h>

namespace mp4v2 { namespace platform { namespace number {

///////////////////////////////////////////////////////////////////////////////

uint32_t
random32()
{
    //return uint32_t( ::random() );
    return uint32_t( ::rand() );
}

///////////////////////////////////////////////////////////////////////////////

void
srandom( uint32_t seed )
{
    //::srandom( seed );
    ::srand( seed );
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::time
