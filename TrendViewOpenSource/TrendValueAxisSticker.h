/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendValueAxisSticker.h
* @Date:2023-2
* @Description: 趋势值轴刻度类
**************************************************/

#pragma
#ifndef __TRENDVALUEAXISTICKER_H__
#define __TRENDVALUEAXISTICKER_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"

namespace Graph
{
    /**
     * @class TrendValueAxisSticker
     * @brief 趋势Y趋势值轴刻度类
     */
    class TrendValueAxisSticker : public QCPAxisTickerFixed
    {
    public:
        explicit TrendValueAxisSticker() = default;
        virtual ~TrendValueAxisSticker() = default;

        /**
         * @brief 设置子刻度数量
         */
        void SetSubTickCount(int subTickCount) { m_subTickCount = subTickCount - 1; };

        /**
         * @brief 设置主刻度数量
         */
        void SetMainTickCount(int mainTickCount) { m_mainTickCount = mainTickCount; };

        /**
         * @brief 设置主刻度步长
         */
        void SetMainTickStep(double mainTickStep) { m_mainTickStep = mainTickStep; };

        /**
         * @brief 设置是否自动刻度
         */
        void SetIsAutoTick(bool isAutoTick) { m_bIsAutoTick = isAutoTick; };

        /**
         * @brief 设置小数位数
         */
        void SetDecimalPlaces(int decimalPlaces) { m_decimalPlaces = decimalPlaces; };

        /**
         * @brief 获取是否需要绘制量程上限
         */
        bool GetIsNeedPaintUpperLabel() { return m_bIsNeedPaintUpperLabel; }

        /**
         * @brief 获取小数位数
         */
        int GetDecimalPlaces() { return m_decimalPlaces; };
    protected:

        /**
         * @brief 获得刻度步长
         * @param 刻度步长
         */
        double getTickStep(const QCPRange& range) override;

        /**
         * @brief 获取子刻度数量
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

        /**
         * @brief 生成次刻度列表
         * @param 次刻度列表
         */
        QVector<double> createSubTickVector(int subTickCount, const QVector<double> &ticks) override;
    private:
        /**
         * @brief 生成次刻度列表
         */
        int GetMainTickDataPrecision(const QVector<double> &ticks, int precision);
        /**
         * @brief 根据值和小数点位数返回格式化的结果
         */
        QString FormatDouble(double value, int decimals);
    protected:
        //主刻度数量
        int m_mainTickCount = 0;
        //主刻度步长
        double m_mainTickStep = 0;
        //子刻度数量
        int m_subTickCount = 0;
        //是否为自动刻度
        bool m_bIsAutoTick = false;
        //小数点位数
        int m_decimalPlaces = 0;
        //是否需要绘制上限刻度标签
        bool m_bIsNeedPaintUpperLabel = true;
        //自动值时的精度
        int m_precision = 3;
    };
}

#endif // __TRENDVALUEAXISTICKER_H__