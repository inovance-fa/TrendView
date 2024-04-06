/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: TrendPlot.h
* @Date:2023-12
* @Description: 趋势曲线
**************************************************/

#pragma
#ifndef __TRENDPLOT_H__
#define __TRENDPLOT_H__
#include "qcustomplot.h"
#include "GlobalDefine.h"

namespace Graph
{
    class TrendRuler;
    class TrendValueAxisSticker;
    /**
     * @class TrendPlot
     * @brief 趋势曲线
     */
    class TRENDVIEWOPENSOURCE_EXPORT TrendPlot : public QCustomPlot
    {
    public:
        explicit TrendPlot(QWidget* parent = nullptr);
        virtual ~TrendPlot() = default;

        /**
         * @brief 初始化函数
        */
        void Initial();

        /**
         * @brief 初始化标尺数据追踪器
        */
        void InitialRulerDataTracer();

        /**
         * @brief 初始化标尺位置
        */
        void InitialRulerPos();

        /**
         * @brief 设置标尺是否显示
        */
        void SetIsShowRuler(bool isShow);

        /**
         * @brief 设置刻度值是否显示
        */
        void SetIsShowTickLabels(bool isShow);

        /**
         * @brief 设置趋势绘制起点
        */
        void SetDrawStartPoint(DrawStartPoint startPoint);

        /**
         * @brief 设置是否显示左Y轴
        */
        void SetIsShowLeftYAxis(bool isShow);

        /**
         * @brief 设置左Y轴自动刻度调整
        */
        void SetLeftYAxisAutoRange(bool isAuto);

        /**
         * @brief 获取左Y轴是否自动刻度调整
        */
        bool GetLeftYAxisAutoRange() { return m_isLAxisAutoValue; };

        /**
         * @brief 设置左Y轴名称
         * @param 左Y轴名称
         */
        void SetLeftYAxisName(const QString& name);

        /**
         * @brief 设置左Y轴轴起点
         * @param 左Y轴轴起点
         */
        void SetLeftYAxisStartPoint(double startPoint);

        /**
         * @brief 设置左Y轴轴终点
         * @param 左Y轴轴终点
         */
        void SetLeftYAxisEndPoint(double endPoint);

        /**
         * @brief 设置左Y轴主刻度
         * @param 左Y轴主刻度
         */
        void SetLeftYAxisMainTick(double mainTick, int subTick);

        /**
         * @brief 设置左Y轴主刻度
         * @param mainTick:左Y轴主刻度 upper:轴终点 lower：轴起点
         */
        void SetLeftYAxisMainTick(double mainTick, int subTick, double lower, double upper);

        /**
         * @brief 设置左Y轴次刻度
         * @param 左Y轴次刻度
         */
        void SetLeftYAxisSubTick(double mainTick, int subTick);

        /**
         * @brief 设置左Y轴小数位数
         * @param 左Y轴小数位数
         */
        void SetLeftYAxisDecimalPlaces(int decimalPlaces);

        /**
         * @brief 设置是否显示右Y轴
        */
        void SetIsShowRightYAxis(bool isShow);

        /**
         * @brief 设置右Y轴自动刻度调整
        */
        void SetRightYAxisAutoRange(bool isAuto);

        /**
         * @brief 获取右Y轴是否自动刻度调整
        */
        bool GetRightYAxisAutoRange() { return m_isRAxisAutoValue; };

        /**
         * @brief 设置右Y轴名称
         * @param 右Y轴名称
         */
        void SetRightYAxisName(const QString& name);

        /**
         * @brief 设置右Y轴轴起点
         * @param 右Y轴轴起点
         */
        void SetRightYAxisStartPoint(double startPoint);

        /**
         * @brief 设置右Y轴轴终点
         * @param 右Y轴轴终点
         */
        void SetRightYAxisEndPoint(double endPoint);

        /**
         * @brief 设置右Y轴主刻度
         * @param 右Y轴主刻度
         */
        void SetRightYAxisMainTick(double mainTick, int subTick);

        /**
         * @brief 设置右Y轴主刻度
         * @param mainTick:右Y轴主刻度 upper:轴终点 lower：轴起点
         */
        void SetRightYAxisMainTick(double mainTick, int subTick, double lower, double upper);

        /**
         * @brief 设置右Y轴次刻度
         * @param 右Y轴次刻度
         */
        void SetRightYAxisSubTick(double mainTick, int subTick);

