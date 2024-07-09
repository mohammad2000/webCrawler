/****************************************************************************
**
** https://www.qxorm.com/
** Copyright (C) 2013 Lionel Marty (contact@qxorm.com)
**
** This file is part of the QxOrm library
**
** This software is provided 'as-is', without any express or implied
** warranty. In no event will the authors be held liable for any
** damages arising from the use of this software
**
** Commercial Usage
** Licensees holding valid commercial QxOrm licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Lionel Marty
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file 'license.gpl3.txt' included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met : http://www.gnu.org/copyleft/gpl.html
**
** If you are unsure which license is appropriate for your use, or
** if you have questions regarding the use of this file, please contact :
** contact@qxorm.com
**
****************************************************************************/

#include <QxPrecompiled.h>

#include <QxDao/IxSqlQueryBuilder.h>
#include <QxDao/QxSqlDatabase.h>
#include <QxDao/QxSqlRelationParams.h>
#include <QxDao/QxSqlGenerator/IxSqlGenerator.h>
#include <QxDao/IxDao_Helper.h>
#include <QxDao/QxSqlQuery.h>
#include <QxDao/QxSession.h>

#include <QxRegister/IxClass.h>

#include <QxMemLeak/mem_leak.h>

namespace qx {

struct Q_DECL_HIDDEN IxSqlQueryBuilder::IxSqlQueryBuilderImpl
{

   typedef QPair<QString, QString> type_id;
   typedef QHash<type_id, void *> type_ptr_by_id;
   typedef std::shared_ptr<type_ptr_by_id> type_ptr_by_id_ptr;
   typedef QList<type_ptr_by_id_ptr> type_lst_ptr_by_id;
   typedef std::shared_ptr<type_lst_ptr_by_id> type_lst_ptr_by_id_ptr;
   typedef QxCollection<QString, IxDataMember *> type_lst_data_member;
   typedef std::shared_ptr<type_lst_data_member> type_lst_data_member_ptr;

   type_lst_data_member_ptr m_lstDataMemberPtr;                      //!< Collection of 'IxDataMember' to build sql query
   std::shared_ptr<IxSqlRelationX> m_lstSqlRelationPtr;              //!< Collection of 'IxSqlRelation' to build sql query
   IxDataMember * m_pDataMemberId;                                   //!< Data member id for sql query
   QString m_sSqlQuery;                                              //!< Current sql query
   QString m_sTableName;                                             //!< Sql table name of current object
   QString m_sHashRelation;                                          //!< Optimization : hash to retrieve sql query with relation
   bool m_bCartesianProduct;                                         //!< Recordset can return cartesian product => same id in multiple records
   type_lst_ptr_by_id_ptr m_pIdX;                                    //!< Collection of id (and pointer associated) to avoid multiple fetch on same id (cartesian product)
   QxSoftDelete m_oSoftDelete;                                       //!< Soft delete (or logical delete) behavior
   QxSoftDelete m_oSoftDeleteEmpty;                                  //!< Keep an empty soft delete instance (used with qx::QxSession::ignoreSoftDelete())
   QHash<QString, QString> m_lstSqlQueryAlias;                       //!< List of sql alias to replace into sql query
   qx::dao::detail::IxDao_Helper * m_pDaoHelper;                     //!< Pointer to the dao helper class associated to the builder
   IxDataMemberX * m_pDataMemberX;                                   //!< QxDataMemberX<type_sql> singleton reference
   bool m_bInitDone;                                                 //!< Class initialisation finished
   QString m_sCurrentBuildingSql;                                    //!< Current building SQL query (not finished yet)

   static QHash<QString, QString> m_lstSqlQuery;                     //!< Store here all SQL queries generated by child classes
   static QHash<QString, QHash<QString, QString> > m_lstSqlAlias;    //!< Store here all SQL aliases generated by child classes
   static QMutex m_mutex;                                            //!< Mutex => qx::IxSqlQueryBuilder is thread-safe

   IxSqlQueryBuilderImpl() : m_pDataMemberId(NULL), m_bCartesianProduct(false), m_pDaoHelper(NULL), m_pDataMemberX(NULL), m_bInitDone(false) { ; }
   ~IxSqlQueryBuilderImpl() { ; }

