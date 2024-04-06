/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: GlobalDefine.h
* @Date:2022-10-14
* @Description: 趋势视图全局定义
**************************************************/
#ifndef _TRENDGLOBALDEFINE_H
#define _TRENDGLOBALDEFINE_H
#include <qnamespace.h>
#include <string>
#include <QVariant>
#include <QColor>
#include <QRgb>
#include <QPen>
#include <QLabel>
#include "TrendViewOpenSource_global.h"

namespace Graph
{
    //标尺数据
    struct RulerData
    {
        QString time = "";
        struct TrendDisplayData
        {
            QRgb lineColor = QRgb();
            QString trendName = "";
            bool isVisible = true;
            QString data = "???";
        };
        QList<TrendDisplayData> displayDataList;
    };

    //Fx标尺数据
    struct FxRulerData
    {
        QString xValue = "???";
        struct FxTrendDisplayData
        {
            QRgb lineColor = QRgb();
            QString trendName = "";
            bool isVisible = true;
            QString yValue = "???";
            QString source = "???";
        };
        QList<FxTrendDisplayData> displayDataList;
    };

    //图例信息结构体
    struct LegendInfo
    {
        std::string trendName = "";
        int lineType = 0;
        QRgb lineColor = QRgb();
    };

    //趋势曲线配置信息
    struct TrendPlotOpenInfo
    {
        int lineType = 1;
        int lineWidth = 1;
        QPen linePen = QPen();
        int axisType = 1;
        std::string trendName = "";
        int trendPlotStyle = 1;
        bool isBoolType = false;
    };

    //绘制起点
    enum class DrawStartPoint
    {
        Left = 1,
        Right
    };

    //曲线最小边距
    const QMargins MINMARGINS = QMargins(43, 15, 43, 44);

    class TRENDVIEWOPENSOURCE_EXPORT GlobalDefine
    {
    public:
        /**
         * @brief 根据QRgb对象获取QColor
         * @return QColor
        */
        static QColor GetQColorFromQRGB(const QRgb& rgb);

        /**
         * @brief 文本过长时显示省略号
        */
        static QString ElideText(int maxWidth, const QString& strInfo);

        /**
         * @brief 获取样式表.
         * @param qssPath 样式表路径
         * @return 样式表字符串
         */
        static QString GetStyleSheet(const QString& qssPath);
    };

}
#endif