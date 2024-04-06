/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendTracerLine.h
* @Date:2023-2
* @Description: 趋势标尺追踪器线
**************************************************/

#pragma
#ifndef __TRENDTRACERLINE_H__
#define __TRENDTRACERLINE_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"
#include <QFont>

namespace Graph
{
    /**
     * @class TrendTracerLine
     * @brief 趋势标尺追踪器线
     */
    class TrendTracerLine : public QCPItemLine
    {
    public:
        explicit TrendTracerLine(QCustomPlot* plot);
        virtual ~TrendTracerLine() = default;

        /**
         * @brief 初始化线
        */
        void InitLine();

        /**
         * @brief 更新位置
        */
        void UpdatePosition(qreal xValue);

        /**
         * @brief 设置是否显示    
        */
        void SetVisible(bool visible) { setVisible(visible); }

        /**
         * @brief 设置颜色 
        */
        void SetColor(const QRgb& color);

    protected:
        //是否可见
        bool m_visible = false;
        //图表
        QCustomPlot* m_plot = nullptr;
    };
}

#endif // __TRENDTRACERLINE_H__