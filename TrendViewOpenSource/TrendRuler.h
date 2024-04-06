/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendRuler.h
* @Date:2023-2
* @Description: 趋势标尺
**************************************************/

#pragma
#ifndef __TRENDRULER_H__
#define __TRENDRULER_H__
#include "GlobalDefine.h"
#include <QFont>
#include <QList>
#include <QMap>

class QCustomPlot;
namespace Graph
{
    class TrendTracer;
    class TrendTracerLine;
    class TrendRulerItemText;
    /**
     * @class TrendPlot
     * @brief 趋势曲线
     */
    class TrendRuler : public QObject
    {
    public:
        explicit TrendRuler(QCustomPlot* plot, QObject* parent = nullptr);
        virtual ~TrendRuler();

        /**
         * @brief 初始化函数
        */
        void Initial();

        /**
         * @brief 设置标尺颜色
        */
        void SetRulerColor(const QRgb& rgb);

        /**
         * @brief 设置标尺字体
        */
        void SetRulerFont(const QFont& font);

        /**
         * @brief 设置是否显示标尺
        */
        void SetIsShow(bool isShow);

        /**
         * @brief 标尺是否显示
        */
        bool IsShow() { return m_isShow; };

        /**
         * @brief 更新位置
        */
        virtual void UpdatePosition(qreal xValue, qreal yValue);

        /**
         * @brief 设置标尺选中状态
        */
        void SetSelected(bool isSelect);

        /**
         * @brief 设置某个数据追踪器显示状态
        */
        void SetDataTracerVisible(int index, bool visible);

        /**
         * @brief 初始化标尺文本框
        */
        virtual void InitialRulerText();

        /**
         * @brief 设置时间格式
        */
        void setDateTimeFormat(const QString& format) { m_formatStr = format; };
        /**
         * @brief 设置曲线变量类型map
        */
        void SetGraphVarTypeMap(const QMap<QCPGraph*, bool>& graphToVarType) { m_graphToVarType = graphToVarType; };
        /**
         * @brief 获取曲线变量类型map
        */
        void GetGraphVarTypeMap(QMap<QCPGraph*, bool>& graphToVarType) { graphToVarType = m_graphToVarType; };
        /**
         * @brief 设置是否为运行态标志
         * @param isRun 是否为运行态
        */
        void SetIsRunning(bool isRun);
        /**
         * @brief 获取是否为运行态标志
         * @return 是否为运行态
        */
        bool GetIsRunning();
    protected:
        //趋势图
        QCustomPlot* m_pPlot = nullptr;
        //是否显示追踪器
        bool m_isShow = true;
        //数据追踪器列表
        QList<TrendTracer*> m_tracerList;
        //时间追踪器
        TrendTracer* m_xAsixTracer = nullptr;
        //标尺线
        TrendTracerLine* m_pTracerLine = nullptr;
        //曲线是否显示
        QList<bool> m_bGraphVisibleList;
        //标尺文本
        TrendRulerItemText* m_pRulerItemText = nullptr;
        //时间格式
        QString m_formatStr = "yyyy-MM-dd\nhh:mm:ss";
        //曲线与变量类型的映射关系
        QMap<QCPGraph*, bool> m_graphToVarType{};
    };
}

#endif // __TRENDRULER_H__