#include "locale_utils.h"

namespace nvl {

NVL_INLINE std::locale streamDefaultLocale()
{
    return std::locale().classic();
}

}
