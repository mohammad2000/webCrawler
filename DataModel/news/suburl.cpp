
#include "precompiled.h"
#include "suburl.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_NEWS(subUrl)
QX_PERSISTABLE_CPP(subUrl)
namespace qx {
    template <> void register_class(QxClass<subUrl> & t)
    {

        t.data(& subUrl::subUrl_id, "subUrl_id");
        t.data(& subUrl::subUrl_baseUrl, "subUrl_baseUrl");
        t.data(& subUrl::subUrl_parent, "subUrl_parent");
        t.id(& subUrl::subUrl_url, "subUrl_url");
        t.data(& subUrl::subUrl_crawled, "subUrl_crawled");
        t.data(& subUrl::subUrl_isReCrawled, "subUrl_isReCrawled");
        t.data(& subUrl::subUrl_Hash, "subUrl_Hash");
        t.data(& subUrl::subUrl_isForceSW,"subUrl_isForceSW");
        t.data(& subUrl::subUrl_isProssesing,"subUrl_isProssesing");
        t.data(& subUrl::subUrl_isProssesing,"subUrl_mayHasPersian");



    }}

