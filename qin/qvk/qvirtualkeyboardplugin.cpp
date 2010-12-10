#include "qvirtualkeyboard.h"
#include "qvirtualkeyboardplugin.h"

#include <QtCore/QtPlugin>

QVirtualKeyboardPlugin::QVirtualKeyboardPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QVirtualKeyboardPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QVirtualKeyboardPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QVirtualKeyboardPlugin::createWidget(QWidget *parent)
{
    return new QVirtualKeyboard(parent);
}

QString QVirtualKeyboardPlugin::name() const
{
    return QLatin1String("QVirtualKeyboard");
}

QString QVirtualKeyboardPlugin::group() const
{
    return QLatin1String("");
}

QIcon QVirtualKeyboardPlugin::icon() const
{
    return QIcon();
}

QString QVirtualKeyboardPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QVirtualKeyboardPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QVirtualKeyboardPlugin::isContainer() const
{
    return false;
}

QString QVirtualKeyboardPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QVirtualKeyboard\" name=\"qVirtualKeyboard\">\n</widget>\n");
}

QString QVirtualKeyboardPlugin::includeFile() const
{
    return QLatin1String("qvirtualkeyboard.h");
}

Q_EXPORT_PLUGIN2(qvirtualkeyboardplugin, QVirtualKeyboardPlugin)