        /**
         * @brief 设置右Y轴小数位数
         * @param 右Y轴小数位数
         */
        void SetRightYAxisDecimalPlaces(int decimalPlaces);

        /**
         * @brief 设置背景颜色
         * @param 背景颜色
         */
        void SetBackgroundColor(const QRgb& color);

        /**
         * @brief 设置标尺颜色
         * @param 标尺颜色
         */
        void SetRulerColor(const QRgb& color);

        /**
         * @brief 设置轴标签文本
        */
        void SetAxisLabel(QCPAxis* axis, const QString& name);

        /**
         * @brief 设置更新鼠标样式回调函数
        */
        void SetUpdateCursorCallback(std::function<void(const QCursor&)> func) { m_updateCursorFunc = func; };

        /**
         * @brief 设置是否为运行态标志
         * @param isRun 是否为运行态
        */
        void SetIsRunning(bool isRun);

        /**
         * @brief 初始化轴刻度线
        */
        void InitialTick();

        /**
         * @brief 鼠标点转换为坐标点
        */
        QPointF MousePosConvertToRealPos(const QPointF& pos);

        /**
         * @brief 设置标尺选中状态
        */
        void SetRulerSelected(bool selected);

        /**
         * @brief 初始化自定义值轴
        */
        void InitCustomValueAxis();

        /**
         * @brief 增加自定义值轴
        */
        void AddCustomValueAxis(QCPAxis::AxisType type);

        /**
         * @brief 设置是否显示X轴
        */
        virtual void SetIsShowXAxis(bool isShow) {};

        /**
         * @brief 设置轴字体颜色
         * @param 轴字体颜色
         */
        virtual void SetAxisFontColor(const QRgb& wordsColor) {};

        /**
         * @brief 设置趋势曲线是否显示
         */
        virtual void SetTrendGraphVisible(const std::map<std::string, bool>& trendVisibleMap) {};

        /**
         * @brief 清除曲线数据
         */
        virtual void ClearGraphData() {};

        /**
         * @brief 更新标尺位置
        */
        virtual void UpdateRulerPosition() {};

        /**
         * @brief 更新标尺位置
        */
        virtual void UpdateRulerPosition(const QPoint& pos) {};

        /**
         * @brief 固定x坐标更新标尺位置
        */
        virtual void UpdateRulerYPosition() {};

        /**
         * @brief 获取曲线数据最后时间
         */
        virtual double GetGraphDataFirstOrLastTime(int isForward) const { return double(); };

        /**
         * @brief 获取单条曲线数据开始或结束时间
         */
        virtual double GetGraphDataTime(const std::string& trendName, int isForward) { return double(); };

        /**
         * @brief 初始化轴
        */
        virtual void InitialAxis() {};

        /**
         * @brief 初始化图例
        */
        virtual void InitialLegendBar() {};

        /**
         * @brief 初始化标尺
        */
        virtual void InitialRuler();

    protected:
        /**
         * @brief 鼠标按下事件
        */
        void mousePressEvent(QMouseEvent* event) override;

        /**
         * @brief 鼠标移动事件
        */
        void mouseMoveEvent(QMouseEvent* event) override;

        /**
         * @brief 鼠标释放事件
        */
        void mouseReleaseEvent(QMouseEvent* event) override;

        /**
         * @brief 滚轮事件
        */
        void wheelEvent(QWheelEvent* e)override;

    protected:
        //趋势标尺
        TrendRuler* m_pTrendRuler = nullptr;
        //鼠标按下标志
        bool m_bMousePress = false;
        //左Y轴刻度格式
        QSharedPointer<TrendValueAxisSticker> m_pLYAxisTicker = nullptr;
        //右Y轴刻度格式
        QSharedPointer<TrendValueAxisSticker> m_pRYAxisTicker = nullptr;
        //标尺当前位置
        QPoint m_rulerCurPos;
        //左Y轴是否是自动值
        bool m_isLAxisAutoValue = false;
        //右Y轴是否是自动值
        bool m_isRAxisAutoValue = false;
        //左Y轴是否显示
        bool m_leftIsVisible = true;
        //右Y轴是否显示
        bool m_rightIsVisible = false;
        //当前轴文本颜色
        QRgb m_curWordsColor = qRgba(0x0C, 0x1A, 0x34, 0xFF);
        //更新鼠标样式回调函数
        std::function<void(const QCursor&)> m_updateCursorFunc = nullptr;
        //标尺当前位置
        QPointF m_curPos = QPointF();
    };
}

#endif // __TRENDPLOT_H__