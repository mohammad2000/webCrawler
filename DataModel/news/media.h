#ifndef _QX_NEWS_MEDIA_H_
#define _QX_NEWS_MEDIA_H_


class QX_NEWS_DLL_EXPORT media : public qx::IxPersistable{
    QX_PERSISTABLE_HPP(media)
    public:

    QString     media_id;
    QString     media_url;
    QString     media_base64;

    media() : qx::IxPersistable(), media_id(QUuid::createUuid().toString()){;}
    virtual ~media(){;}
};
QX_REGISTER_PRIMARY_KEY(media, QString);
QX_REGISTER_HPP_QX_NEWS(media, qx::trait::no_base_class_defined, 0)

typedef std::shared_ptr<media> media_ptr;
typedef std::vector<media_ptr> media_list;
#endif // MEDIA_H
