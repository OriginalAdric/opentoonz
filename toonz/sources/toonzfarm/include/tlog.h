

#ifndef TSYSLOG_H
#define TSYSLOG_H

#include "tcommon.h"
#include <QString>

class TFilePath;

#ifdef TFARMAPI
#undef TFARMAPI
#endif

#ifdef WIN32
#ifdef TFARM_EXPORTS
#define TFARMAPI __declspec(dllexport)
#else
#define TFARMAPI __declspec(dllimport)
#endif
#else
#define TFARMAPI
#endif

namespace TSysLog
{
TFARMAPI void success(const QString &msg);
TFARMAPI void warning(const QString &msg);
TFARMAPI void error(const QString &msg);
TFARMAPI void info(const QString &msg);
}

//------------------------------------------------------------------------------

class TFARMAPI TUserLog
{
public:
	TUserLog(); // used to redirect log messages to the console
	TUserLog(const TFilePath &fp);
	~TUserLog();

	void warning(const QString &msg);
	void error(const QString &msg);
	void info(const QString &msg);

private:
	class Imp;
	Imp *m_imp;
};

#endif