   bool checkIgnoreSoftDelete() const
   {
      if ((! m_pDaoHelper) || (! m_pDataMemberX)) { return false; }
      qx::QxSession * pSession = m_pDaoHelper->getSession(); if (! pSession) { return false; }
      qx::IxClass * pClass = m_pDataMemberX->getClass(); if (! pClass) { return false; }
      return pSession->checkIgnoreSoftDelete(pClass->getKey());
   }

};

QHash<QString, QString> IxSqlQueryBuilder::IxSqlQueryBuilderImpl::m_lstSqlQuery;
QHash<QString, QHash<QString, QString> > IxSqlQueryBuilder::IxSqlQueryBuilderImpl::m_lstSqlAlias;
QMutex IxSqlQueryBuilder::IxSqlQueryBuilderImpl::m_mutex;

IxSqlQueryBuilder::IxSqlQueryBuilder() : m_pImpl(new IxSqlQueryBuilderImpl()) { ; }

IxSqlQueryBuilder::~IxSqlQueryBuilder() { ; }

IxDataMemberX * IxSqlQueryBuilder::getDataMemberX() const { return m_pImpl->m_pDataMemberX; }

QxCollection<QString, IxDataMember *> * IxSqlQueryBuilder::getLstDataMember() const { return m_pImpl->m_lstDataMemberPtr.get(); }

IxSqlRelationX * IxSqlQueryBuilder::getLstRelation() const { return m_pImpl->m_lstSqlRelationPtr.get(); }

qx::dao::detail::IxDao_Helper * IxSqlQueryBuilder::getDaoHelper() const { return m_pImpl->m_pDaoHelper; }

void IxSqlQueryBuilder::setDaoHelper(qx::dao::detail::IxDao_Helper * p) { m_pImpl->m_pDaoHelper = p; }

void IxSqlQueryBuilder::setHashRelation(const QString & s) { m_pImpl->m_sHashRelation = s; }

void IxSqlQueryBuilder::setCartesianProduct(bool b) { m_pImpl->m_bCartesianProduct = b; }

QString IxSqlQueryBuilder::getSqlQuery() const { return m_pImpl->m_sSqlQuery; }

QString IxSqlQueryBuilder::getHashRelation() const { return m_pImpl->m_sHashRelation; }

QString IxSqlQueryBuilder::table() const { return m_pImpl->m_sTableName; }

QxSoftDelete IxSqlQueryBuilder::getSoftDelete() const { return (m_pImpl->checkIgnoreSoftDelete() ? m_pImpl->m_oSoftDeleteEmpty : m_pImpl->m_oSoftDelete); }

bool IxSqlQueryBuilder::getCartesianProduct() const { return m_pImpl->m_bCartesianProduct; }

long IxSqlQueryBuilder::getDataCount() const { return (m_pImpl->m_lstDataMemberPtr ? m_pImpl->m_lstDataMemberPtr->count() : 0); }

long IxSqlQueryBuilder::getRelationCount() const { return (m_pImpl->m_lstSqlRelationPtr ? m_pImpl->m_lstSqlRelationPtr->count() : 0); }

IxDataMember * IxSqlQueryBuilder::getDataId() const { return m_pImpl->m_pDataMemberId; }

IxDataMember * IxSqlQueryBuilder::nextData(long & l) const { if ((! m_pImpl->m_lstDataMemberPtr) || (l < 0) || (l >= m_pImpl->m_lstDataMemberPtr->count())) { return NULL; }; ++l; return m_pImpl->m_lstDataMemberPtr->getByIndex(l - 1); }

IxSqlRelation * IxSqlQueryBuilder::nextRelation(long & l) const { if ((! m_pImpl->m_lstSqlRelationPtr) || (l < 0) || (l >= m_pImpl->m_lstSqlRelationPtr->count())) { return NULL; }; ++l; return m_pImpl->m_lstSqlRelationPtr->getByIndex(l - 1); }

QString & IxSqlQueryBuilder::getCurrentBuildingSql() const { return m_pImpl->m_sCurrentBuildingSql; }

bool IxSqlQueryBuilder::isInitDone() const { return m_pImpl->m_bInitDone; }

QxSoftDelete & IxSqlQueryBuilder::softDelete() { return (m_pImpl->checkIgnoreSoftDelete() ? m_pImpl->m_oSoftDeleteEmpty : m_pImpl->m_oSoftDelete); }

const QxSoftDelete & IxSqlQueryBuilder::softDelete() const { return (m_pImpl->checkIgnoreSoftDelete() ? m_pImpl->m_oSoftDeleteEmpty : m_pImpl->m_oSoftDelete); }

void IxSqlQueryBuilder::setSoftDelete(const QxSoftDelete & o) { m_pImpl->m_oSoftDelete = o; }

void IxSqlQueryBuilder::setDataMemberX(IxDataMemberX * p) { m_pImpl->m_pDataMemberX = p; }

void IxSqlQueryBuilder::clone(const IxSqlQueryBuilder & other)
{
   this->m_pImpl->m_lstDataMemberPtr = other.m_pImpl->m_lstDataMemberPtr;
   this->m_pImpl->m_lstSqlRelationPtr = other.m_pImpl->m_lstSqlRelationPtr;
   this->m_pImpl->m_pDataMemberId = other.m_pImpl->m_pDataMemberId;
   this->m_pImpl->m_sSqlQuery = other.m_pImpl->m_sSqlQuery;
   this->m_pImpl->m_sTableName = other.m_pImpl->m_sTableName;
   this->m_pImpl->m_sHashRelation = other.m_pImpl->m_sHashRelation;
   this->m_pImpl->m_bCartesianProduct = other.m_pImpl->m_bCartesianProduct;
   this->m_pImpl->m_pIdX = other.m_pImpl->m_pIdX;
   this->m_pImpl->m_oSoftDelete = other.m_pImpl->m_oSoftDelete;
   this->m_pImpl->m_lstSqlQueryAlias = other.m_pImpl->m_lstSqlQueryAlias;
   this->m_pImpl->m_pDaoHelper = other.m_pImpl->m_pDaoHelper;
   this->m_pImpl->m_pDataMemberX = other.m_pImpl->m_pDataMemberX;
}

void IxSqlQueryBuilder::init()
{
   if (! m_pImpl->m_pDataMemberX || ! m_pImpl->m_pDataMemberX->getClass() || m_pImpl->m_bInitDone) { return; }
   m_pImpl->m_pDataMemberId = m_pImpl->m_pDataMemberX->getId_WithDaoStrategy();
   m_pImpl->m_sTableName = m_pImpl->m_pDataMemberX->getName();
   m_pImpl->m_lstDataMemberPtr = m_pImpl->m_pDataMemberX->getClass()->getSqlDataMemberX();
   m_pImpl->m_lstSqlRelationPtr = m_pImpl->m_pDataMemberX->getClass()->getSqlRelationX();
   m_pImpl->m_bInitDone = true;
}

void IxSqlQueryBuilder::setSqlQuery(const QString & sql, const QString & key /* = QString() */)
{
   m_pImpl->m_sSqlQuery = sql;
   if (! key.isEmpty())
   {
      QMutexLocker locker(& IxSqlQueryBuilderImpl::m_mutex);
      IxSqlQueryBuilderImpl::m_lstSqlQuery.insert(key, sql);
   }
}

bool IxSqlQueryBuilder::findSqlQuery(const QString & key)
{
   if (key.isEmpty()) { return false; }
   QMutexLocker locker(& IxSqlQueryBuilderImpl::m_mutex);
   QString sql = IxSqlQueryBuilderImpl::m_lstSqlQuery.value(key);
   if (! sql.isEmpty()) { m_pImpl->m_sSqlQuery = sql; }
   return (! sql.isEmpty());
}

bool IxSqlQueryBuilder::findSqlAlias(const QString & key)
{
   if (key.isEmpty()) { return false; }
   QMutexLocker locker(& IxSqlQueryBuilderImpl::m_mutex);
   if (! IxSqlQueryBuilderImpl::m_lstSqlAlias.contains(key)) { return false; }
   m_pImpl->m_lstSqlQueryAlias = IxSqlQueryBuilderImpl::m_lstSqlAlias.value(key);
   return true;
}

void IxSqlQueryBuilder::insertSqlAlias(const QString & key)
{
   if (key.isEmpty()) { return; }
   QMutexLocker locker(& IxSqlQueryBuilderImpl::m_mutex);
   IxSqlQueryBuilderImpl::m_lstSqlAlias.insert(key, m_pImpl->m_lstSqlQueryAlias);
}

void IxSqlQueryBuilder::initIdX(long lAllRelationCount)
{
   if (! m_pImpl->m_bCartesianProduct) { qAssert(false); return; }
   m_pImpl->m_pIdX = std::make_shared<IxSqlQueryBuilderImpl::type_lst_ptr_by_id>();
   for (long l = 0; l < (lAllRelationCount + 1); ++l)
   { IxSqlQueryBuilderImpl::type_ptr_by_id_ptr pItem = IxSqlQueryBuilderImpl::type_ptr_by_id_ptr(new IxSqlQueryBuilderImpl::type_ptr_by_id()); m_pImpl->m_pIdX->append(pItem); }
}

bool IxSqlQueryBuilder::insertIdX(long lIndex, const QVariant & idOwner, const QVariant & idData, void * ptr)
{
   QString sIdOwner = idOwner.toString(); QString sIdData = idData.toString();
   if (! m_pImpl->m_pIdX || sIdOwner.isEmpty() || sIdData.isEmpty()) { qAssert(false); return false; }
   if ((lIndex < 0) || (lIndex >= m_pImpl->m_pIdX->count())) { qAssert(false); return false; }

   IxSqlQueryBuilderImpl::type_id idX(sIdOwner, sIdData);
   IxSqlQueryBuilderImpl::type_ptr_by_id_ptr pHash = m_pImpl->m_pIdX->at(lIndex);
   if (! ptr || ! pHash || pHash->contains(idX)) { qAssert(false); return false; }
   pHash->insert(idX, ptr);

   return true;
}

void * IxSqlQueryBuilder::existIdX(long lIndex, const QVariant & idOwner, const QVariant & idData)
{
   QString sIdOwner = idOwner.toString(); QString sIdData = idData.toString();
   if (! m_pImpl->m_pIdX || sIdOwner.isEmpty() || sIdData.isEmpty()) { qAssert(false); return NULL; }
   if ((lIndex < 0) || (lIndex >= m_pImpl->m_pIdX->count())) { qAssert(false); return NULL; }

   IxSqlQueryBuilderImpl::type_id idX(sIdOwner, sIdData);
   IxSqlQueryBuilderImpl::type_ptr_by_id_ptr pHash = m_pImpl->m_pIdX->at(lIndex);
   if (! pHash || ! pHash->contains(idX)) { return NULL; }

   return pHash->value(idX);
}

bool IxSqlQueryBuilder::getAddAutoIncrementIdToUpdateQuery() const
{
   return qx::QxSqlDatabase::getSingleton()->getAddAutoIncrementIdToUpdateQuery();
}

void IxSqlQueryBuilder::addSqlQueryAlias(const QString & sql, const QString & sqlAlias)
{
   m_pImpl->m_lstSqlQueryAlias.insert(sql, sqlAlias);
}

void IxSqlQueryBuilder::replaceSqlQueryAlias(QString & sql) const
{
   if (! qx::QxSqlDatabase::getSingleton()->getAutoReplaceSqlAliasIntoQuery()) { return; }
   if (m_pImpl->m_lstSqlQueryAlias.count() <= 0) { return; }
   QHashIterator<QString, QString> itr(m_pImpl->m_lstSqlQueryAlias);
   sql = (" " + sql);
   while (itr.hasNext())
   {
      itr.next();
      QString sBefore = (" " + itr.key() + ".");
      QString sAfter = (" " + itr.value() + ".");
      sql.replace(sBefore, sAfter);
   }
   sql = sql.trimmed();
}

bool IxSqlQueryBuilder::verifyColumns(const QStringList & columns) const
{
#ifdef _QX_MODE_DEBUG
   if (! m_pImpl->m_pDataMemberX) { qAssert(false); return false; }
   for (int i = 0; i < columns.count(); i++)
   {
      if (! m_pImpl->m_pDataMemberX->exist_WithDaoStrategy(columns.at(i)))
      {
         QString sErrorMsg = QString("column '%1' not found in table '%2'").arg(columns.at(i), m_pImpl->m_sTableName);
         qDebug("[QxOrm] Error in qx::IxSqlQueryBuilder::verifyColumns() : %s", qPrintable(sErrorMsg));
         qAssertMsg(false, "[QxOrm] qx::IxSqlQueryBuilder::verifyColumns()", qPrintable(sErrorMsg));
         return false;
      }
   }
   return true;
#else // _QX_MODE_DEBUG
   Q_UNUSED(columns);
   return true;
#endif // _QX_MODE_DEBUG
}

void IxSqlQueryBuilder::sql_CreateTable(QString & sql, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxSqlRelation * pRelation = NULL;
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "CREATE TABLE " + qx::IxDataMember::getSqlTableName(table) + " (";
   int iSqlCountRef = sql.count();
   if (pId) { sql += pId->getSqlNameAndTypeAndParams(", ") + ", "; qAssert(! pId->getSqlType().isEmpty()); }
   while ((p = builder.nextData(l1))) { sql += p->getSqlNameAndTypeAndParams(", ") + ", "; qAssert(! p->getSqlType().isEmpty()); }
   if (! oSoftDelete.isEmpty()) { sql += oSoftDelete.buildSqlQueryToCreateTable() + ", "; }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->createTable(params); }
   bool bAddBracket = (sql.count() != iSqlCountRef);
   sql = sql.left(sql.count() - 2); // Remove last ", "
   if (bAddBracket) { sql += ")"; }
}

