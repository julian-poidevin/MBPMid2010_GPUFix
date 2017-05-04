#include "logger.h"

Logger::Logger(QObject *parent, QString fileName, QPlainTextEdit *editor) : QObject(parent) {

    m_editor = editor;
    m_showDate = false;
    if (!fileName.isEmpty()) {
        file = new QFile;
        file->setFileName(fileName);
        file->open(QIODevice::Append | QIODevice::Text);
    }
}

void Logger::write(const QString &value) {

    QString text = value + " ";
    if (m_showDate)
        text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text;
    QTextStream out(file);
    out.setCodec("UTF-8");
    if (file != 0) {
        out << text;
    }
    if (m_editor != 0)
    {
        QTextCursor prev_cursor = m_editor->textCursor();
        m_editor->moveCursor (QTextCursor::End);
        m_editor->insertPlainText (text);
        m_editor->setTextCursor(prev_cursor);
    }
}

void Logger::setShowDateTime(bool value) {

    m_showDate = value;
}

Logger::~Logger() {

    if (file != 0)
        file->close();
}
