/*************************************************
* @Copyright: inovance
* @Author: MYQ
* @File: FxTrendRulerItemText.h
* @Date:2023-7
* @Description: Fx趋势标尺时间显示项
**************************************************/

#pragma
#ifndef __FXTRENDRULERITEMTEXT_H__
#define __FXTRENDRULERITEMTEXT_H__

#include "TrendRulerItemText.h"

namespace Graph
{
    /**
     * @class FxTrendRulerItemText
     * @brief 趋势标尺显示项
     */
    class FxTrendRulerItemText : public TrendRulerItemText
    {
    public:
        explicit FxTrendRulerItemText(QCustomPlot *parentPlot);
        virtual ~FxTrendRulerItemText() = default;

        /**
         * @brief 设置标尺数据
         */
        void SetRulerData(const FxRulerData& rulerData);

        /**
         * @brief 设置视图曲线
        */
        void SetCurvesParam(const QList<QCPCurve*>& curves);

        /**
         * @brief 获取视图曲线
        */
        const QList<QCPCurve*>& GetCurvesParam();

        /**
         * @brief 设置标尺提示框参照大小
         */
        void SetRulerTextHeight(int hei);

    protected:
        /**
         * @brief 重绘函数
         */
        void draw(QCPPainter *painter) override;

        /**
         * @brief 滚轮事件
         */
        void wheelEvent(QWheelEvent* event) override;
        
    protected:
        //标尺数据
        FxRulerData m_FxRulerData;
        //曲线指针
        QList<QCPCurve*> m_plotCurves = QList<QCPCurve*>();
        //数据最大高度
        int m_height = 0;
    };
}

#endif //