void IxSqlQueryBuilder::sql_DeleteById(QString & sql, IxSqlQueryBuilder & builder, bool bSoftDelete)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   if (bSoftDelete && ! oSoftDelete.isEmpty()) { sql = "UPDATE " + qx::IxDataMember::getSqlTableName(table) + " SET " + oSoftDelete.buildSqlQueryToUpdate(); }
   else { sql = "DELETE FROM " + qx::IxDataMember::getSqlFromTable(table); }
   sql += IxSqlQueryBuilder::addSqlCondition(sql);
   sql += pId->getSqlNameEqualToPlaceHolder("", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_Exist(QString & sql, IxSqlQueryBuilder & builder)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "SELECT ";
   if (pId) { sql += pId->getSqlTablePointNameAsAlias(table, ", ", "", false, "", (& builder)); }
   if (! oSoftDelete.isEmpty()) { sql += ", " + oSoftDelete.buildSqlTablePointName(); }
   sql += " FROM " + qx::IxDataMember::getSqlFromTable(table);
   sql += " WHERE " + pId->getSqlAliasEqualToPlaceHolder(table, true, "", " AND ", false, (& builder));
   if (! oSoftDelete.isEmpty()) { sql += " AND " + oSoftDelete.buildSqlQueryToFetch(); }
}

