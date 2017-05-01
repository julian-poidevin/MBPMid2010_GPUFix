#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger : public QObject {

    Q_OBJECT
public:

    explicit Logger(QObject *parent, QString fileName, QPlainTextEdit *editor = 0);
    ~Logger();
    void setShowDateTime(bool value);
private:

    QFile *file;
    QPlainTextEdit *m_editor;
    bool m_showDate;
signals:

public slots:

    void write(const QString &value);
};

#endif // LOGGER_H
