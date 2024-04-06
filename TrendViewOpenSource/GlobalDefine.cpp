#include "GlobalDefine.h"
#include <QJsonObject>
#include <QMouseEvent>

using namespace Graph;

QColor GlobalDefine::GetQColorFromQRGB(const QRgb& rgb)
{
    return QColor(qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb));
}

QString GlobalDefine::ElideText(int maxWidth, const QString& strInfo)
{
    if (strInfo.length() <= maxWidth)
    {
        return strInfo;
    }

    return strInfo.left(maxWidth - 1) + "...";
}

QString GlobalDefine::GetStyleSheet(const QString& qssPath)
{
    QFile file(qssPath);
    auto res = QString();
    if (!file.open(QFile::ReadOnly))
    {
        return res;
    }

    res = QString::fromUtf8(file.readAll());
    file.close();
    return res;
}