void IxSqlQueryBuilder::sql_FetchAll(QString & sql, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0), l3(0), l4(0), l5(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxSqlRelation * pRelation = NULL;
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "SELECT ";
   bool isDistinct = (builder.getDaoHelper() ? builder.getDaoHelper()->isDistinct() : false);
   if (pId && (! isDistinct)) { sql += (pId->getSqlTablePointNameAsAlias(table, ", ", "", false, "", (& builder)) + ", "); }
   while ((p = builder.nextData(l1))) { sql += (p->getSqlTablePointNameAsAlias(table, ", ", "", false, "", (& builder)) + ", "); }
   if (! oSoftDelete.isEmpty()) { l1++; sql += (oSoftDelete.buildSqlTablePointName() + ", "); }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazySelect(params); }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += " FROM " + qx::IxDataMember::getSqlFromTable(table) + ", ";
   while ((pRelation = builder.nextRelation(l3))) { params.setIndex(l3); pRelation->lazyFrom(params); }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   while ((pRelation = builder.nextRelation(l4))) { params.setIndex(l4); pRelation->lazyJoin(params); }
   if (! oSoftDelete.isEmpty()) { sql += " WHERE " + oSoftDelete.buildSqlQueryToFetch(); }
   while ((pRelation = builder.nextRelation(l5))) { params.setIndex(l5); pRelation->lazyWhereSoftDelete(params); }
}

