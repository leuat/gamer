#pragma once

#include <QListData>
#include <QListWidget>
#include <QString>
#include <QVector>
#include <math.h>
#include <QDateTime>

class GMessages
{
private:
    static QListWidget* m_widget;
    static QVector<QString> m_messages;

    static int m_maxMessageCount;
    static bool m_debugMessages;
    static bool m_verbatim;

public:
    GMessages();

    static void Message(QString msg) {
        if (m_verbatim)
            m_messages.push_front("[ " +QDateTime::currentDateTime().toString()  + " ] " + msg);
        Update();

    }

    static void Debug(QString msg) {
        if (m_debugMessages)
            m_messages.push_front("[ " + QDateTime::currentDateTime().toString()  + " ] " + msg);
        Update();
    }


    static void Initialize(QListWidget* widget) {
        m_widget = widget;
    }

    static void Update() {
        if (m_widget == nullptr)
            return;
        m_widget->clear();
        for (int i=0;i<std::min(m_messages.size(), m_maxMessageCount);i++) {
            m_widget->addItem(m_messages[i]);
        }
        if (m_messages.size()!=0)
            m_widget->item(0)->setForeground(QColor(0,1,0));

    }

    static bool debugMessages();
    static void setDebugMessages(bool debugMessages);
    static bool verbatim();
    static void setVerbatim(bool verbatim);
};

