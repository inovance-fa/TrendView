#include "TrendValueAxisSticker.h"
#include <unordered_set>
using namespace Graph;

double TrendValueAxisSticker::getTickStep(const QCPRange& range)
{
    if (m_bIsAutoTick)
    {
        return QCPAxisTicker::getTickStep(range);
    }
    else
    {
        return QCPAxisTickerFixed::getTickStep(range);
    }
}

int TrendValueAxisSticker::getSubTickCount(double tickStep)
{
    return m_subTickCount;
}

QString TrendValueAxisSticker::getTickLabel(double tick, const QLocale& locale, QChar formatChar, int precision)
{
    return m_bIsAutoTick ? FormatDouble(tick, m_precision)
                         : FormatDouble(tick, m_decimalPlaces);
}

QString TrendValueAxisSticker::FormatDouble(double value, int decimals)
{
    QString strValue = QString::number(value, 'f', decimals);

    double convertedValue = strValue.toDouble();
    double threshold = 1.0 / qPow(10, decimals + 1);
    if (qAbs(convertedValue) < threshold) 
    {
        strValue = "0";
        if (decimals > 0)
        {
            strValue += "." + QString(decimals, '0');
        }
    }

    return strValue;
}

QVector<double> TrendValueAxisSticker::createTickVector(double tickStep, const QCPRange& range)
{  
    if (m_mainTickCount < 2)
    {
        m_mainTickCount = 2;
    }

    double step;
    if (m_bIsAutoTick)
    {
        step = range.size() / double(m_mainTickCount - 1 + 1e-10);
    }
    else
    {
        step = m_mainTickStep;
    }

    QVector<double> result;
    result.push_back(range.lower);
    for (int i = 1; i < m_mainTickCount; ++i)
    {
        result.push_back(result.back() + step);
    }

    if (result.size() > 1)
    {
        if (!m_bIsAutoTick)
        {
            m_bIsNeedPaintUpperLabel = qFuzzyCompare(result[result.size() - 1], range.upper);
        }
        result[result.size() - 1] = range.upper;
    }

    if (m_bIsAutoTick)
    {
        m_precision = GetMainTickDataPrecision(result, 3);
    }

    return result;
}

bool IsDivisible(double numerator, double denominator) 
{
    double epsilon = 1e-10;
    double remainder = std::fmod(numerator, denominator);
    return std::fabs(remainder) < epsilon;
}

QVector<double> TrendValueAxisSticker::createSubTickVector(int subTickCount, const QVector<double>& ticks)
{
    QVector<double> result;
    if (subTickCount <= 0 || ticks.size() < 2)
    {
        return result;
    }

    if (m_bIsAutoTick)
    {
        result.reserve((ticks.size() - 1) * subTickCount);
        for (int i = 1; i < ticks.size(); ++i)
        {
            double subTickStep = (ticks.at(i) - ticks.at(i - 1)) / double(subTickCount + 1);
            for (int k = 1; k <= subTickCount; ++k)
            {
                result.append(ticks.at(i - 1) + k * subTickStep);
            }
        }
    }
    else
    {
        auto subTickStep = m_mainTickStep / (double)(subTickCount + 1);
        for (int i = 1; i < ticks.size(); ++i)
        {
            double subTickCnt = 0;
            if (IsDivisible(ticks.at(i) - ticks.at(i - 1), subTickStep))
            {
                subTickCnt = (ticks.at(i) - ticks.at(i - 1)) / subTickStep - 1;
            }
            else
            {
                subTickCnt = (ticks.at(i) - ticks.at(i - 1)) / subTickStep;
            }

            for (int k = 1; k <= subTickCnt; ++k)
            {
                result.append(ticks.at(i - 1) + k * subTickStep);
            }
        }
    }

    return result;
}

int TrendValueAxisSticker::GetMainTickDataPrecision(const QVector<double>& ticks, int precision)
{
    std::unordered_set<std::string> uniqueValues;
    bool isExistEqual = false;
    for (const auto& data : ticks)
    {
        auto tempStr = QString::number(data, 'f', precision).toStdString();
        if (uniqueValues.find(tempStr) != uniqueValues.end())
        {
            isExistEqual = true;
            break;;
        }
        uniqueValues.insert(tempStr);
    }
    if (isExistEqual)
    {
        return GetMainTickDataPrecision(ticks, precision + 1);
    }
    else
    {
        return precision;
    }
}
