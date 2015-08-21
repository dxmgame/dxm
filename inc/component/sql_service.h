#ifndef _____SQL_SERVICE_H_________
#define _____SQL_SERVICE_H_________

#include "component_service/component_service.h"

#include "log_service.h"

namespace Foundation {

	namespace SqlService{

		/*
		 *	��ѯ����ö��;
		 */
		enum eSqlQueryError
		{
			SQL_QUERY_ERROR_OK,
			SQL_QUERY_ERROR_DISCONNECT,
			SQL_QUERY_ERROR_OTHER,
			SQL_QUERY_ERROR_MAX,
		};

		/*
		 *	�ֶ�����ö��;
		 */
		enum eSqlParamType
		{
			SQL_PARAM_TYPE_BYTE,
			SQL_PARAM_TYPE_SHORT,
			SQL_PARAM_TYPE_WORD,
			SQL_PARAM_TYPE_INT,
			SQL_PARAM_TYPE_DWORD,
			SQL_PARAM_TYPE_FLOAT,
			SQL_PARAM_TYPE_DOUBLE,
			SQL_PARAM_TYPE_STRING,
			SQL_PARAM_TYPE_BLOB,
			SQL_PARAM_TYPE_DATETIME,
			SQL_PARAM_TYPE_MAX,
		};

		/*
		 *	��ѯ�����;
		 */
		class ISqlResult
		{
		public:
			typedef boost::shared_ptr<ISqlResult> Ptr;

			ISqlResult(IComponent::Ptr &sql_connector_depend) : sql_connector_depend_(sql_connector_depend) {}
			virtual ~ISqlResult(void) {}

			/*
			 *	��ȡ�ֶ�����;
			 */
			virtual unsigned int GetFieldCount(void) = 0;

			/*
			 *	��ȡ�ֶ�����;
			 */
			virtual const char *GetFieldName(unsigned int field_index) = 0;

			/*
			 *	���ݱ�Ż�ȡ�ֶ�����;
			 */
			virtual const char *GetFieldValueByIndex(unsigned int field_index, unsigned int &size) = 0;

			/*
			 *	�������ƻ�ȡ�ֶ�����;
			 */
			virtual const char *GetFieldValueByName(const char *field_name, unsigned int &size) = 0;

			/*
			 *	��ȡ��¼����;
			 */
			virtual unsigned int GetRowCount(void) = 0;

			/*
			 *	����ɨ���¼;
			 */
			virtual bool FetchRow(void) = 0;

		protected:

			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø����;
			 */
			IComponent::Ptr sql_connector_depend_;
		};

		/*
		 *	���ݿ�����;
		 */
		class ISqlConnector : public IComponent
		{
		public:
			typedef boost::shared_ptr<ISqlConnector> Ptr;

			/*
			 *	�������ݿ�;
			 */
			virtual bool Connect(const char *ip, unsigned int port, const char *id, const char *password, const char *database_name, const char *charset_name) = 0;

			/*
			 *	�ر����ݿ�����;
			 */
			virtual void Shutdown(void) = 0;

			/*
			 *	ִ�м����;
			 */
			virtual ISqlResult::Ptr ExecuteSql(const char *sql, eSqlQueryError &error) = 0;

			/*
			 *	ִ�и�����䣬�����ı����;
			 */
			virtual bool ExecuteSqlBegin(const char *sql) = 0;

			/*
			 *	ִ�и�����䣬�����ֶ�����;
			 */
			virtual bool ExecuteSqlPushParam(eSqlParamType database_param_type, void *buffer, unsigned int size) = 0;

			/*
			 *	ִ�и�����䣬��ʽִ�в��õ����;
			 */
			virtual bool ExecuteSqlEnd(eSqlQueryError &error) = 0;

			/*
			 *	������־����;
			 */
			virtual void set_log(Foundation::LogService::ILog::Ptr log) = 0;

			/*
			 *	��ȡ�������;
			 */
			virtual unsigned int GetAutoIncrementID(void) = 0;
		};

	};

};

#endif
