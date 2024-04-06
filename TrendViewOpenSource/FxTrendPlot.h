/*************************************************
* @Copyright: inovance
* @Author: MYQ
* @File: FxTrendPlot.h
* @Date:2023-7
* @Description: 趋势曲线
**************************************************/

#pragma
#ifndef __FXTRENDPLOT_H__
#define __FXTRENDPLOT_H__
#include "TrendPlot.h"

namespace Graph
{
    class FxTrendRuler;
    class FxTrendLegendItem;
    /**
     * @class FxTrendPlot
     * @brief 趋势曲线
     */
    class TRENDVIEWOPENSOURCE_EXPORT FxTrendPlot : public TrendPlot
    {
    public:
        explicit FxTrendPlot(QWidget* parent = nullptr);
        virtual ~FxTrendPlot() = default;

        /**
         * @brief 设置是否显示X轴
        */
        void SetIsShowXAxis(bool isShow) override;

        /**
         * @brief 设置X轴自动刻度调整
        */
        void SetXAxisAutoRange(bool isAuto);

        /**
         * @brief 获取X轴是否自动刻度调整
        */
        bool GetXAxisAutoRange() { return m_isXAxisAutoValue; };

        /**
         * @brief 设置X轴名称
         * @param x轴名称
         */
        void SetXAxisName(const QString& name);

        /**
         * @brief 设置X轴轴起点
         * @param X轴轴起点
         */
        void SetXAxisStartPoint(double startPoint);

        /**
         * @brief 设置X轴轴终点
         * @param X轴轴终点
         */
        void SetXAxisEndPoint(double endPoint);

        /**
         * @brief 设置X轴主刻度
         * @param X轴主刻度
         */
        void SetXAxisMainTick(double mainTick, int subTick);

        /**
         * @brief 设置X轴主刻度
         * @param mainTick:X轴主刻度 upper:轴终点 lower：轴起点
         */
        void SetXAxisMainTick(double mainTick, int subTick, double lower, double upper);

        /**
         * @brief 设置X轴次刻度
         * @param X轴次刻度
         */
        void SetXAxisSubTick(double mainTick, int subTick);

        /**
         * @brief 设置X轴小数位数
         * @param X轴小数位数
         */
        void SetXAxisDecimalPlaces(int decimalPlaces);

        /**
         * @brief 设置轴字体颜色
         * @param 轴字体颜色
         */
        void SetAxisFontColor(const QRgb& wordsColor) override;

        /**
         * @brief 设置轴标签字体
         * @param 轴标签字体
         */
        void SetAxisLabelFont(QFont& font);

        /**
         * @brief 设置标尺字体
         * @param 标尺字体
         */
        void SetRulerFont(QFont& font);

        /**
         * @brief 设置趋势曲线是否显示
         */
        void SetTrendGraphVisible(const std::map<std::string, bool>& trendVisibleMap) override;

        /**
         * @brief 更新趋势数据
         */
        void UpdateTrendsData(const QCPCurve* curve, const std::vector<QPair<std::uint64_t, double>>& values);

        /**
         * @brief 清除曲线数据
         */
        void ClearGraphData() override;

        /**
         * @brief 清除曲线运行状态冗余数据
         */
        void ClearGraphEarlyData();

        /**
         * @brief 更新标尺位置
        */
        void UpdateRulerPosition() override;

        /**
         * @brief 更新标尺位置
        */
        void UpdateRulerPosition(const QPoint& pos) override;

        /**
         * @brief 固定x坐标更新标尺位置
        */
        void UpdateRulerYPosition() override;

        /**
         * @brief 设置曲线配置
         * @param plotConfigList 配置信息列表
         */
        void SetPlotConfig(const QList<TrendPlotOpenInfo>& plotConfigList);

        /**
         * @brief 获取曲线数据最后时间
         */
        double GetGraphDataFirstOrLastTime(int isForward) const override;

        /**
         * @brief 设置X轴标签文本
        */
        void SetXAxisLabel(QCPAxis* axis, const QString& name);

        /**
         * @brief 获取单条曲线数据开始或结束时间
         */
        double GetGraphDataTime(const std::string& trendName, int isForward) override;

        /**
         * @brief 设置标尺提示框参照大小
         */
        void SetRulerTextHeight(int hei);

        /**
         * @brief 设置配方显示
        */
        void SetRecipeShow(bool isShow);

        /**
         * @brief 获取配方显示
        */
        bool GetRecipeShow();

        /**
         * @brief 根据控件尺寸设置主刻度数量
        */
        void SetMainTickCountByControlSize(int xAxisSubTickCnt, int leftYAxisSubTickCnt, int rightYAxisSubTickCnt);

        /**
         * @brief 设置时间格式
        */
        void SetDataTimeFormat(int format, int hourFormat);

        /**
         * @brief 标尺左移
         * @param 
        */
        void RulerMoveLeft();

        /**
         * @brief 标尺右移
         * @param
        */
        void RulerMoveRight();

        /**
         * @brief 初始化轴
        */
        virtual void InitialAxis();

        /**
         * @brief 初始化图例
        */
        void InitialLegendBar() override;

        /**
         * @brief 初始化标尺
        */
        virtual void InitialRuler();

        /**
         * @brief 清除曲线
         * @return 数量
        */
        int ClearCurves();

        /**
         * @brief 添加曲线
         * @return 曲线指针
        */
        QCPCurve* AddCurve(QCPAxis* keyAxis, QCPAxis* valueAxis);

        /**
         * @brief 过滤数据小数位
         * @param data 原始数据
         * @param precision 小数位
         * @return 过滤后数据
        */
        double FilterDataPrecision(double data, int precision);

        const QList<QCPCurve*>& GetCurves() { return m_curves; };

    private:
        //曲线趋势
        QList<QCPCurve*> m_curves;
        //X轴刻度格式
        QSharedPointer<TrendValueAxisSticker> m_pXAxisTicker = nullptr;
        //X轴是否是自动值
        bool m_isXAxisAutoValue = false;
        //X轴是否显示
        bool m_xIsVisible = true;
        //图例项映射
        QMap<QString, FxTrendLegendItem*> m_legendItems;
        
    };
}

#endif // __FXTRENDPLOT_H__