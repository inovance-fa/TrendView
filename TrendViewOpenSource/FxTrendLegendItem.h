/*************************************************
* @Copyright: inovance
* @Author: MYQ
* @File: FxTrendLegendItem.h
* @Date:2023-7
* @Description: Fx趋势自定义图例项
**************************************************/

#pragma
#ifndef __FXTRENDLEGENDITEM_H__
#define __FXTRENDLEGENDITEM_H__
#include "TrendLegendItem.h"

namespace Graph
{
    /**
     * @class FxTrendLegendItem
     * @brief 趋势自定义图例项
     */
    class FxTrendLegendItem : public TrendLegendItem
    {
    public:
        explicit FxTrendLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);
        virtual ~FxTrendLegendItem() = default;
   
    protected:

        /**
         * @brief 绘制图例图标
         */
        void DrawLegendIcon(QCPPainter* painter, const QRectF& rect) const override;
    };
}

#endif // 