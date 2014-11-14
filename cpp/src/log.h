#ifndef LOG_HXX
#define LOG_HXX

#include <QObject>
#include <QTextStream>
#include <QDateTime>
#include <QQueue>
#include <QTimer>
#include <QDebug>
#include <boost/function.hpp>

struct LogData{
    QString time;
    QString level;
    QString file;
    int     line;
    QString text;
};
class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log();
    ~Log();
    static Log *instance();
//    void append(const QString& log);
//    void append(const QString& time, const QString& level, const QString& file, int line, const QString& text);
    void append(const LogData& data);
    void setLogFile(const QString& path);
    void setDataBaseCallBack( const boost::function<void (const LogData&) > & databaseCallBack );

public Q_SLOTS:
    void onTimeout();
private:
    boost::function<void (const LogData&) > databaseCallBack_;
    QString logPath_;
    QTimer *timer_;
    QQueue<LogData> logs_;
    static Log * g_single;
};



template<class T1,class T2,class T3,class T4,class T5>
void addLog(const char* level,const char * file, int line,T1 t1,T2 t2, T3 t3, T4 t4,T5 t5)
{
    QString fileStr(file);
 //   fileStr.replace("..\\ServerSource\\","");
    if (fileStr.contains("ServerSource"))
        fileStr = fileStr.mid(16);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString text;
    QTextStream textStream(&text);
    textStream << t1 <<" "<< t2 <<" "<< t3 <<" "<< t4 <<" "<< t5;
    
    LogData data;
    data.level = level;
    data.file = fileStr;
    data.line = line;
    data.time = time;
    data.text = text;
    
    Log::instance()->append(data);
}

inline static QTextStream & operator <<(QTextStream &ss, const std::string & s)
{
    ss << QString::fromStdString(s);
    return ss;
}


template<class T>
static QTextStream & operator <<(QTextStream &ss, const QList<T>& list)
{
    ss << "(";
    if (list.size() > 0)
         ss << list.at(0);
    for (int i = 1; i < list.size(); ++i)
        ss << ", "<<list.at(i);
    ss <<")";
    return ss;
}
// log, debug, info, warn, error, fatal 
#define INFO5(A,B,C,D,E) addLog("INFO", __FILE__, __LINE__,A,B,C,D,E);
#define INFO4(A,B,C,D) addLog("INFO", __FILE__, __LINE__,A,B,C,D,"");
#define INFO3(A,B,C) addLog("INFO", __FILE__, __LINE__,A,B,C,"","");
#define INFO2(A,B) addLog("INFO", __FILE__, __LINE__,A,B,"","","");
#define INFO1(A) addLog("INFO", __FILE__, __LINE__,A,"","","","");

#define DEBUG5(A,B,C,D,E) addLog("DEBUG", __FILE__, __LINE__,A,B,C,D,E);
#define DEBUG4(A,B,C,D) addLog("DEBUG", __FILE__, __LINE__,A,B,C,D,"");
#define DEBUG3(A,B,C) addLog("DEBUG", __FILE__, __LINE__,A,B,C,"","");
#define DEBUG2(A,B) addLog("DEBUG", __FILE__, __LINE__,A,B,"","","");
#define DEBUG1(A) addLog("DEBUG", __FILE__, __LINE__,A,"","","","");

#define WARN5(A,B,C,D,E) addLog("WARN", __FILE__, __LINE__,A,B,C,D,E);
#define WARN4(A,B,C,D) addLog("WARN", __FILE__, __LINE__,A,B,C,D,"");
#define WARN3(A,B,C) addLog("WARN", __FILE__, __LINE__,A,B,C,"","");
#define WARN2(A,B) addLog("WARN", __FILE__, __LINE__,A,B,"","","");
#define WARN1(A) addLog("WARN", __FILE__, __LINE__,A,"","","","");

#define ERROR5(A,B,C,D,E) addLog("ERROR", __FILE__, __LINE__,A,B,C,D,E);
#define ERROR4(A,B,C,D) addLog("ERROR", __FILE__, __LINE__,A,B,C,D,"");
#define ERROR3(A,B,C) addLog("ERROR", __FILE__, __LINE__,A,B,C,"","");
#define ERROR2(A,B) addLog("ERROR", __FILE__, __LINE__,A,B,"","","");
#define ERROR1(A) addLog("ERROR", __FILE__, __LINE__,A,"","","","");

#define FATAL5(A,B,C,D,E) addLog("FATAL", __FILE__, __LINE__,A,B,C,D,E);
#define FATAL4(A,B,C,D) addLog("FATAL", __FILE__, __LINE__,A,B,C,D,"");
#define FATAL3(A,B,C) addLog("FATAL", __FILE__, __LINE__,A,B,C,"","");
#define FATAL2(A,B) addLog("FATAL", __FILE__, __LINE__,A,B,"","","");
#define FATAL1(A) addLog("FATAL", __FILE__, __LINE__,A,"","","","");

#define LOG5(A,B,C,D,E) addLog("INFO", __FILE__, __LINE__,A,B,C,D,E);
#define LOG4(A,B,C,D) addLog("INFO", __FILE__, __LINE__,A,B,C,D,"");
#define LOG3(A,B,C) addLog("INFO", __FILE__, __LINE__,A,B,C,"","");
#define LOG2(A,B) addLog("INFO", __FILE__, __LINE__,A,B,"","","");
#define LOG1(A) addLog("INFO", __FILE__, __LINE__,A,"","","","");

#define USERLOG5(A,B,C,D,E) addLog("USER", __FILE__, __LINE__,A,B,C,D,E);
#define USERLOG4(A,B,C,D) addLog("USER", __FILE__, __LINE__,A,B,C,D,"");
#define USERLOG3(A,B,C) addLog("USER", __FILE__, __LINE__,A,B,C,"","");
#define USERLOG2(A,B) addLog("USER", __FILE__, __LINE__,A,B,"","","");
#define USERLOG1(A) addLog("USER", __FILE__, __LINE__,A,"","","","");

#endif // LOG_HXX
