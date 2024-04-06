/*************************************************
* @Copyright: inovance
* @Author: MYQ
* @File: FxTrendRuler.h
* @Date:2023-7
* @Description: Fx趋势标尺
**************************************************/

#pragma
#ifndef __FXTRENDRULER_H__
#define __FXTRENDRULER_H__
#include "TrendRuler.h"

class QCustomPlot;
namespace Graph
{
    class TrendTracer;
    class TrendTracerLine;
    class FxTrendRulerItemText;
    /**
     * @class FxTrendRuler
     * @brief 趋势曲线
     */
    class FxTrendRuler : public TrendRuler
    {
    public:
        explicit FxTrendRuler(QCustomPlot* plot, QObject* parent = nullptr);
        virtual ~FxTrendRuler();

        /**
         * @brief 更新位置
        */
        void UpdatePosition(qreal xValue, qreal yValue) override;

        /**
         * @brief 初始化标尺文本框
        */
        void InitialRulerText() override;

        /**
         * @brief 设置视图曲线
        */
        void SetCurvesParam(const QList<QCPCurve*> &curves);

        /**
         * @brief 获取视图曲线
        */
        QList<QCPCurve*> GetCurvesParam();

        /**
         * @brief 设置标尺提示框参照大小
         */
        void SetRulerTextHeight(int hei);

        /**
         * @brief 设置配方标尺
        */
        void SetRecipeShowRuler(bool isShow);

        /**
         * @brief 获取配方显示
        */
        bool GetRecipeShow() { return m_isRecipe; }

        /**
         * @brief 设置时间格式
        */
        void setDateTimeFormat(const QString& format) { m_fxFormatStr = format; };

        /**
         * @brief 设置x轴精度
        */
        void SetXAxisPrecision(int precision) { m_xAxisPrecision = precision ; }

        /**
         * @brief 获取x轴精度
        */
        int GetXAxisPrecision() { return m_xAxisPrecision; }

    private:
        //时间格式
        QString m_fxFormatStr = "yyyy-MM-dd hh:mm:ss";
        //是否显示配方ID
        bool m_isRecipe = false;
        //x轴小数位
        int m_xAxisPrecision = 0;

    };
}

#endif // __TRENDRULER_H__