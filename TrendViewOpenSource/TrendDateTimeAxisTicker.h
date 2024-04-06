/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendDateTimeAxisTicker.h
* @Date:2023-2
* @Description: 趋势时间轴刻度线类
**************************************************/

#pragma
#ifndef __TRENDDATETIMEAXISTICKER_H__
#define __TRENDDATETIMEAXISTICKER_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"

namespace Graph
{
    /**
     * @class TrendDateTimeAxisSticker
     * @brief 趋势时间轴刻度线类
     */
    class TrendDateTimeAxisTicker : public QCPAxisTickerDateTime
    {
    public:
        explicit TrendDateTimeAxisTicker() = default;
        virtual ~TrendDateTimeAxisTicker() = default;

    protected:

        /**
         * @brief 获得刻度步长
         * @param 刻度步长
         */
        double getTickStep(const QCPRange& range) override;

        /**
         * @brief 获得次刻度数量
         * @param 次刻度数量
         */
        int getSubTickCount(double tickStep) override;

        /**
         * @brief 获得刻度标签
         * @param 刻度标签
         */
        QString getTickLabel(double tick, const QLocale& locale, QChar formatChar, int precision) override;

        /**
         * @brief 生成刻度列表
         * @param 刻度列表
         */
        QVector<double> createTickVector(double tickStep, const QCPRange& range) override;
    };
}

#endif // __TRENDDATETIMEAXISTICKER_H__