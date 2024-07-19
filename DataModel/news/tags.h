#ifndef _QX_NEWS_TAGS_H_
#define _QX_NEWS_TAGS_H_
class baseUrl;

class QX_NEWS_DLL_EXPORT tags : public qx::IxPersistable{
    QX_PERSISTABLE_HPP(tags)
    public:

    QString     tags_id;
    QString     tags_tagName;
    QString     tags_selector;

    tags() : qx::IxPersistable(), tags_id(QUuid::createUuid().toString()){;}
    virtual ~tags(){;}
};
QX_REGISTER_PRIMARY_KEY(tags, QString);
QX_REGISTER_HPP_QX_NEWS(tags, qx::trait::no_base_class_defined, 0)

typedef std::shared_ptr<tags> tags_ptr;
typedef qx::QxCollection<QString,tags_ptr> tags_list;
#endif // TAGS_H
