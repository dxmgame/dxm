#ifndef _____SQL_SERVICE_H_________
#define _____SQL_SERVICE_H_________

#include "component_service/component_service.h"

#include "log_service.h"

namespace Foundation {

	namespace SqlService{

		/*
		 *	查询错误枚举;
		 */
		enum eSqlQueryError
		{
			SQL_QUERY_ERROR_OK,
			SQL_QUERY_ERROR_DISCONNECT,
			SQL_QUERY_ERROR_OTHER,
			SQL_QUERY_ERROR_MAX,
		};

		/*
		 *	字段类型枚举;
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
		 *	查询结果集;
		 */
		class ISqlResult
		{
		public:
			typedef boost::shared_ptr<ISqlResult> Ptr;

			ISqlResult(IComponent::Ptr &sql_connector_depend) : sql_connector_depend_(sql_connector_depend) {}
			virtual ~ISqlResult(void) {}

			/*
			 *	获取字段数量;
			 */
			virtual unsigned int GetFieldCount(void) = 0;

			/*
			 *	获取字段名称;
			 */
			virtual const char *GetFieldName(unsigned int field_index) = 0;

			/*
			 *	根据编号获取字段内容;
			 */
			virtual const char *GetFieldValueByIndex(unsigned int field_index, unsigned int &size) = 0;

			/*
			 *	根据名称获取字段内容;
			 */
			virtual const char *GetFieldValueByName(const char *field_name, unsigned int &size) = 0;

			/*
			 *	获取记录数量;
			 */
			virtual unsigned int GetRowCount(void) = 0;

			/*
			 *	逐条扫描记录;
			 */
			virtual bool FetchRow(void) = 0;

		protected:

			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件;
			 */
			IComponent::Ptr sql_connector_depend_;
		};

		/*
		 *	数据库连接;
		 */
		class ISqlConnector : public IComponent
		{
		public:
			typedef boost::shared_ptr<ISqlConnector> Ptr;

			/*
			 *	连接数据库;
			 */
			virtual bool Connect(const char *ip, unsigned int port, const char *id, const char *password, const char *database_name, const char *charset_name) = 0;

			/*
			 *	关闭数据库连接;
			 */
			virtual void Shutdown(void) = 0;

			/*
			 *	执行简单语句;
			 */
			virtual ISqlResult::Ptr ExecuteSql(const char *sql, eSqlQueryError &error) = 0;

			/*
			 *	执行复杂语句，输入文本语句;
			 */
			virtual bool ExecuteSqlBegin(const char *sql) = 0;

			/*
			 *	执行复杂语句，推入字段内容;
			 */
			virtual bool ExecuteSqlPushParam(eSqlParamType database_param_type, void *buffer, unsigned int size) = 0;

			/*
			 *	执行复杂语句，正式执行并得到结果;
			 */
			virtual bool ExecuteSqlEnd(eSqlQueryError &error) = 0;

			/*
			 *	设置日志对象;
			 */
			virtual void set_log(Foundation::LogService::ILog::Ptr log) = 0;

			/*
			 *	获取自增编号;
			 */
			virtual unsigned int GetAutoIncrementID(void) = 0;
		};

	};

};

#endif
