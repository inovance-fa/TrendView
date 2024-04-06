/*************************************************
* @Copyright: inovance
* @Author: WFL
* @File: FtTrendPlot.h
* @Date:2023-2
* @Description: 趋势曲线
**************************************************/

#pragma
#ifndef __FTTRENDPLOT_H__
#define __FTTRENDPLOT_H__
#include "TrendPlot.h"

namespace Graph
{
    class TrendDateTimeAxisTicker;
    class TrendLegendItem;
    /**
     * @class FtTrendPlot
     * @brief 趋势曲线
     */
    class TRENDVIEWOPENSOURCE_EXPORT FtTrendPlot : public TrendPlot
    {
    public:
        explicit FtTrendPlot(QWidget* parent = nullptr);
        virtual ~FtTrendPlot() = default;

        /**
         * @brief 设置是否显示X轴
        */
        void SetIsShowXAxis(bool isShow) override;

        /**
         * @brief 设置X轴时间格式
        */
        void SetXAxisDataTimeFormat(int format, int hourFormat);

        /**
         * @brief 设置X轴时间范围
        */
        void SetXAxisTimeRange(int timeRange, int timeUnit, bool isRunning = false);

        /**
         * @brief 设置X轴时间单位
         * @param X轴时间单位
         */
        void SetXAxisTimeUnit(int timeRange, int timeUnit);

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
        void UpdateTrendsData(const QCPGraph* graph, const std::vector<QPair<std::uint64_t, double>>& values);

        /**
         * @brief 设置时间轴范围
         */
        void UpdateXAxisRange(double lower, double upper);

        /**
         * @brief 清除曲线数据
         */
        void ClearGraphData() override;

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
         * @brief 清楚时间之前的数据
        */
        void ClearGraphData(double time);

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
         * @brief 获取单条曲线数据开始或结束时间
         */
        double GetGraphDataTime(const std::string& trendName, int isForward) override;
        /**
         * @brief 设置曲线变量类型map
        */
        void UpdateGraphVarTypeMap(const QList<QPair<std::string, bool>>& infoList);
        /**
         * @brief 根据控件高度设置主刻度数量
        */
        void SetMainTickCountByControlHeight(int leftYAxisSubTickCnt, int rightYAxisSubTickCnt);

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
         * @brief 增加自定义曲线
        */
        QCPGraph *AddGraph(QCPAxis *keyAxis = nullptr, QCPAxis *valueAxis = nullptr);

    private:
        //X轴刻度格式
        QSharedPointer<TrendDateTimeAxisTicker> m_pXAxisTicker = nullptr;
        //图例项映射
        QMap<QString, TrendLegendItem*> m_legendItems;
    };
}

#endif // __FTTRENDPLOT_H__