void IxSqlQueryBuilder::sql_FetchAll(QString & sql, IxSqlQueryBuilder & builder, const QStringList & columns)
{
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxDataMemberX * pDataMemberX = builder.getDataMemberX(); qAssert(pDataMemberX);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "SELECT ";
   bool isDistinct = (builder.getDaoHelper() ? builder.getDaoHelper()->isDistinct() : false);
   if (pId && (! isDistinct)) { sql += (pId->getSqlTablePointNameAsAlias(table, ", ", "", false, "", (& builder)) + ", "); }
   for (int i = 0; i < columns.count(); i++)
   { p = pDataMemberX->get_WithDaoStrategy(columns.at(i)); if (p && (p != pId)) { sql += (p->getSqlTablePointNameAsAlias(table, ", ", "", false, "", (& builder)) + ", "); } }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   if (! oSoftDelete.isEmpty()) { sql += ", " + oSoftDelete.buildSqlTablePointName(); }
   sql += " FROM " + qx::IxDataMember::getSqlFromTable(table);
   if (! oSoftDelete.isEmpty()) { sql += " WHERE " + oSoftDelete.buildSqlQueryToFetch(); }
}

void IxSqlQueryBuilder::sql_FetchAll_WithRelation(qx::QxSqlRelationLinked * pRelationX, QString & sql, IxSqlQueryBuilder & builder)
{
   long l(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "SELECT ";
   bool isDistinct = (builder.getDaoHelper() ? builder.getDaoHelper()->isDistinct() : false);
   bool bForceRootId = ((isDistinct && pId) ? pRelationX->checkRootColumns(pId->getKey()) : false);
   if (pId && ((! isDistinct) || (bForceRootId))) { sql += (pId->getSqlTablePointNameAsAlias(table, ", ", "", false, pRelationX->getRootCustomAlias(), (& builder)) + ", "); }
   while ((p = builder.nextData(l))) { if (pRelationX->checkRootColumns(p->getKey())) { sql += (p->getSqlTablePointNameAsAlias(table, ", ", "", false, pRelationX->getRootCustomAlias(), (& builder)) + ", "); } }
   if (! oSoftDelete.isEmpty()) { l++; sql += (oSoftDelete.buildSqlTablePointName(pRelationX->getRootCustomAlias()) + ", "); }
   pRelationX->hierarchySelect(params);
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += " FROM " + qx::IxDataMember::getSqlFromTable(table, pRelationX->getRootCustomAlias()) + ", ";
   pRelationX->hierarchyFrom(params);
   sql = sql.left(sql.count() - 2); // Remove last ", "
   pRelationX->hierarchyJoin(params);
   if (! oSoftDelete.isEmpty()) { sql += " WHERE " + oSoftDelete.buildSqlQueryToFetch(pRelationX->getRootCustomAlias()); }
   pRelationX->hierarchyWhereSoftDelete(params);
}

void IxSqlQueryBuilder::sql_FetchById(QString & sql, IxSqlQueryBuilder & builder)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   QString table = builder.table();
   sql = builder.buildSql().getSqlQuery();
   sql += IxSqlQueryBuilder::addSqlCondition(sql);
   sql += pId->getSqlAliasEqualToPlaceHolder(table, true, "", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_FetchById(QString & sql, IxSqlQueryBuilder & builder, const QStringList & columns)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   QString table = builder.table();
   sql = builder.buildSql(columns).getSqlQuery();
   sql += IxSqlQueryBuilder::addSqlCondition(sql);
   sql += pId->getSqlAliasEqualToPlaceHolder(table, true, "", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_FetchById_WithRelation(qx::QxSqlRelationLinked * pRelationX, QString & sql, IxSqlQueryBuilder & builder)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   QString table = builder.table(); QStringList columns;
   sql = builder.buildSql(columns, pRelationX).getSqlQuery();
   sql += IxSqlQueryBuilder::addSqlCondition(sql);
   sql += pId->getSqlAliasEqualToPlaceHolder(table, true, "", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_Insert(QString & sql, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxSqlRelation * pRelation = NULL;
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   QString table = builder.table(); QString tmp;
   sql = "INSERT INTO " + qx::IxDataMember::getSqlTableName(table) + " (";
   if (pId && ! pId->getAutoIncrement()) { tmp = pId->getSqlName(", ", "", true, (& builder)); if (! tmp.isEmpty()) { sql += tmp + ", "; } }
   while ((p = builder.nextData(l1))) { sql += p->getSqlName(", ", "", false, (& builder)) + ", "; }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert(params); }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += ") VALUES (";
   l1 = 0; l2 = 0; p = NULL; pRelation = NULL;
   if (pId && ! pId->getAutoIncrement()) { tmp = pId->getSqlPlaceHolder("", -1, ", ", "", true); if (! tmp.isEmpty()) { sql += tmp + ", "; } }
   while ((p = builder.nextData(l1))) { sql += p->getSqlPlaceHolder("", -1, ", ") + ", "; }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert_Values(params); }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += ")";
}

void IxSqlQueryBuilder::sql_Update(QString & sql, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::IxSqlRelation * pRelation = NULL;
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   QString table = builder.table(); QString tmp;
   sql = "UPDATE " + qx::IxDataMember::getSqlTableName(table) + " SET ";
   if (! pId->getAutoIncrement() || (pId->getAutoIncrement() && builder.getAddAutoIncrementIdToUpdateQuery()))
   { tmp = pId->getSqlNameEqualToPlaceHolder("", ", ", true, (& builder)); if (! tmp.isEmpty()) { sql += tmp + ", "; } }
   while ((p = builder.nextData(l1))) { sql += p->getSqlNameEqualToPlaceHolder("", ", ", false, (& builder)) + ", "; }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyUpdate(params); }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += " WHERE " + pId->getSqlNameEqualToPlaceHolder("_bis", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_Update(QString & sql, IxSqlQueryBuilder & builder, const QStringList & columns)
{
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::IxDataMemberX * pDataMemberX = builder.getDataMemberX(); qAssert(pDataMemberX);
   QString table = builder.table();
   sql = "UPDATE " + qx::IxDataMember::getSqlTableName(table) + " SET ";
   if (! pId->getAutoIncrement() || (pId->getAutoIncrement() && builder.getAddAutoIncrementIdToUpdateQuery()))
   { sql += pId->getSqlNameEqualToPlaceHolder("", ", ", false, (& builder)) + ", "; }
   for (int i = 0; i < columns.count(); i++)
   { p = pDataMemberX->get_WithDaoStrategy(columns.at(i)); if (p && (p != pId)) { sql += p->getSqlNameEqualToPlaceHolder("", ", ", false, (& builder)) + ", "; } }
   sql = sql.left(sql.count() - 2); // Remove last ", "
   sql += " WHERE " + pId->getSqlNameEqualToPlaceHolder("_bis", " AND ", false, (& builder));
}

void IxSqlQueryBuilder::sql_Count_WithRelation(qx::QxSqlRelationLinked * pRelationX, QString & sql, IxSqlQueryBuilder & builder)
{
   if (! pRelationX) { qAssert(false); return; }
   qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   QString table = builder.table();
   sql = "SELECT COUNT(*) FROM " + qx::IxDataMember::getSqlFromTable(table, pRelationX->getRootCustomAlias()) + ", ";
   pRelationX->hierarchyFrom(params);
   sql = sql.left(sql.count() - 2); // Remove last ", "
   pRelationX->hierarchyJoin(params);
   if (! oSoftDelete.isEmpty()) { sql += " WHERE " + oSoftDelete.buildSqlQueryToFetch(pRelationX->getRootCustomAlias()); }
   pRelationX->hierarchyWhereSoftDelete(params);
}

void IxSqlQueryBuilder::resolveOutput_FetchAll(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxSqlRelation * pRelation = NULL;
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   bool isDistinct = (builder.getDaoHelper() ? builder.getDaoHelper()->isDistinct() : false);
   short iOffset = ((pId && (! isDistinct)) ? pId->getNameCount() : 0);
   if (pId && (! isDistinct)) { for (int i = 0; i < pId->getNameCount(); i++) { pId->fromVariant(t, query.value(i), i, qx::cvt::context::e_database); } }
   while ((p = builder.nextData(l1))) { p->fromVariant(t, query.value(l1 + iOffset - 1), -1, qx::cvt::context::e_database); }
   iOffset = (builder.getDataCount() + iOffset + (oSoftDelete.isEmpty() ? 0 : 1));
   qx::QxSqlRelationParams params(0, iOffset, NULL, (& builder), (& query), t);
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyFetch_ResolveOutput(params); }
}

void IxSqlQueryBuilder::resolveOutput_FetchAll(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder, const QStringList & columns)
{
   qx::IxDataMember * p = NULL; int idx = 0;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxDataMemberX * pDataMemberX = builder.getDataMemberX(); qAssert(pDataMemberX);
   bool isDistinct = (builder.getDaoHelper() ? builder.getDaoHelper()->isDistinct() : false);
   short iOffset = ((pId && (! isDistinct)) ? pId->getNameCount() : 0);
   if (pId && (! isDistinct)) { for (int i = 0; i < pId->getNameCount(); i++) { pId->fromVariant(t, query.value(i), i, qx::cvt::context::e_database); } }
   for (int i = 0; i < columns.count(); i++)
   { p = pDataMemberX->get_WithDaoStrategy(columns.at(i)); if (p && (p != pId)) { p->fromVariant(t, query.value(idx + iOffset), -1, qx::cvt::context::e_database); idx++; } }
}

void IxSqlQueryBuilder::resolveOutput_FetchAll_WithRelation(qx::QxSqlRelationLinked * pRelationX, void * t, QSqlQuery & query, IxSqlQueryBuilder & builder)
{
   long l(0); long lCurrIndex(0); QVariant vId;
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::QxSoftDelete oSoftDelete = builder.getSoftDelete();
   qx::dao::detail::IxDao_Helper * pDaoHelper = builder.getDaoHelper();
   bool isDistinct = (pDaoHelper ? pDaoHelper->isDistinct() : false);
   bool bForceRootId = ((isDistinct && pId) ? pRelationX->checkRootColumns(pId->getKey()) : false);
   short iOffsetId = ((pId && ((! isDistinct) || (bForceRootId))) ? pId->getNameCount() : 0);
   vId = (pDaoHelper ? pDaoHelper->getIdFromQuery(-1) : QVariant());
   bool bComplex = builder.getCartesianProduct();
   bool bByPass = (bComplex && builder.existIdX(0, vId, vId));

   if (! bByPass)
   {
      if (pId && ((! isDistinct) || (bForceRootId))) { for (int i = 0; i < pId->getNameCount(); i++) { pId->fromVariant(t, query.value(i), i, qx::cvt::context::e_database); } }
      while ((p = builder.nextData(l))) { if (pRelationX->checkRootColumns(p->getKey())) { p->fromVariant(t, query.value(lCurrIndex + iOffsetId), -1, qx::cvt::context::e_database); lCurrIndex++; } }
      if (bComplex) { builder.insertIdX(0, vId, vId, t); }
   }

   short iOffset = (builder.getDataCount() + iOffsetId + (oSoftDelete.isEmpty() ? 0 : 1));
   if ((pRelationX->getRootColumnsCount() > 0) && (pRelationX->getRootColumnsOffset() > 0))
   { iOffset = (iOffset - pRelationX->getRootColumnsOffset()); }
   else if (pRelationX->getRootColumnsCount() > 0)
   {
      l = 0; p = NULL; long lRootColumnsOffset = 0;
      while ((p = builder.nextData(l))) { if (! pRelationX->checkRootColumns(p->getKey())) { iOffset = (iOffset - 1); lRootColumnsOffset++; } }
      pRelationX->setRootColumnsOffset(lRootColumnsOffset);
   }

   qx::QxSqlRelationParams params(0, iOffset, NULL, (& builder), (& query), t, vId);
   pRelationX->hierarchyResolveOutput(params);
}

void IxSqlQueryBuilder::resolveInput_Insert(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId();
   qx::IxSqlRelation * pRelation = NULL;
   bool bUseExecBatch = (builder.getDaoHelper() ? builder.getDaoHelper()->getUseExecBatch() : false);
   qx::QxCollection<QString, QVariantList> * pLstExecBatch = (bUseExecBatch ? (& builder.getDaoHelper()->getListExecBatch()) : NULL);
   qx::QxSqlRelationParams params(0, 0, NULL, (& builder), (& query), t, QVariant(), pLstExecBatch);
   if (pId && ! pId->getAutoIncrement()) { pId->setSqlPlaceHolder(query, t, "", "", true, pLstExecBatch); }
   while ((p = builder.nextData(l1))) { p->setSqlPlaceHolder(query, t, "", "", false, pLstExecBatch); }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert_ResolveInput(params); }
}

void IxSqlQueryBuilder::resolveInput_Update(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder)
{
   long l1(0), l2(0);
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::IxSqlRelation * pRelation = NULL;
   bool bUseExecBatch = (builder.getDaoHelper() ? builder.getDaoHelper()->getUseExecBatch() : false);
   qx::QxCollection<QString, QVariantList> * pLstExecBatch = (bUseExecBatch ? (& builder.getDaoHelper()->getListExecBatch()) : NULL);
   qx::QxSqlRelationParams params(0, 0, NULL, (& builder), (& query), t, QVariant(), pLstExecBatch);
   if (! pId->getAutoIncrement() || (pId->getAutoIncrement() && builder.getAddAutoIncrementIdToUpdateQuery()))
   { pId->setSqlPlaceHolder(query, t, "", "", true, pLstExecBatch); }
   while ((p = builder.nextData(l1))) { p->setSqlPlaceHolder(query, t, "", "", false, pLstExecBatch); }
   while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyUpdate_ResolveInput(params); }
   pId->setSqlPlaceHolder(query, t, "_bis", "", false, pLstExecBatch);
}

