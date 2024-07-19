
#include "precompiled.h"
#include "tags.h"

#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_NEWS(tags)
QX_PERSISTABLE_CPP(tags)
namespace qx {
    template <> void register_class(QxClass<tags> & t)
    {

        t.id(& tags::tags_id, "tags_id");
        t.data(& tags::tags_tagName, "tags_tagName");
        t.data(& tags::tags_selector, "tags_selector");



    }}

