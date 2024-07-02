#ifndef _QX_NEWS_BASEURL_H_
#define _QX_NEWS_BASEURL_H_

#include "tags.h"
class QX_NEWS_DLL_EXPORT baseUrl : public qx::IxPersistable
{
    QX_PERSISTABLE_HPP(baseUrl)
    QX_REGISTER_FRIEND_CLASS(baseUrl)
    public:

    QString     baseUrl_id;
    QString     baseUrl_name;
    QString     baseUrl_url;
    QString     baseUrl_isActive;
    tags_list   baseUrl_tags;
    QString     baseUrl_agency;
    QString     baseUrl_hash;
    QString     baseUrl_timeInterval;

    baseUrl() : qx::IxPersistable(), baseUrl_id(QUuid::createUuid().toString()){;}
    virtual ~baseUrl() {;}
#ifndef _QX_NO_JSON
    // -- function callable by introspection and REST API
    static QJsonValue helloWorld(const QJsonValue & request);
#endif // _QX_NO_JSON

};
QX_REGISTER_PRIMARY_KEY(baseUrl, QString);

QX_REGISTER_HPP_QX_NEWS(baseUrl,qx::trait::no_base_class_defined,0)
typedef std::shared_ptr<baseUrl> baseUrl_ptr;
typedef std::vector<baseUrl_ptr> baseUrl_list;
#endif // BASEURL_H
