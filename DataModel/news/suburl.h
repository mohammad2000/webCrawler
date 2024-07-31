#ifndef _QX_NEWS_SUBURL_H_
#define _QX_NEWS_SUBURL_H_

#include"baseurl.h"

class QX_NEWS_DLL_EXPORT subUrl : public qx::IxPersistable{
    QX_PERSISTABLE_HPP(subUrl)
    public:

    QString         subUrl_id;
    baseUrl_ptr     subUrl_baseUrl;
    QString         subUrl_url;
    QString         subUrl_crawled;
    QString         subUrl_parent;
    QString         subUrl_isReCrawled;
    QString         subUrl_Hash;
    QString         subUrl_isForceSW;
    QString         subUrl_isProssesing;
    QString         subUrl_mayHasPersian;
    subUrl() : qx::IxPersistable(), subUrl_id(QUuid::createUuid().toString()),subUrl_crawled("false"),subUrl_isProssesing("false"),subUrl_mayHasPersian("false"){;}
    virtual ~subUrl(){;}
};
QX_REGISTER_PRIMARY_KEY(subUrl, QString);
QX_REGISTER_HPP_QX_NEWS(subUrl, qx::trait::no_base_class_defined, 0)

typedef std::shared_ptr<subUrl> subUrl_ptr;
typedef std::vector<subUrl_ptr> subUrl_list;
#endif // SUBURL_H
