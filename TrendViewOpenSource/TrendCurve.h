/*************************************************
* @Copyright: inovance
* @Author: MYQ
* @File: TrendCurve.h
* @Date:2023-9
* @Description: Fx趋势曲线类
**************************************************/
#ifndef __TRENDCURVE_H__
#define __TRENDCURVE_H__
#include "qcustomplot.h"
#include "TrendViewOpenSource_global.h"

namespace Graph
{
    /**
     * @class TrendCurve
     * @brief Fx趋势曲线类
     */
    class TrendCurve : public QCPCurve
    {
    public:
        explicit TrendCurve(QCPAxis *keyAxis, QCPAxis *valueAxis);
        virtual ~TrendCurve() = default;

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

#endif // __TRENDCURVE_H__