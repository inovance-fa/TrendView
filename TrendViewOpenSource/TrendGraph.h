/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendGraph.h
* @Date:2023-9
* @Description: Ft趋势曲线类
**************************************************/
#ifndef __TRENDGRAPH_H__
#define __TRENDGRAPH_H__
#include "qcustomplot.h"
#include "TrendViewOpenSource_global.h"

namespace Graph
{
    /**
     * @class TrendGraph
     * @brief Ft趋势曲线类
     */
    class TrendGraph : public QCPGraph
    {
    public:
        explicit TrendGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
        virtual ~TrendGraph() = default;

    protected:
        /**
         * @brief 裁剪矩形
        */
        QRect clipRect() const override;
    private:
        /**
         * @brief 扩大矩形范围
        */
        QRect EnlargeRect(const QRect& rect, int dx, int dy) const;
    };
}

#endif // __TRENDGRAPH_H__