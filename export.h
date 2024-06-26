#ifndef _QX_NEWS_EXPORT_H_
#define _QX_NEWS_EXPORT_H_

#ifdef _BUILDING_QX_NEWS
#define QX_NEWS_DLL_EXPORT QX_DLL_EXPORT_HELPER
#else // _BUILDING_QX_BLOG
#define QX_NEWS_DLL_EXPORT QX_DLL_IMPORT_HELPER
#endif // _BUILDING_QX_NEWS

#ifdef _BUILDING_QX_NEWS
#define QX_REGISTER_HPP_QX_NEWS     QX_REGISTER_HPP_EXPORT_DLL
#define QX_REGISTER_CPP_QX_NEWS     QX_REGISTER_CPP_EXPORT_DLL
#else // _BUILDING_QX_NEWS
#define QX_REGISTER_HPP_QX_NEWS    QX_REGISTER_HPP_IMPORT_DLL
#define QX_REGISTER_CPP_QX_NEWS     QX_REGISTER_CPP_IMPORT_DLL
#endif // _BUILDING_QX_NEWS

#endif // _QX_NEWS_EXPORT_H_

