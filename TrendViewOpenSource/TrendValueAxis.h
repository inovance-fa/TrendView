/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendValueAxisPainter.h
* @Date:2023-2
* @Description: 趋势值轴类
**************************************************/

#pragma
#ifndef __TRENDVALUEAXIS_H__
#define __TRENDVALUEAXIS_H__
#include "qcustomplot.h"

namespace Graph
{
    /**
     * @class TrendValueAxis
     * @brief 趋势Y趋势值轴类
     */
    class TrendValueAxis : public QCPAxis
    {
    public:
        explicit TrendValueAxis(QCPAxisRect *parent, AxisType type);
        virtual ~TrendValueAxis() = default;

        /**
         * @brief 窗口坐标转换为轴坐标
        */
        double pixelToCoord(double value) const;
    };

    /**
     * @class TrendValueAxisPainter
     * @brief 趋势Y趋势值轴绘制类
     */
    class TrendValueAxisPainter : public QCPAxisPainterPrivate
    {
    public:
        explicit TrendValueAxisPainter(QCustomPlot *parentPlot, QCPAxis* axis);
        virtual ~TrendValueAxisPainter() = default;

    protected:
        void draw(QCPPainter *painter) override;
    private:
        /**
         * @brief 绘制基线
        */
        QLineF DrawBaseline(QCPPainter *painter, const QPoint& origin, double xCor, double yCor);
        /**
         * @brief 绘制主刻度
        */
        void DrawTicks(QCPPainter *painter, const QPoint& origin, double xCor, double yCor);
        /**
         * @brief 绘制次刻度
        */
        void DrawSubticks(QCPPainter *painter, const QPoint& origin, double xCor, double yCor);
        /**
         * @brief 绘制轴结束图标
        */
        void DrawAxisBaseEndings(QCPPainter *painter, const QLineF& baseLine);
        /**
         * @brief 绘制轴刻度标签
        */
        QSize DrawTickLabels(QCPPainter *painter, int& margin);
        /**
         * @brief 绘制轴标签
        */
        QRect DrawAxisLabel(QCPPainter *painter, const QPoint& origin, int& margin);
        /**
         * @brief 设置选择区域
        */
        void SetSelectionBoxes(QCPPainter *painter, const QPoint& origin, const QSize& tickLabelsSize, const QRect& labelBounds);

    private:
        //曲线指针
        QCustomPlot* m_pPlot = nullptr;
        //轴指针
        QCPAxis* m_pAxis = nullptr;
    };
}

#endif // __TRENDVALUEAXIS_H__