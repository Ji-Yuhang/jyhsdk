#include "log.h"
#include <QFile>
#include <assert.h>
#include <QDebug>
#include <QCoreApplication>
static QTextStream & operator <<(QTextStream &ss, const LogData& data)
{
    ss << data.time <<" "<< data.level <<" " << data.text <<" " << data.file  <<" "<< data.line <<" ";
    return ss;
}
static QTextStream & operator >> (QTextStream &ss, LogData& data)
{
    QString time, level, text, file;
    int line;
    ss >> time >> level >> file >> line >> text;
    data.time = time;
    data.level = level;
    data.file = file;
    data.line = line;
    data.text = text;
    return ss;
}


Log* Log::g_single = 0;
Log::Log():
    timer_(new QTimer)
{
    assert(g_single == 0);
    g_single = this;
    bool su = connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
    assert(su);
    timer_->start(3000);
    bool s = connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onTimeout()));
    assert(s);
}

Log::~Log()
{
    onTimeout();
}

Log *Log::instance()
{
    return g_single;
}

//void Log::append(const QString &log)
//{
//    m_logs.append(log);
//    qDebug(log.toLocal8Bit());
//}

//void Log::append(const QString &time, const QString &level, const QString &file, int line, const QString &text)
//{
//    m_logs.append();
//}

void Log::append(const LogData &data)
{
    QString content;
    QTextStream in(&content);
    
    QString color;
    if (data.level == "ERROR" || data.level == "FATAL" )
        color = "\033[31m";
    else if (data.level == "INFO" || data.level == "LOG")
        color = "\033[32m";
        in << data.time <<" "<< color << data.level <<" \033[0m" << "\033[7m" << data.text <<" \033[0m" << data.file  <<" "<< data.line <<" ";
    
    qDebug() << content.toStdString().c_str();
    
    logs_.enqueue(data);
}

void Log::setLogFile(const QString &path)
{
    logPath_ = path;
}

void Log::setDataBaseCallBack(const boost::function<bool (const LogData &)> &databaseCallBack)
{
    databaseCallBack_ = databaseCallBack;
}


void Log::onTimeout()
{
    if (logs_.empty()) return;


    QString content;
    QTextStream in(&content);
    while (!logs_.empty()) {
        LogData data = logs_.dequeue();
        in << data <<"\r"<< endl;
        if (databaseCallBack_) {
            if (!databaseCallBack_(data)) {
                qDebug("can not writelog to database");
            }

        }
    }
    QFile file;
    if (!logPath_.isEmpty()) {
        file.setFileName(logPath_);
        if (!file.open(QIODevice::Append)) {
            qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            qDebug("\t\t\t cannot write log \t\t\t\n");
            qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
        file.write(content.toLocal8Bit());
        file.close();
    }

    /*
    Database * database = Database::instance();
    if (database && database->isLogOpen()){
        while (!logs_.empty()) {
            LogData data = logs_.dequeue();
            database->insertLog(data);
            if (data.level == "USER") {
                database->insertUserLog(data);
            }
        }
    } else {
         QFile file;
         QString path = logPath_;
         file.setFileName(path);
         if (!file.open(QIODevice::Append)) {
             qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
             qDebug("\t\t\t cannot write log \t\t\t\n");
             qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
         }
         
         QString content;
         QTextStream in(&content);
         while (!logs_.empty()) {
             LogData data = logs_.dequeue();
             in << data <<"\r"<< endl;
         }
         file.write(content.toLocal8Bit());
         file.close();
         return;
    }
    {
        QFile file;
        file.setFileName(logPath_);
        if (!file.open(QIODevice::Append)) {
            qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            qDebug("\t\t\t cannot write log \t\t\t\n");
            qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }

        QString content;
        QTextStream in(&content);
        while (!logs_.empty()) {
            LogData data = logs_.dequeue();
            in << data <<"\r"<< endl;
        }
        file.write(content.toLocal8Bit());
        file.close();
    }
    */

}

