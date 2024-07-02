
#include "precompiled.h"
#include "baseurl.h"

#include <QxOrm_Impl.h>
QX_REGISTER_CPP_QX_NEWS(baseUrl)
QX_PERSISTABLE_CPP(baseUrl)

namespace qx {
    template <> void register_class(QxClass<baseUrl> & t)
    {
        t.id(& baseUrl::baseUrl_id,"baseUrl_id");
        t.data(& baseUrl::baseUrl_url,"baseUrl_url");
        t.data(& baseUrl::baseUrl_name,"baseUrl_name");
        t.data(& baseUrl::baseUrl_isActive,"baseUrl_isActive");
        t.data(& baseUrl::baseUrl_agency,"baseUrl_agency");
        t.data(& baseUrl::baseUrl_tags,"baseUrl_tags");
        t.data(& baseUrl::baseUrl_hash,"baseUrl_hash");
        t.data(& baseUrl::baseUrl_timeInterval,"baseUrl_timeInterval");


    }
}
