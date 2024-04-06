/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendTracer.h
* @Date:2023-2
* @Description: 趋势追踪器
**************************************************/

#pragma
#ifndef __TRENDTRACER_H__
#define __TRENDTRACER_H__
#include "GlobalDefine.h"
#include <QFont>

class QCustomPlot;
class QCPItemTracer;
class QCPItemText;
class QCPItemLine;
namespace Graph
{
    /**
     * @class TrendTracer
     * @brief 趋势曲线标尺追踪器
     */
    class TrendTracer : public QObject
    {
    public:

        enum TracerType
        {
            XAxisTracer,//在图中显示时间值
            DataTracer//在图中显示数值
        };

        explicit TrendTracer(QCustomPlot* plot, TracerType type, QObject* parent = nullptr);
        virtual ~TrendTracer();

        /**
         * @brief 初始化函数
        */
        void Initial();

        /**
         * @brief 设置追踪器颜色
        */
        void SetColor(const QRgb& color);

        /**
         * @brief 设置画刷
        */
        void SetBrush(const QBrush& brush);

        /**
         * @brief 设置追踪器文本
        */
        void SetText(const QString& text);

        /**
         * @brief 设置标签画笔
        */
        void SetLabelPen(const QPen& pen);

        /**
         * @brief 更新位置
        */
        void UpdatePosition(qreal xValue, qreal yValue, bool isInValid);

        /**
         * @brief 设置是否显示
        */
        void SetVisible(bool isVisible);

        /**
         * @brief 设置标尺选中状态
        */
        void SetSelected(bool isSelect);

        /**
         * @brief 设置追踪器字体
        */
        void SetTracerFont(const QFont& font);

    private:
        //追踪器类型
        TracerType m_type;
        //是否可见
        bool m_isVisible = true;
        //曲线指针
        QCustomPlot* m_plot = nullptr;
        //追踪器
        QCPItemTracer* m_tracer = nullptr;
        //显示文本
        QCPItemText* m_label = nullptr;
        //箭头
        QCPItemLine* m_arrow = nullptr;
    };
}

#endif // __TRENDTRACER_H__