#include "gmessages.h"

QListWidget* GMessages::m_widget = nullptr;
QVector<QString> GMessages::m_messages;

int GMessages::m_maxMessageCount = 5;
bool GMessages::m_debugMessages = false;
bool GMessages::m_verbatim = true;


bool GMessages::debugMessages()
{
    return m_debugMessages;
}

void GMessages::setDebugMessages(bool debugMessages)
{
    m_debugMessages = debugMessages;
}

bool GMessages::verbatim()
{
    return m_verbatim;
}

void GMessages::setVerbatim(bool verbatim)
{
    m_verbatim = verbatim;
}

GMessages::GMessages()
{

}
