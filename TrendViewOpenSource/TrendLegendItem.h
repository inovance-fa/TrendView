/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendLegendItem.h
* @Date:2023-3
* @Description: 趋势自定义图例项
**************************************************/

#pragma
#ifndef __TRENDLEGENDITEM_H__
#define __TRENDLEGENDITEM_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"
#include "TrendViewOpenSource_global.h"

namespace Graph
{
    /**
     * @class TrendLegendItem
     * @brief 趋势自定义图例项
     */
    class TrendLegendItem : public QCPPlottableLegendItem
    {
    public:
        explicit TrendLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);
        virtual ~TrendLegendItem() = default;
   
    protected:
        /**
         * @brief 重绘函数
         */
        void draw(QCPPainter *painter) override;

        /**
         * @brief 最小尺寸
         */
        QSize minimumOuterSizeHint() const override;
    protected:

        /**
         * @brief 绘制图例图标
         */
        virtual void DrawLegendIcon(QCPPainter* painter, const QRectF& rect) const;

        /**
         * @brief 应用反走样
         */
        void ApplyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;
    };
}

#endif // __TRENDYAXISFIXEDSTICKER_H__