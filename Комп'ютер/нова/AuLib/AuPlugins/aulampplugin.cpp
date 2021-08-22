#include "../AuWidget/aulamp.h"
#include "aulampplugin.h"

#include <QtPlugin>

AuLampPlugin::AuLampPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void AuLampPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool AuLampPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *AuLampPlugin::createWidget(QWidget *parent)
{
    return new AuLamp(parent);
}

QString AuLampPlugin::name() const
{
    return QLatin1String("AuLamp");
}

QString AuLampPlugin::group() const
{
    return QLatin1String("AuLib Widgets");
}

QIcon AuLampPlugin::icon() const
{
    return QIcon(QLatin1String(":/lampOn.png"));
}

QString AuLampPlugin::toolTip() const
{
    return QLatin1String("");
}

QString AuLampPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool AuLampPlugin::isContainer() const
{
    return false;
}

QString AuLampPlugin::domXml() const
{
    return QLatin1String("<widget class=\"AuLamp\" name=\"auLamp\">\n</widget>\n");
}

QString AuLampPlugin::includeFile() const
{
    return QLatin1String("AuWidget/aulamp.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(auplugin, AuLampPlugin)
#endif // QT_VERSION < 0x050000
