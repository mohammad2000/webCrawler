
#include "precompiled.h"
#include "media.h"

#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_NEWS(media)
QX_PERSISTABLE_CPP(media)
namespace qx {
    template <> void register_class(QxClass<media> & t)
    {

        t.id(& media::media_id, "media_id");
        t.data(& media::media_url, "media_url");
        t.data(& media::media_base64, "media_base64");



    }}

