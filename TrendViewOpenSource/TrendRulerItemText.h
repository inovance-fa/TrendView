/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendRulerItemText.h
* @Date:2023-2
* @Description: 趋势标尺时间显示项
**************************************************/

#pragma
#ifndef __TRENDRULERITEMTEXT_H__
#define __TRENDRULERITEMTEXT_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"

namespace Graph
{
    /**
     * @class TrendRulerItemText
     * @brief 趋势Y趋势标尺时间显示项
     */
    class TrendRulerItemText : public QCPItemText
    {
    public:
        explicit TrendRulerItemText(QCustomPlot *parentPlot);
        virtual ~TrendRulerItemText() = default;

        /**
         * @brief 设置标尺数据
         */
        void SetRulerData(const RulerData& rulerData);

        /**
         * @brief 设置标尺数据
         */
        void SetRulerColor(const QRgb& rgb) { m_rgb = rgb; };

        /**
         * @brief 设置标尺字体
        */
        void SetRulerFont(const QFont& font) { m_font = font; };

        /**
         * @brief 设置是否显示
        */
        void SetVisible(bool isVisible) { setVisible(isVisible); };

        /**
         * @brief 设置标尺选中状态
        */
        void SetSelected(bool isSelect) { setSelected(isSelect); };

        /**
         * @brief 设置是否为运行态标志
         * @param isRun 是否为运行态
        */
        void SetIsRunning(bool isRun) { m_isRunning = isRun; };

        /**
         * @brief 获取是否为运行态标志
         * @return 是否为运行态
        */
        bool GetIsRunning() { return m_isRunning; };
    protected:
        /**
         * @brief 重绘函数
         */
        virtual void draw(QCPPainter *painter) override;

        /**
         * @brief 选中处理函数
         */
        virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const override;

        /**
         * @brief 更新位置
         */
        void UpdatePosition(QRectF& rect);
        
    protected:
        RulerData m_rulerData;
        QRgb m_rgb = QRgb();
        QFont m_font = QFont();
        QRectF m_rect = QRectF();
        QCustomPlot* m_pPlot = nullptr;
        bool m_isRunning = false;
    };
}

#endif // __TRENDYAXISFIXEDSTICKER_H__