void IxSqlQueryBuilder::resolveInput_Update(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder, const QStringList & columns)
{
   qx::IxDataMember * p = NULL;
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   qx::IxDataMemberX * pDataMemberX = builder.getDataMemberX(); qAssert(pDataMemberX);
   bool bUseExecBatch = (builder.getDaoHelper() ? builder.getDaoHelper()->getUseExecBatch() : false);
   qx::QxCollection<QString, QVariantList> * pLstExecBatch = (bUseExecBatch ? (& builder.getDaoHelper()->getListExecBatch()) : NULL);
   if (! pId->getAutoIncrement() || (pId->getAutoIncrement() && builder.getAddAutoIncrementIdToUpdateQuery()))
   { pId->setSqlPlaceHolder(query, t, "", "", false, pLstExecBatch); }
   for (int i = 0; i < columns.count(); i++)
   { p = pDataMemberX->get_WithDaoStrategy(columns.at(i)); if (p && (p != pId)) { p->setSqlPlaceHolder(query, t, "", "", false, pLstExecBatch); } }
   pId->setSqlPlaceHolder(query, t, "_bis", "", false, pLstExecBatch);
}

void IxSqlQueryBuilder::resolveInput_DeleteById(void * t, QSqlQuery & query, IxSqlQueryBuilder & builder)
{
   qx::IxDataMember * pId = builder.getDataId(); qAssert(pId);
   bool bUseExecBatch = (builder.getDaoHelper() ? builder.getDaoHelper()->getUseExecBatch() : false);
   qx::QxCollection<QString, QVariantList> * pLstExecBatch = (bUseExecBatch ? (& builder.getDaoHelper()->getListExecBatch()) : NULL);
   pId->setSqlPlaceHolder(query, t, "", "", false, pLstExecBatch);
}

} // namespace qx